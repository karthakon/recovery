#include <pebble.h>
#include "smoother.h"
#include "storage.h"
#include "sleep_stage.h"

// ---- Frozen-spec constants (recovery-smoothing-spec-v1.md, 7f6c250) ----
// Three states, mapped to a dense 0..2 index for the Viterbi lattice.
// S_AW=Awake, S_LT=Light, S_RM=REM. SleepStage enum values differ
// (StageAwake=0, StageLight=1, StageREM=3), so map explicitly.
#define NS 3
#define S_AW 0
#define S_LT 1
#define S_RM 2

#define MAX_EPOCHS 960   // spec section 7 cap (storage caps lower first)
#define ONSET_RUN  5     // 5-in-a-row non-Awake => onset (matches live rule)

// Fixed-point: int16_t log2-probability scaled by 256 (spec section 2).
// LOG2_256(x) = round(log2(x) * 256). Precomputed, no floats at runtime.
// Transition matrix (spec section 2), rows FROM, cols TO:
//        ->Awake  ->REM    ->Light
// Awake  0.833    0.017    0.150
// REM    0.014    0.944    0.042
// Light  0.018    0.022    0.960
// Stored below in [from][to] with to-order {Awake, Light, REM}.
static const int16_t TRANS[NS][NS] = {
  // from Awake:  Awake 0.833   Light 0.150   REM 0.017
  { -67,  -701, -1505 },
  // from Light:  Awake 0.018   Light 0.960   REM 0.022
  { -1484,  -15, -1410 },
  // from REM:    Awake 0.014   Light 0.042   REM 0.944
  { -1577, -1171,  -21 },
};

// Emission (spec section 3): P(correct)=0.75, P(each other)=0.125.
// log2(0.75)*256 = -106 ; log2(0.125)*256 = -768.
#define EMIT_HIT  -106
#define EMIT_MISS -768

// REM-latency prior (spec section 3), multiplier on REM emission only:
// 0-45 min: 0.25 ; 45-90 min: 0.50 ; 90+ : 1.00.
// log2(0.25)*256 = -512 ; log2(0.50)*256 = -256 ; log2(1.0)*256 = 0.
#define REM_LAT_EARLY -512
#define REM_LAT_MID   -256
#define REM_LAT_LATE     0

// Minimum episode durations, minutes (spec section 4).
#define MIN_EP_REM   5
#define MIN_EP_LIGHT 3
#define MIN_EP_AWAKE 2
#define MIN_EP_PASSES 3

// ---- state-index <-> SleepStage mapping ----
static uint8_t stage_to_idx(uint8_t st) {
  switch (st) {
    case StageAwake: return S_AW;
    case StageREM:   return S_RM;
    default:         return S_LT;  // StageLight (and any non-emitted)
  }
}
static uint8_t idx_to_stage(uint8_t ix) {
  switch (ix) {
    case S_AW: return StageAwake;
    case S_RM: return StageREM;
    default:   return StageLight;
  }
}

// Working arrays (file-scope to keep them off the stack).
static uint8_t s_obs[MAX_EPOCHS];   // observed state index per epoch
static uint8_t s_path[MAX_EPOCHS];  // decoded state index per epoch
static uint8_t s_bp[MAX_EPOCHS][NS];// Viterbi backpointers

// REM-latency multiplier for an epoch given minutes since onset.
static int16_t rem_lat_mult(int mins_since_onset) {
  if (mins_since_onset < 0)  return REM_LAT_EARLY; // before onset: suppress
  if (mins_since_onset < 45) return REM_LAT_EARLY;
  if (mins_since_onset < 90) return REM_LAT_MID;
  return REM_LAT_LATE;
}

// Emission log-prob of observing s_obs[t] given hidden state `hid`,
// including the REM-latency prior on the REM state.
static int16_t emit_cost(uint16_t t, uint8_t hid, int onset_idx) {
  int16_t e = (s_obs[t] == hid) ? EMIT_HIT : EMIT_MISS;
  if (hid == S_RM) {
    int mins = (onset_idx < 0) ? -1 : ((int)t - onset_idx);
    e += rem_lat_mult(mins);
  }
  return e;
}

// Find onset epoch index: first start of a run of ONSET_RUN consecutive
// non-Awake observed epochs. Returns -1 if none.
static int find_onset(uint16_t n) {
  uint16_t run = 0;
  for (uint16_t t = 0; t < n; t++) {
    if (s_obs[t] != S_AW) {
      run++;
      if (run >= ONSET_RUN) return (int)(t - (ONSET_RUN - 1));
    } else {
      run = 0;
    }
  }
  return -1;
}

// Viterbi decode over s_obs[0..n-1] into s_path[0..n-1].
static void viterbi(uint16_t n, int onset_idx) {
  int32_t cost[NS];   // best cost to reach each state at current t
  int32_t ncost[NS];
  // t = 0: no transition, uniform start.
  for (uint8_t j = 0; j < NS; j++) {
    cost[j] = emit_cost(0, j, onset_idx);
    s_bp[0][j] = j;
  }
  for (uint16_t t = 1; t < n; t++) {
    for (uint8_t j = 0; j < NS; j++) {
      int32_t best = INT32_MIN;
      uint8_t arg = 0;
      for (uint8_t i = 0; i < NS; i++) {
        int32_t c = cost[i] + TRANS[i][j];
        if (c > best) { best = c; arg = i; }
      }
      ncost[j] = best + emit_cost(t, j, onset_idx);
      s_bp[t][j] = arg;
    }
    for (uint8_t j = 0; j < NS; j++) cost[j] = ncost[j];
  }
  // Terminate: pick best final state, backtrace.
  int32_t best = INT32_MIN; uint8_t arg = 0;
  for (uint8_t j = 0; j < NS; j++) if (cost[j] > best) { best = cost[j]; arg = j; }
  s_path[n - 1] = arg;
  for (int t = (int)n - 2; t >= 0; t--) {
    arg = s_bp[t + 1][arg];
    s_path[t] = arg;
  }
}

static uint16_t min_ep_for(uint8_t ix) {
  switch (ix) {
    case S_RM: return MIN_EP_REM;
    case S_AW: return MIN_EP_AWAKE;
    default:   return MIN_EP_LIGHT;
  }
}

// One min-episode pass over s_path[0..n-1]. Returns true if it changed
// anything. Processes shortest sub-minimum episode first (spec section 4).
static bool min_episode_pass(uint16_t n) {
  // Find the shortest sub-minimum episode this pass.
  uint16_t best_len = 0xFFFF, best_start = 0, best_end = 0;
  bool found = false;
  uint16_t i = 0;
  while (i < n) {
    uint16_t j = i;
    while (j + 1 < n && s_path[j + 1] == s_path[i]) j++;
    uint16_t len = j - i + 1;
    if (len < min_ep_for(s_path[i]) && len < best_len) {
      best_len = len; best_start = i; best_end = j; found = true;
    }
    i = j + 1;
  }
  if (!found) return false;
  // Determine absorbing neighbour: longer adjacent episode; tie -> following;
  // edge -> the one existing neighbour.
  int prev_len = -1, next_len = -1;
  uint8_t prev_state = 0, next_state = 0;
  if (best_start > 0) {
    uint16_t pe = best_start - 1, ps = pe;
    while (ps > 0 && s_path[ps - 1] == s_path[pe]) ps--;
    prev_len = pe - ps + 1; prev_state = s_path[pe];
  }
  if (best_end + 1 < n) {
    uint16_t ns_ = best_end + 1, nne = ns_;
    while (nne + 1 < n && s_path[nne + 1] == s_path[ns_]) nne++;
    next_len = nne - ns_ + 1; next_state = s_path[ns_];
  }
  uint8_t absorb;
  if (prev_len < 0)      absorb = next_state;
  else if (next_len < 0) absorb = prev_state;
  else if (next_len > prev_len) absorb = next_state;
  else if (prev_len > next_len) absorb = prev_state;
  else                   absorb = next_state; // tie -> following
  for (uint16_t t = best_start; t <= best_end; t++) s_path[t] = absorb;
  return true;
}

void smoother_run(uint16_t out_mins[4]) {
  out_mins[StageAwake] = 0;
  out_mins[StageLight] = 0;
  out_mins[StageREM]   = 0;
  uint16_t n = storage_epoch_count();
  if (n == 0) return;
  if (n > MAX_EPOCHS) n = MAX_EPOCHS; // degrade gracefully past the cap

  // Load raw observed states.
  for (uint16_t t = 0; t < n; t++) {
    EpochRecord rec;
    if (!storage_epoch_read(t, &rec)) { s_obs[t] = S_LT; continue; }
    s_obs[t] = stage_to_idx(rec.stage);
  }

  int onset_idx = find_onset(n);
  viterbi(n, onset_idx);

  for (uint8_t pass = 0; pass < MIN_EP_PASSES; pass++) {
    if (!min_episode_pass(n)) break;
  }

  // Write smoothed stages back; preserve raw stage in reserved.
  for (uint16_t t = 0; t < n; t++) {
    EpochRecord rec;
    if (!storage_epoch_read(t, &rec)) continue;
    rec.reserved = rec.stage;               // keep raw classifier stage
    rec.stage = idx_to_stage(s_path[t]);    // smoothed
    storage_epoch_update(t, &rec);
    out_mins[rec.stage]++;
  }
}
