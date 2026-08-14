#include <pebble.h>
#include "hrv_math.h"
#include "storage.h"
#include "sleep_stage.h"
#include "smoother.h"

static Window *s_window;
static Layer *s_canvas;
static bool s_recording = false;
static uint32_t s_hr_events = 0;
static uint32_t s_hrv_events = 0;
// Movement corroboration: accel_service_peek only, never subscribed.
// mag2 in milli-g squared; 1g rest == 1000000. Display-only for now.
static uint32_t s_mv_samples = 0;
static uint32_t s_mv_moved = 0;
static uint16_t s_mv_min_samples = 0;
static uint16_t s_mv_min_moved = 0;
#define MV_MOVED_PCT 10
static void prv_accel_peek(void) {
  AccelData d;
  if (accel_service_peek(&d) != 0) return;
  int32_t mag2 = (int32_t)d.x * d.x + (int32_t)d.y * d.y + (int32_t)d.z * d.z;
  s_mv_samples++;
  s_mv_min_samples++;
  if (mag2 < 722500 || mag2 > 1322500) {
    s_mv_moved++;
    s_mv_min_moved++;
  }
}
static uint16_t s_last_ppi = 0;
static uint16_t s_last_hr = 0;
// classifier-spec-v3 s3.2: s_last_hr is never cleared (assigned only when
// hr > 0), so a presence test passes forever on a stale value. FROZEN at 180 s
// on a measured HR event rate of ~59.7/min. Gates ns.mean_hr and nothing else.
#define HR_STALE_SEC 180
static uint32_t s_last_hr_time = 0;
static HrvBuffer s_live_buf;
static HrvBuffer s_minute_buf;
static HrvBuffer s_night_buf;
#define SLEEP_ONSET_MINUTES 5
static uint16_t s_sleep_streak;
static uint32_t s_onset_mark;
static bool s_onset_marked;
// classifier-spec-v3 s3.5/s4.2. A_MIN_MINUTES carried from the existing
// s_night_hr_count >= 20. AW_MOVED_MIN is an UNWEIGHTED 3-of-5 majority.
#define A_MIN_MINUTES 20
#define AW_MOVED_MIN 3
// s_onset_mark is s_night_buf.total_accepted -- a BEAT count. s3.5 needs the
// EPOCH index at onset, which no existing static carries. -1 until onset marks.
static int32_t s_onset_epoch_idx = -1;
static time_t s_session_start = 0;
static uint32_t s_night_baseline_var = 0;
// classifier-spec-v3 s6.1 limb 1 and s6.3: A_H and A_D are stack locals in
// prv_stop_recording with no persist site and no render site, so neither was
// checkable before this commit. RAM-only mirrors for the DIAG readout. s_anchor_hr 0 means
// UNDEFINED (prv_compute_anchor zeroes it when no minute qualifies), so DIAG
// prints -- for 0 on that line. Not persisted, not in NightSummary.
static uint16_t s_anchor_hr = 0;
static uint16_t s_anchor_hr_n = 0;
static uint32_t s_anchor_d = 0;
// classifier-spec-v2 s4: among minutes that clear T1 (v > 2*A_D), which of the
// other two terms vetoed. A complete partition of the T1-admitted set AS
// prv_base_redecide SEES IT -- NOT guaranteed to sum to Gate, because prv_measure
// re-reads rec.stage after the re-decision rewrote it and the two Awake skips
// therefore filter different sets. P-SUB measures Gate minus RemN but CANNOT say
// which term subtracted; no instrument separated them before this. RAM-only.
static uint16_t s_veto_t2 = 0;    // cleared T1, failed T2 only
static uint16_t s_veto_t3 = 0;    // cleared T1, failed T3 only
static uint16_t s_veto_both = 0;  // cleared T1, failed both
static uint16_t s_veto_none = 0;  // cleared T1, passed both -- became REM
#define BASE_SAMPLE_MAX 160   // safety bound, not a modelling parameter (base-spec-v1 s3.1)
#define EPOCH_VAR_MAX 960     // 16 h of per-minute variance, RAM only (base-spec-v1 s4)
#define BASE_SAMPLE_MIN 3
static uint32_t s_base_samples[BASE_SAMPLE_MAX];
static uint8_t s_base_sample_count = 0;
static uint32_t s_base_next_mark = 0;
static uint32_t s_epoch_var[EPOCH_VAR_MAX];
static uint16_t s_epoch_var_count = 0;
// classifier-spec-v2 s3.5 as corrected: S(m), RAM-only stillness bitmap,
// index-parallel to s_epoch_var. EpochRecord.reserved is NOT touched -- it
// holds the pre-smoother stage that RUNS reads.
static uint8_t s_epoch_still[(EPOCH_VAR_MAX + 7) / 8];
// classifier-spec-v3 s3.1: movement is THREE-VALUED. s_epoch_still carries
// !movement exactly as 2401455 s3 defines it and is NOT redefined here; this
// second RAM-only bitmap carries whether the minute had ANY accel sample.
// STILL = still && known.  UNKNOWN = still && !known.  UNKNOWN is NEVER STILL.
static uint8_t s_epoch_mv_known[(EPOCH_VAR_MAX + 7) / 8];
static uint32_t s_stop_night_var = 0;
static uint8_t s_batt_start = 0;
static uint8_t s_batt_end = 0;
static uint32_t s_night_hr_sum = 0;
static uint16_t s_night_hr_count = 0;
static uint16_t s_mins[4] = {0, 0, 0, 0};
static uint8_t s_awake_streak = 0;
static SleepStage s_last_stage = StageLight;
static AppTimer *s_ui_timer = NULL;
typedef enum { MODE_IDLE, MODE_RECORDING, MODE_RESULTS, MODE_HYPNO, MODE_HISTORY, MODE_DIAG, MODE_RUNS } ScreenMode;
static ScreenMode s_mode = MODE_IDLE;
static uint8_t s_hist_idx = 0;      // 0 = newest
static uint8_t s_hist_count = 0;    // populated nights at entry
static bool s_hist_ok = false;      // last storage_night_read result
static NightSummary s_hist_ns;      // currently loaded night
static uint8_t s_hist_slot[MAX_NIGHTS]; // list-position -> physical idx-from-newest (readable only)
static time_t s_session_end = 0;
static void prv_click_config(void *ctx);
#define AWAKE_DEBOUNCE 3

// Firmware HRV sample period persists across app runs and prv_deinit sets
// it to 0, so never assume it is on at launch. prv_init forces the write.
static bool s_hrv_on = false;
static void prv_set_hrv(bool on) {
  if (on == s_hrv_on) return;
  s_hrv_on = on;
  health_service_set_hrv_sample_period(on ? 1 : 0);
}

static uint32_t prv_base_median(void) {
  if (s_base_sample_count == 0) return 0;
  static uint32_t tmp[BASE_SAMPLE_MAX];
  for (uint16_t i = 0; i < s_base_sample_count; i++) tmp[i] = s_base_samples[i];
  for (uint16_t i = 1; i < s_base_sample_count; i++) {
    uint32_t key = tmp[i];
    int16_t j = (int16_t)i - 1;
    while (j >= 0 && tmp[j] > key) { tmp[j + 1] = tmp[j]; j--; }
    tmp[j + 1] = key;
  }
  return tmp[(s_base_sample_count - 1) / 2];
}

static void prv_close_minute(void) {
  if (!s_recording) return;
  // classifier-spec-v3 s3.2/s3.3: accumulate ONLY when fresh. s_last_hr is
  // never cleared, so a bare > 0 test passes forever on a stale value.
  {
    uint32_t now = (uint32_t)time(NULL);
    bool hr_fresh = (s_last_hr > 0) && (s_last_hr_time > 0) &&
                    ((now - s_last_hr_time) <= HR_STALE_SEC);
    if (hr_fresh) {
      s_night_hr_sum += s_last_hr;
      s_night_hr_count++;
    }
  }
  EpochRecord rec;
  uint16_t total = s_minute_buf.count + (uint16_t)s_minute_buf.rejected;
  rec.mean_ppi = hrv_mean_ppi(&s_minute_buf);
  rec.rmssd = hrv_rmssd(&s_minute_buf);
  rec.beat_count = (s_minute_buf.count > 255) ? 255 : (uint8_t)s_minute_buf.count;
  rec.quality_pct = (total > 0) ? (uint8_t)((s_minute_buf.count * 100) / total) : 0;
  rec.reserved = 0;
  // classifier-spec-v3 s3.1: three-valued. MOVED and STILL both require a
  // sample; no samples is UNKNOWN, which is NEVER folded into STILL.
  bool mv_known = (s_mv_min_samples > 0);
  bool movement = mv_known &&
    ((uint32_t)s_mv_min_moved * 100 >=
     (uint32_t)s_mv_min_samples * MV_MOVED_PCT);
  MovementState mv = !mv_known ? MV_UNKNOWN : (movement ? MV_MOVED : MV_STILL);
  SleepStage st = sleep_stage_classify(&s_minute_buf, s_night_baseline_var,
                                       mv);
  s_mv_min_samples = 0;
  s_mv_min_moved = 0;
  if (st == StageAwake) {
    s_awake_streak++;
    if (s_awake_streak < AWAKE_DEBOUNCE) st = s_last_stage;
  } else {
    s_awake_streak = 0;
  }
  s_last_stage = st;
  // classifier-spec-v3 s3.4: onset is IMMOBILITY-based, on accelerometer data
  // ALONE -- it reads no stage label, no mask, no HR, no HRV, no stored night.
  // MOVED resets the streak and UNKNOWN resets it too: a run interrupted by
  // minutes carrying no evidence is not five consecutive immobile minutes.
  if (mv != MV_STILL) {
    s_sleep_streak = 0;
  } else if (!s_onset_marked && ++s_sleep_streak >= SLEEP_ONSET_MINUTES) {
    s_onset_mark = s_night_buf.total_accepted;
    // s3.5 needs the EPOCH index at onset. s_onset_mark is a BEAT count and
    // cannot serve. This minute's epoch index is the pre-increment count.
    s_onset_epoch_idx = (int32_t)s_epoch_var_count;
    s_onset_marked = true;
    s_night_hr_sum = 0;
    s_night_hr_count = 0;
  }
  rec.stage = (uint8_t)st;
  s_mins[st]++;
  storage_epoch_write(&rec);
  // base-spec-v1 s4: RAM-only per-minute variance for the stop-time
  // re-decision pass. Captured before the minute buffer is reset.
  if (s_epoch_var_count < EPOCH_VAR_MAX) {
    // classifier-spec-v1 s3.1: D(m), robust per-minute dispersion. Replaces
    // hrv_ppi_variance HERE ONLY. The s_night_buf call below is BASE and is
    // unchanged (base-spec-v1, classifier-spec s2 as corrected).
    // classifier-spec-v2 s3.5: set the stillness bit for THIS index before
    // the count advances. Cleared for the whole array at session start.
    if (!movement) {
      s_epoch_still[s_epoch_var_count >> 3] |=
        (uint8_t)(1 << (s_epoch_var_count & 7));
    }
    // classifier-spec-v3 s3.1: the parallel known-bit. s_epoch_still is
    // UNCHANGED in meaning (2401455 s3); STILL = still && known at read time.
    if (mv_known) {
      s_epoch_mv_known[s_epoch_var_count >> 3] |=
        (uint8_t)(1 << (s_epoch_var_count & 7));
    }
    s_epoch_var[s_epoch_var_count++] = hrv_mad2(&s_minute_buf);
  }
  if (s_onset_marked && s_base_sample_count < BASE_SAMPLE_MAX) {
    if (s_base_next_mark == 0) {
      s_base_next_mark = s_onset_mark + HRV_BUF_MAX;
    }
    if (s_night_buf.total_accepted >= s_base_next_mark) {
      s_base_samples[s_base_sample_count++] = hrv_ppi_variance(&s_night_buf);
      s_base_next_mark += HRV_BUF_MAX;
      if (s_base_sample_count >= BASE_SAMPLE_MIN) {
        s_night_baseline_var = prv_base_median();
      } else if (s_base_sample_count == 1) {
        s_night_baseline_var = s_base_samples[0];
      }
    }
  }
  hrv_buf_reset(&s_minute_buf);
}

static void prv_tick_handler(struct tm *tick_time, TimeUnits changed) {
  prv_close_minute();
  if (s_recording) prv_set_hrv(true);
  layer_mark_dirty(s_canvas);
}

static void prv_health_handler(HealthEventType event, void *context) {
  prv_accel_peek();
  if (event == HealthEventHeartRateUpdate) {
    s_hr_events++;
    HealthValue hr = health_service_peek_current_value(HealthMetricHeartRateRawBPM);
    // classifier-spec-v3 s3.2: stamp the assignment so freshness is testable
    // at minute close. Same branch, same condition -- never assigned apart.
    if (hr > 0) {
      s_last_hr = (uint16_t)hr;
      s_last_hr_time = (uint32_t)time(NULL);
    }
  } else if ((int)event == 5) {
    s_hrv_events++;
    uint16_t ppi = (uint16_t)health_service_peek_hrv_ppi_ms();
    if (ppi > 0) {
      s_last_ppi = ppi;
      uint32_t now = (uint32_t)time(NULL);
      hrv_buf_add(&s_live_buf, ppi, 1, now);
      if (s_recording) {
        hrv_buf_add(&s_minute_buf, ppi, 1, now);
        hrv_buf_add(&s_night_buf, ppi, 1, now);
      }
    }
  }
  if (s_mode == MODE_IDLE) layer_mark_dirty(s_canvas);
}

static void prv_ui_tick(void *data) {
  s_ui_timer = NULL;
  if (!s_recording) return;
  layer_mark_dirty(s_canvas);
  s_ui_timer = app_timer_register(1000, prv_ui_tick, NULL);
}

static void prv_start_recording(void) {
  s_recording = true;
  prv_set_hrv(true);
  s_session_start = time(NULL);
  s_night_baseline_var = 0;
  s_epoch_var_count = 0;
  memset(s_epoch_still, 0, sizeof(s_epoch_still));   // classifier-spec-v2 s3.5
  memset(s_epoch_mv_known, 0, sizeof(s_epoch_mv_known)); // c-spec-v3 s3.1
  s_veto_t2 = s_veto_t3 = s_veto_both = s_veto_none = 0;
  s_base_sample_count = 0;
  s_base_next_mark = 0;
  s_stop_night_var = 0;
  s_batt_start = battery_state_service_peek().charge_percent;
  s_batt_end = 0;
  s_night_hr_sum = 0;
  s_night_hr_count = 0;
  s_last_hr_time = 0;                                // c-spec-v3 s3.2
  s_mv_min_samples = 0;
  s_mv_min_moved = 0;
  s_awake_streak = 0;
  s_last_stage = StageLight;
  for (int i = 0; i < 4; i++) s_mins[i] = 0;
  hrv_buf_reset(&s_minute_buf);
  hrv_buf_reset(&s_night_buf);
  s_sleep_streak = 0;
  s_onset_mark = 0;
  s_onset_marked = false;
  s_onset_epoch_idx = -1;                            // c-spec-v3 s3.5
  storage_session_start();
  s_mode = MODE_RECORDING;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}

// measurement-spec-v1 s3.2: statistics over the per-minute variance
// population and the BASE sample distribution. READ-ONLY with respect to
// staging - writes no EpochRecord, touches no s_mins element, changes no
// stage. s3.4 requires it to run AFTER prv_base_redecide because it sorts
// s_epoch_var in place.
static uint32_t s_v_max, s_v_p90, s_v_median, s_base_min, s_base_max;
static uint16_t s_v_count, s_v_over_gate;

// classifier-spec-v1 s3.2: F(m), the median of D over the centred five-minute
// window m-2..m+2, truncated at the night's edges, missing (zero) values
// excluded. Held in its own array because s_epoch_var must stay un-sorted and
// epoch-indexed for the whole re-decision pass (scope-correction s4).
static uint32_t s_epoch_f[EPOCH_VAR_MAX];

// Sort scratch for the anchor median. Separate array so s_epoch_f keeps its
// epoch indexing for the re-decision pass that follows.
static uint32_t s_anchor_scratch[EPOCH_VAR_MAX];
// classifier-spec-v2 s3.3: HF(m), the windowed per-minute heart rate.
// uint16_t is sufficient for bpm and halves the BSS cost (s5.4).
static uint16_t s_epoch_hf[EPOCH_VAR_MAX];

static void prv_measure(uint32_t base_final) {
  s_v_max = 0; s_v_p90 = 0; s_v_median = 0;
  s_base_min = 0; s_base_max = 0;
  s_v_count = 0; s_v_over_gate = 0;

  // measurement-spec-v1 feature correction s2: the gate counts the minutes the
  // classifier decided REM, so it tests F(m) > 2*A over EPOCH-INDEXED s_epoch_f
  // and applies the same skips prv_base_redecide applies. This MUST run before
  // the compaction below, which destroys epoch indexing.
  if (base_final > 0) {
    uint32_t gate = base_final * 2;
    uint16_t en = storage_epoch_count();
    if (en > s_epoch_var_count) en = s_epoch_var_count;
    for (uint16_t i = 0; i < en; i++) {
      EpochRecord rec;
      if (!storage_epoch_read(i, &rec)) continue;
      if (rec.stage == (uint8_t)StageAwake) continue;
      if (rec.beat_count < 20) continue;
      if (s_epoch_f[i] == 0) continue;
      if (s_epoch_f[i] > gate) s_v_over_gate++;
    }
  }
  // s3.1: qualifying population is v > 0, compacted to the front in place.
  uint16_t n = 0;
  for (uint16_t i = 0; i < s_epoch_var_count; i++) {
    if (s_epoch_var[i] > 0) s_epoch_var[n++] = s_epoch_var[i];
  }
  s_v_count = n;
  if (n == 0) return;

  // s3.3: in-place insertion sort, ascending. Integer only.
  for (uint16_t i = 1; i < n; i++) {
    uint32_t key = s_epoch_var[i];
    int32_t j = (int32_t)i - 1;
    while (j >= 0 && s_epoch_var[j] > key) {
      s_epoch_var[j + 1] = s_epoch_var[j];
      j--;
    }
    s_epoch_var[j + 1] = key;
  }

  s_v_max = s_epoch_var[n - 1];
  s_v_median = s_epoch_var[n / 2];          // s3.2: upper-middle, no averaging
  uint32_t pi = ((uint32_t)n * 9) / 10;
  if (pi > (uint32_t)(n - 1)) pi = n - 1;   // s3.2: clamped
  s_v_p90 = s_epoch_var[pi];


  // s3.2: BASE sample extremes by linear scan; no sort needed.
  if (s_base_sample_count > 0) {
    s_base_min = s_base_samples[0];
    s_base_max = s_base_samples[0];
    for (uint16_t i = 1; i < s_base_sample_count; i++) {
      uint32_t v = s_base_samples[i];
      if (v < s_base_min) s_base_min = v;
      if (v > s_base_max) s_base_max = v;
    }
  }
}

// base-spec-v1 s3.4: re-decide Light vs REM against the whole-night BASE.
static uint32_t prv_window_median(uint16_t m, uint16_t n) {
  uint32_t w[5];
  uint16_t k = 0;
  uint16_t lo = (m >= 2) ? (m - 2) : 0;
  uint16_t hi = (m + 2 < n) ? (m + 2) : (n - 1);
  for (uint16_t i = lo; i <= hi; i++) {
    if (s_epoch_var[i] == 0) continue;        // s3.1: missing, excluded
    uint32_t key = s_epoch_var[i];
    uint16_t j = k;
    while (j > 0 && w[j - 1] > key) { w[j] = w[j - 1]; j--; }
    w[j] = key;
    k++;
  }
  if (k == 0) return 0;
  return w[k / 2];                            // upper-middle, no averaging
}

// classifier-spec-v2 s3.2/s3.3: HF(m) = median of H over the centred window
// m-2 .. m+2, missing excluded, truncated at edges. H(m) = 60000 / mean_ppi,
// read from EpochRecord at stop -- NOT s_last_hr, which is not persisted.
// Identical window shape to prv_window_median so both terms share support.
static uint16_t prv_window_median_hr(uint16_t m, uint16_t n) {
  uint16_t w[5];
  uint16_t k = 0;
  uint16_t lo = (m >= 2) ? (m - 2) : 0;
  uint16_t hi = (m + 2 < n) ? (m + 2) : (n - 1);
  for (uint16_t i = lo; i <= hi; i++) {
    EpochRecord rec;
    if (!storage_epoch_read(i, &rec)) continue;
    if (rec.mean_ppi == 0) continue;                 // s3.2: missing, excluded
    uint16_t key = (uint16_t)(60000UL / rec.mean_ppi);
    if (key == 0) continue;
    uint16_t j = k;
    while (j > 0 && w[j - 1] > key) { w[j] = w[j - 1]; j--; }
    w[j] = key;
    k++;
  }
  if (k == 0) return 0;
  return w[k / 2];                            // upper-middle, no averaging
}
// classifier-spec-v3 s3.5/s3.6/s4.2: the Awake re-decision pass. Runs BEFORE
// prv_compute_anchor (s5 step 3 before step 4) -- that single ordering is the
// whole mechanism, so anchors are computed over a CORRECTED label set.
// This is the ONLY pass that writes StageAwake.
static void prv_awake_redecide(void) {
  uint16_t n = storage_epoch_count();
  if (n > s_epoch_var_count) n = s_epoch_var_count;
  if (n == 0) return;

  // s3.3: HF(m), same window shape as F(m). prv_compute_anchor refills
  // s_epoch_hf identically before it uses it, so filling it here is safe.
  for (uint16_t i = 0; i < n; i++) s_epoch_hf[i] = prv_window_median_hr(i, n);

  // s3.5: A = median of HF over minutes at or after the ONSET index with HF
  // defined and movement STILL. STILL = still && known -- UNKNOWN is never
  // STILL (s3.1). A filters on accelerometer evidence, NOT on Awake labels,
  // which is what keeps it non-circular with the decision it feeds.
  uint32_t a_hr = 0;
  if (s_onset_epoch_idx >= 0) {
    uint16_t k = 0;
    for (uint16_t i = (uint16_t)s_onset_epoch_idx; i < n; i++) {
      bool still = (s_epoch_still[i >> 3] & (uint8_t)(1 << (i & 7))) != 0;
      bool known = (s_epoch_mv_known[i >> 3] & (uint8_t)(1 << (i & 7))) != 0;
      if (!(still && known)) continue;
      if (s_epoch_hf[i] == 0) continue;
      s_anchor_scratch[k++] = (uint32_t)s_epoch_hf[i];
    }
    // s3.5: fewer than A_MIN_MINUTES qualifying minutes leaves A undefined.
    if (k >= A_MIN_MINUTES) {
      for (uint16_t i = 1; i < k; i++) {
        uint32_t key = s_anchor_scratch[i];
        uint16_t j = i;
        while (j > 0 && s_anchor_scratch[j - 1] > key) {
          s_anchor_scratch[j] = s_anchor_scratch[j - 1];
          j--;
        }
        s_anchor_scratch[j] = key;
      }
      a_hr = s_anchor_scratch[k / 2];        // upper-middle, no averaging
    }
  }

  for (uint16_t i = 0; i < n; i++) {
    EpochRecord rec;
    if (!storage_epoch_read(i, &rec)) continue;

    // s3.6: AW(m) over the centred window i-2 .. i+2, truncated at edges,
    // missing excluded. Same window shape as F(m) and HF(m), so no term in
    // the project fires on a single isolated minute.
    uint16_t lo = (i >= 2) ? (i - 2) : 0;
    uint16_t hi = (i + 2 < n) ? (i + 2) : (n - 1);
    uint16_t moved = 0;
    for (uint16_t w = lo; w <= hi; w++) {
      bool still = (s_epoch_still[w >> 3] & (uint8_t)(1 << (w & 7))) != 0;
      bool known = (s_epoch_mv_known[w >> 3] & (uint8_t)(1 << (w & 7))) != 0;
      if (known && !still) moved++;          // MOVED only; UNKNOWN is not MOVED
    }
    // s4.2 clause 1: unweighted majority, AW_MOVED_MIN of 5.
    bool c1 = (moved >= AW_MOVED_MIN);
    // s4.2 clause 2: A defined, HF defined, HF(m) * 100 > A * 103. The 103 is
    // carried from sleep_stage.c's 97; its REFERENCE is not carried. When A is
    // undefined no minute is scored Awake by the HR term -- a missing baseline
    // must never make Awake EASIER to declare.
    bool c2 = (a_hr > 0) && (s_epoch_hf[i] > 0) &&
              ((uint32_t)s_epoch_hf[i] * 100 > a_hr * 103);

    SleepStage ns_stage = (c1 || c2) ? StageAwake : StageLight;
    // Clearing goes to Light so the minute becomes eligible for step 5's
    // Light/REM decision, which skips StageAwake (bae23c3 s2, unchanged).
    if (!(c1 || c2) && rec.stage != (uint8_t)StageAwake) continue;
    if ((uint8_t)ns_stage == rec.stage) continue;
    if (s_mins[rec.stage] > 0) s_mins[rec.stage]--;
    s_mins[ns_stage]++;
    rec.stage = (uint8_t)ns_stage;
    storage_epoch_update(i, &rec);
  }
}

// classifier-spec-v1 s3.3: A, the median of F over all non-Awake minutes with
// F defined. Fills s_epoch_f as it goes. Returns 0 if no minute qualifies.
static uint32_t prv_compute_anchor(uint16_t *out_anchor_hr, uint16_t *out_hk) {
  uint16_t n = storage_epoch_count();
  if (n > s_epoch_var_count) n = s_epoch_var_count;
  if (out_anchor_hr) *out_anchor_hr = 0;
  if (out_hk) *out_hk = 0;
  if (n == 0) return 0;
  for (uint16_t i = 0; i < n; i++) s_epoch_f[i] = prv_window_median(i, n);
  // classifier-spec-v2 s3.3: fill HF alongside F, same pass, same window.
  for (uint16_t i = 0; i < n; i++) s_epoch_hf[i] = prv_window_median_hr(i, n);
  // classifier-spec-v2 s3.4: A_H = median of HF over non-Awake minutes with
  // HF defined. Computed FIRST, before A_D reuses s_anchor_scratch below.
  if (out_anchor_hr) {
    uint16_t hk = 0;
    for (uint16_t i = 0; i < n; i++) {
      EpochRecord rec;
      if (!storage_epoch_read(i, &rec)) continue;
      if (rec.stage == (uint8_t)StageAwake) continue;
      if (s_epoch_hf[i] == 0) continue;
      s_anchor_scratch[hk++] = (uint32_t)s_epoch_hf[i];
    }
    if (hk > 0) {
      for (uint16_t i = 1; i < hk; i++) {
        uint32_t key = s_anchor_scratch[i];
        uint16_t j = i;
        while (j > 0 && s_anchor_scratch[j - 1] > key) {
          s_anchor_scratch[j] = s_anchor_scratch[j - 1];
          j--;
        }
        s_anchor_scratch[j] = key;
      }
      *out_anchor_hr = (uint16_t)s_anchor_scratch[hk / 2];
      if (out_hk) *out_hk = hk;
    }
  }
  // Collect the qualifying F values into the scratch array, then sort it.
  uint16_t k = 0;
  for (uint16_t i = 0; i < n; i++) {
    EpochRecord rec;
    if (!storage_epoch_read(i, &rec)) continue;
    if (rec.stage == (uint8_t)StageAwake) continue;
    if (s_epoch_f[i] == 0) continue;
    s_anchor_scratch[k++] = s_epoch_f[i];
  }
  if (k == 0) return 0;
  for (uint16_t i = 1; i < k; i++) {
    uint32_t key = s_anchor_scratch[i];
    uint16_t j = i;
    while (j > 0 && s_anchor_scratch[j - 1] > key) {
      s_anchor_scratch[j] = s_anchor_scratch[j - 1];
      j--;
    }
    s_anchor_scratch[j] = key;
  }
  return s_anchor_scratch[k / 2];             // upper-middle, no averaging
}

// Awake is untouched (s2) and runs before smoother_run (s3.5).
static void prv_base_redecide(uint32_t anchor, uint16_t anchor_hr) {
  if (anchor == 0) return;
  uint16_t n = storage_epoch_count();
  if (n > s_epoch_var_count) n = s_epoch_var_count;
  for (uint16_t i = 0; i < n; i++) {
    EpochRecord rec;
    if (!storage_epoch_read(i, &rec)) continue;
    if (rec.stage == (uint8_t)StageAwake) continue;   // s3.4 step 2
    if (rec.beat_count < 20) continue;                // s3.4 step 3
    uint32_t v = s_epoch_f[i];                        // classifier-spec s3.2
    if (v == 0) continue;                             // s3.4 step 4
    SleepStage ns_stage;
    // classifier-spec-v1 s3.4. Multipliers unchanged; the anchor is A, not
    // BASE (D1, and scope-correction s2).
    // classifier-spec-v2 s4: REM is the conjunction of all three terms.
    // T2 is a median split on the night's own HF distribution; T3 is the
    // atonia veto, read from the RAM-only bitmap (s3.5 as corrected).
    bool t2 = (anchor_hr > 0) && (s_epoch_hf[i] > anchor_hr);
    // classifier-spec-v3 s3.1: UNKNOWN is NEVER treated as STILL. s_epoch_still
    // is set whenever !movement, which includes minutes with NO accel samples
    // (prv_accel_peek runs only inside prv_health_handler), so the known-bit is
    // required at read time -- the contract main.c 188-189 states and which the
    // Awake path already honours at 429-430 and 461-462. T3 read stillness alone
    // here, so a sensor gap passed the atonia veto. classifier-spec-v2 s4.3's
    // intact-atonia assumption is unchanged; this makes the veto mean what it says.
    bool t3 = ((s_epoch_still[i >> 3] & (uint8_t)(1 << (i & 7))) != 0)
           && ((s_epoch_mv_known[i >> 3] & (uint8_t)(1 << (i & 7))) != 0);
    if (v * 2 >= anchor && v <= anchor * 2) {
      ns_stage = StageLight;
    } else if (v > anchor * 2 && t2 && t3) {
      ns_stage = StageREM;
    } else {
      ns_stage = StageLight;
    }
    // Tally AFTER the decision, over T1-admitted minutes only. Counting, not deciding.
    if (v > anchor * 2) {
      if (t2 && t3)       s_veto_none++;
      else if (!t2 && !t3) s_veto_both++;
      else if (!t2)        s_veto_t2++;
      else                 s_veto_t3++;
    }
    if ((uint8_t)ns_stage == rec.stage) continue;
    if (s_mins[rec.stage] > 0) s_mins[rec.stage]--;
    s_mins[ns_stage]++;
    rec.stage = (uint8_t)ns_stage;
    storage_epoch_update(i, &rec);
  }
}

static void prv_stop_recording(void) {
  prv_close_minute();
  s_night_baseline_var = prv_base_median();   // s3.3 whole-night BASE
  // classifier-spec-v3 s5 step 3: MUST run before prv_compute_anchor so the
  // anchors are medians over a corrected label set. This is the whole point.
  prv_awake_redecide();
  uint16_t anchor_hr = 0;                      // classifier-spec-v2 s3.4
  uint16_t anchor_hk = 0;                      // count feeding the A_H median
  uint32_t anchor = prv_compute_anchor(&anchor_hr, &anchor_hk);
  // classifier-spec-v3 s6.1/s6.3 readout. Mirrors only -- no decision reads these.
  s_anchor_hr = anchor_hr;
  s_anchor_hr_n = anchor_hk;
  s_anchor_d = anchor;
  prv_base_redecide(anchor, anchor_hr);        // s3.4, before the smoother
  prv_measure(anchor);                         // measurement-spec correction s2
  smoother_run(s_mins);
  s_recording = false;
  s_session_end = time(NULL);
  NightSummary ns;
  ns.date = s_session_start;
  ns.rmssd = hrv_rmssd(&s_night_buf);
  ns.sdnn = hrv_sdnn(&s_night_buf);
  ns.mean_ppi = hrv_mean_ppi(&s_night_buf);
  ns.epoch_count = storage_epoch_count();
  int deep_sec = (int)health_service_sum(HealthMetricSleepRestfulSeconds,
                                         s_session_start, s_session_end);
  uint16_t deep_min = (deep_sec > 0) ? (uint16_t)(deep_sec / 60) : 0;
  s_mins[StageDeep] = deep_min;
  ns.mins_awake = s_mins[StageAwake];
  ns.mins_light = s_mins[StageLight];
  ns.mins_deep = s_mins[StageDeep];
  ns.mins_rem = s_mins[StageREM];
  ns.beats = s_night_buf.total_accepted;
  ns.rejected = s_night_buf.rejected;
  ns.rej_range = s_night_buf.rej_range;
  ns.rej_jump = s_night_buf.rej_jump;
  ns.start_time = s_session_start;
  ns.end_time = s_session_end;
  ns.mean_hr = (s_night_hr_count > 0)
    ? (uint16_t)(s_night_hr_sum / s_night_hr_count) : 0;
  ns.baseline_var = s_night_baseline_var;
  ns.night_var = hrv_ppi_variance(&s_night_buf);
  s_stop_night_var = ns.night_var;
  s_batt_end = battery_state_service_peek().charge_percent;
  ns.batt_start_pct = s_batt_start;
  ns.batt_end_pct = s_batt_end;
  ns.v_max = s_v_max;
  ns.v_p90 = s_v_p90;
  ns.v_median = s_v_median;
  ns.base_min = s_base_min;
  ns.base_max = s_base_max;
  ns.v_count = s_v_count;
  ns.v_over_gate_count = s_v_over_gate;
  if (ns.epoch_count >= 30) storage_night_save(&ns);
  s_mode = MODE_RESULTS;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}

static void prv_draw_idle(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_text_color(ctx, GColorBlack);
  char line[64];
  int y = 2;
  snprintf(line, sizeof(line), "Recovery idle");
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),
    GRect(4, y, b.size.w - 8, 30), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 30;
  if (s_recording) {
    uint32_t dur = (uint32_t)(time(NULL) - s_session_start);
    snprintf(line, sizeof(line), "Dur %lu:%02lu",
      (unsigned long)(dur/60), (unsigned long)(dur%60));
  } else {
    char tbuf[16];
    clock_copy_time_string(tbuf, sizeof(tbuf));
    snprintf(line, sizeof(line), "%s", tbuf);
  }
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "HR %u  PPI %u", s_last_hr, s_last_ppi);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "RMSSD %u", hrv_rmssd(&s_live_buf));
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "ev H%lu V%lu M%lu/%lu",
    (unsigned long)s_hr_events, (unsigned long)s_hrv_events,
    (unsigned long)s_mv_moved, (unsigned long)s_mv_samples);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_14),
    GRect(4, y, b.size.w - 8, 36), GTextOverflowModeWordWrap,
    GTextAlignmentLeft, NULL); y += 36;
  snprintf(line, sizeof(line), "A%u L%u D%u R%u", s_mins[0], s_mins[1],
    s_mins[2], s_mins[3]);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL);
}

static void prv_draw_recording(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_text_color(ctx, GColorBlack);
  char t[8];
  clock_copy_time_string(t, sizeof(t));
  graphics_draw_text(ctx, t, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS),
    GRect(0, 40, b.size.w, 48), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx, "Recording", fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, 100, b.size.w - 8, 28), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx, "Hold Select 1.5s to stop",
    fonts_get_system_font(FONT_KEY_GOTHIC_18),
    GRect(4, 132, b.size.w - 8, 40), GTextOverflowModeWordWrap,
    GTextAlignmentCenter, NULL);
}

static void prv_draw_results(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_text_color(ctx, GColorBlack);
  char line[64];
  int y = 2;
  graphics_draw_text(ctx, "Sleep Recorded",
    fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),
    GRect(4, y, b.size.w - 8, 30), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 32;
  uint32_t dur = (s_session_end > s_session_start)
    ? (uint32_t)(s_session_end - s_session_start) : 0;
  snprintf(line, sizeof(line), "Dur %luh %02lum",
    (unsigned long)(dur/3600), (unsigned long)((dur%3600)/60));
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "Awake %u  REM %u", s_mins[StageAwake], s_mins[StageREM]);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "Light %u  OSrest %u", s_mins[StageLight], s_mins[StageDeep]);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "RMSSD %u", hrv_rmssd(&s_night_buf));
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  snprintf(line, sizeof(line), "BASE %lu/n%u  NIGHT %lu",
    (unsigned long)s_night_baseline_var,
    (unsigned)s_base_sample_count,
    (unsigned long)s_stop_night_var);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_14),
    GRect(4, y, b.size.w - 8, 18), GTextOverflowModeWordWrap,
    GTextAlignmentLeft, NULL); y += 18;
  snprintf(line, sizeof(line), "Batt %u>%u", s_batt_start, s_batt_end);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_14),
    GRect(4, y, b.size.w - 8, 18), GTextOverflowModeWordWrap,
    GTextAlignmentLeft, NULL); y += 18;
  snprintf(line, sizeof(line), "B%lu rej%lu rng%lu jmp%lu",
    (unsigned long)s_night_buf.total_accepted,
    (unsigned long)s_night_buf.rejected,
    (unsigned long)s_night_buf.rej_range,
    (unsigned long)s_night_buf.rej_jump);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_14),
    GRect(4, y, b.size.w - 8, 36), GTextOverflowModeWordWrap,
    GTextAlignmentLeft, NULL); y += 36;
  graphics_draw_text(ctx, "Down: graph",
    fonts_get_system_font(FONT_KEY_GOTHIC_14),
    GRect(4, y, b.size.w - 8, 18), GTextOverflowModeWordWrap,
    GTextAlignmentLeft, NULL);
}

static GColor prv_stage_color(uint8_t stage) {
  switch (stage) {
    case StageAwake: return GColorRed;
    case StageREM:   return GColorPurple;
    case StageLight: return GColorCyan;
    case StageDeep:  return GColorDukeBlue;
    default:         return GColorLightGray;
  }
}

static void prv_draw_hypno(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_draw_text(ctx, "Sleep Stages",
    fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
    GRect(4, 0, b.size.w - 8, 22), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL);
  int top = 26;
  int bottom = b.size.h - 8;
  int gh = bottom - top;
  uint16_t n = storage_epoch_count();
  if (n == 0) return;
  int plot_w = b.size.w - 8;
  // depth fraction: Awake tallest -> Deep shortest
  for (uint16_t i = 0; i < n; i++) {
    EpochRecord rec;
    if (!storage_epoch_read(i, &rec)) continue;
    int h;
    switch (rec.stage) {
      case StageAwake: h = gh;           break;
      case StageREM:   h = (gh * 3) / 4; break;
      case StageLight: h = gh / 2;       break;
      case StageDeep:  h = gh / 4;       break;
      default:         h = gh / 8;       break;
    }
    int x0 = 4 + (int)((uint32_t)i * plot_w / n);
    int x1 = 4 + (int)((uint32_t)(i + 1) * plot_w / n);
    int w = (x1 > x0) ? (x1 - x0) : 1;
    graphics_context_set_fill_color(ctx, prv_stage_color(rec.stage));
    graphics_fill_rect(ctx, GRect(x0, bottom - h, w, h), 0, GCornerNone);
  }
}

static void prv_draw_history(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_text_color(ctx, GColorBlack);
  char line[64];
  int y = 2;

  if (s_hist_count == 0) {
    graphics_draw_text(ctx, "No nights",
      fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),
      GRect(4, y, b.size.w - 8, 30), GTextOverflowModeTrailingEllipsis,
      GTextAlignmentLeft, NULL);
    return;
  }

  snprintf(line, sizeof(line), "Night %u/%u",
    (unsigned)(s_hist_idx + 1), (unsigned)s_hist_count);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD),
    GRect(4, y, b.size.w - 8, 30), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 30;

  if (!s_hist_ok) {
    snprintf(line, sizeof(line), "v%u mismatch", (unsigned)s_hist_ns.version);
    graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
      GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
      GTextAlignmentLeft, NULL); y += 26;
    graphics_draw_text(ctx, "UP older  DOWN newer",
      fonts_get_system_font(FONT_KEY_GOTHIC_14),
      GRect(4, y, b.size.w - 8, 18), GTextOverflowModeWordWrap,
      GTextAlignmentLeft, NULL);
    return;
  }

  char dbuf[16], sbuf[16], ebuf[16];
  time_t t_date = s_hist_ns.date;
  time_t t_start = s_hist_ns.start_time;
  time_t t_end = s_hist_ns.end_time;
  strftime(dbuf, sizeof(dbuf), "%b %d", localtime(&t_date));
  strftime(sbuf, sizeof(sbuf), "%l:%M%P", localtime(&t_start));
  strftime(ebuf, sizeof(ebuf), "%l:%M%P", localtime(&t_end));
  snprintf(line, sizeof(line), "%s %s-%s", dbuf, sbuf, ebuf);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_18),
    GRect(4, y, b.size.w - 8, 22), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 22;

  snprintf(line, sizeof(line), "Awake %u  REM %u",
    s_hist_ns.mins_awake, s_hist_ns.mins_rem);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;

  snprintf(line, sizeof(line), "Light %u  OSrest %u",
    s_hist_ns.mins_light, s_hist_ns.mins_deep);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;

  snprintf(line, sizeof(line), "RMSSD %u  BASE %lu",
    s_hist_ns.rmssd, (unsigned long)s_hist_ns.baseline_var);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_14),
    GRect(4, y, b.size.w - 8, 18), GTextOverflowModeWordWrap,
    GTextAlignmentLeft, NULL); y += 18;

  snprintf(line, sizeof(line), "Batt %u>%u",
    s_hist_ns.batt_start_pct, s_hist_ns.batt_end_pct);
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_14),
    GRect(4, y, b.size.w - 8, 18), GTextOverflowModeWordWrap,
    GTextAlignmentLeft, NULL); y += 18;
  // nights-render-spec-v1 s4. s3.1: absent fields print -- , NEVER 0, because
  // a rendered 0 is indistinguishable from N16's genuine Gate 15 or N17's 77.
  bool has_v = (s_hist_ns.version >= 2);
  bool has_cs = (s_hist_ns.version >= 3);
  if (has_v) {
    snprintf(line, sizeof(line), "Vmax %lu  P90 %lu",
      (unsigned long)s_hist_ns.v_max, (unsigned long)s_hist_ns.v_p90);
  } else {
    snprintf(line, sizeof(line), "Vmax --  P90 --");
  }
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_14),
    GRect(4, y, b.size.w - 8, 18), GTextOverflowModeWordWrap,
    GTextAlignmentLeft, NULL); y += 18;
  if (has_v) {
    snprintf(line, sizeof(line), "Vmed %lu  Vn %u  Gate %u",
      (unsigned long)s_hist_ns.v_median, s_hist_ns.v_count,
      s_hist_ns.v_over_gate_count);
  } else {
    snprintf(line, sizeof(line), "Vmed --  Vn --  Gate --");
  }
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_14),
    GRect(4, y, b.size.w - 8, 18), GTextOverflowModeWordWrap,
    GTextAlignmentLeft, NULL); y += 18;
  // s3.2: the series byte is what makes the numbers above interpretable --
  // the v_* fields changed meaning at classifier_series 7 (443f499).
  if (has_v && has_cs) {
    snprintf(line, sizeof(line), "Bmin %lu  Bmax %lu  cs%u",
      (unsigned long)s_hist_ns.base_min, (unsigned long)s_hist_ns.base_max,
      s_hist_ns.classifier_series);
  } else if (has_v) {
    snprintf(line, sizeof(line), "Bmin %lu  Bmax %lu  cs--",
      (unsigned long)s_hist_ns.base_min, (unsigned long)s_hist_ns.base_max);
  } else {
    snprintf(line, sizeof(line), "Bmin --  Bmax --  cs--");
  }
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_14),
    GRect(4, y, b.size.w - 8, 18), GTextOverflowModeWordWrap,
    GTextAlignmentLeft, NULL); y += 18;

  graphics_draw_text(ctx, "UP older  DOWN newer",
    fonts_get_system_font(FONT_KEY_GOTHIC_14),
    GRect(4, y, b.size.w - 8, 18), GTextOverflowModeWordWrap,
    GTextAlignmentLeft, NULL);
}

// measurement-spec-v1 s3.6: DIAG is a diagnostic screen. RESULTS is
// unchanged. A v1 record has no measured tail, so it prints -- rather
// than 0 - zero is a meaningful measured value here.
static void prv_draw_diag(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_text_color(ctx, GColorBlack);
  char line[64];
  int y = 2;
  GFont f = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  snprintf(line, sizeof(line), "Diag");
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;
  if (s_v_count == 0) {
    snprintf(line, sizeof(line), "Vmax --  P90 --");
    graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;
    snprintf(line, sizeof(line), "Vmed --  Vn 0");
    graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;
    snprintf(line, sizeof(line), "Gate --");
    graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;
  } else {
    snprintf(line, sizeof(line), "Vmax %lu", (unsigned long)s_v_max);
    graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;
    snprintf(line, sizeof(line), "P90 %lu  Vmed %lu",
      (unsigned long)s_v_p90, (unsigned long)s_v_median);
    graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;
    snprintf(line, sizeof(line), "Vn %u  Gate %u",
      (unsigned)s_v_count, (unsigned)s_v_over_gate);
    graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;
  }
  snprintf(line, sizeof(line), "Bmin %lu  Bmax %lu",
    (unsigned long)s_base_min, (unsigned long)s_base_max);
  graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
    GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;
  snprintf(line, sizeof(line), "BASE %lu  x2 %lu",
    (unsigned long)s_night_baseline_var, (unsigned long)(s_night_baseline_var * 2));
  graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
    GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;
  // classifier-spec-v3 s6.1 limb 1 (A_H) and s6.3 (A_D). AH 0 is UNDEFINED,
  // not measured -- prv_compute_anchor zeroes it when no minute qualifies --
  // so it prints -- per this screen s convention. n is the count of minutes
  // feeding the A_H median (non-Awake, HF defined), which review finding 4
  // needs and which A_H alone cannot show.
  if (s_anchor_hr == 0) {
    snprintf(line, sizeof(line), "AH --  n %u", (unsigned)s_anchor_hr_n);
  } else {
    snprintf(line, sizeof(line), "AH %u  n %u",
      (unsigned)s_anchor_hr, (unsigned)s_anchor_hr_n);
  }
  graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
    GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;
  snprintf(line, sizeof(line), "AD %lu", (unsigned long)s_anchor_d);
  graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
    GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;
  // The T1-admitted partition as prv_base_redecide sees it. It is NOT guaranteed
  // to equal Gate: prv_measure re-reads rec.stage AFTER the re-decision rewrote
  // it, so the two Awake skips filter different sets. Close, not identical --
  // record the difference, do not assume zero.
  snprintf(line, sizeof(line), "xT2 %u xT3 %u xB %u",
    (unsigned)s_veto_t2, (unsigned)s_veto_t3, (unsigned)s_veto_both);
  graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
    GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;
  snprintf(line, sizeof(line), "pass %u", (unsigned)s_veto_none);
  graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
    GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);
}

// epoch-readout-spec-v1 s3: run-length statistics over the PRE-SMOOTHER
// stage held in EpochRecord.reserved (smoother.c line 215 writes it there
// before overwriting rec.stage). READ ONLY - storage_epoch_read only, never
// storage_epoch_update. No EpochRecord change, no new static array.
// s2 registered identity: rem_total MUST equal the night's v_over_gate_count.
typedef struct {
  uint16_t ep_n;
  uint16_t rem_total;
  uint16_t rem_runs;
  uint16_t rem_max;
  uint16_t r1, r2, r3, r4, r5p;
  int16_t first_off;
  bool has_off;
  // classifier-spec-v3 s3.4: the LABEL-derived onset Off is measured
  // against. Under v3 this is NOT the same quantity as the live
  // immobility onset s_onset_epoch_idx -- both are rendered so the
  // divergence is observable. -1 when no onset was found.
  int16_t onset_label;
} RunStats;

// Onset by the SAME ONSET_RUN 5 consecutive non-Awake rule find_onset uses
// (smoother.c lines 93-106), applied over reserved. -1 if none.
#define RUNS_ONSET_RUN 5

static void prv_compute_runs(RunStats *st) {
  memset(st, 0, sizeof(*st));
  st->first_off = 0;
  st->has_off = false;
  st->onset_label = -1;
  uint16_t n = storage_epoch_count();
  st->ep_n = n;
  if (n == 0) return;

  int onset_idx = -1;
  int first_rem = -1;
  uint16_t onset_run = 0;
  uint16_t cur_run = 0;

  for (uint16_t t = 0; t < n; t++) {
    EpochRecord rec;
    if (!storage_epoch_read(t, &rec)) { continue; }
    uint8_t st_pre = rec.reserved;

    if (onset_idx < 0) {
      if (st_pre != StageAwake) {
        onset_run++;
        if (onset_run >= RUNS_ONSET_RUN) {
          onset_idx = (int)t - (RUNS_ONSET_RUN - 1);
        }
      } else {
        onset_run = 0;
      }
    }

    if (st_pre == StageREM) {
      if (first_rem < 0) first_rem = (int)t;
      st->rem_total++;
      cur_run++;
    } else if (cur_run > 0) {
      st->rem_runs++;
      if (cur_run > st->rem_max) st->rem_max = cur_run;
      if (cur_run == 1) st->r1++;
      else if (cur_run == 2) st->r2++;
      else if (cur_run == 3) st->r3++;
      else if (cur_run == 4) st->r4++;
      else st->r5p++;
      cur_run = 0;
    }
  }
  if (cur_run > 0) {
    st->rem_runs++;
    if (cur_run > st->rem_max) st->rem_max = cur_run;
    if (cur_run == 1) st->r1++;
    else if (cur_run == 2) st->r2++;
    else if (cur_run == 3) st->r3++;
    else if (cur_run == 4) st->r4++;
    else st->r5p++;
  }

  if (onset_idx >= 0) st->onset_label = (int16_t)onset_idx;
  if (onset_idx >= 0 && first_rem >= 0) {
    st->first_off = (int16_t)(first_rem - onset_idx);
    st->has_off = true;
  }
}

// epoch-readout-spec-v1 s4. Diagnostic-only screen, queued item 9.
// An undefined value prints -- and NEVER 0.
static void prv_draw_runs(Layer *layer, GContext *ctx) {
  GRect b = layer_get_bounds(layer);
  graphics_context_set_text_color(ctx, GColorBlack);
  char line[64];
  int y = 2;
  GFont f = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  RunStats st;
  prv_compute_runs(&st);

  snprintf(line, sizeof(line), "Runs");
  graphics_draw_text(ctx, line, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(4, y, b.size.w - 8, 26), GTextOverflowModeTrailingEllipsis,
    GTextAlignmentLeft, NULL); y += 26;

  snprintf(line, sizeof(line), "RemN %u  Runs %u",
    (unsigned)st.rem_total, (unsigned)st.rem_runs);
  graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
    GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;

  snprintf(line, sizeof(line), "Max %u  Ep %u",
    (unsigned)st.rem_max, (unsigned)st.ep_n);
  graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
    GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;

  snprintf(line, sizeof(line), "L1 %u  L2 %u  L3 %u",
    (unsigned)st.r1, (unsigned)st.r2, (unsigned)st.r3);
  graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
    GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;

  snprintf(line, sizeof(line), "L4 %u  L5p %u",
    (unsigned)st.r4, (unsigned)st.r5p);
  graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
    GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;

  if (st.has_off) {
    snprintf(line, sizeof(line), "Off %d", (int)st.first_off);
  } else {
    snprintf(line, sizeof(line), "Off --");
  }
  graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
    GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL); y += 18;

  // classifier-spec-v3 s3.4. Ons  = live immobility onset epoch index.
  // OnsL = label-derived onset, the one Off is measured against. They were
  // the same quantity before v3. An undefined value prints -- and NEVER 0.
  {
    char a1[12], a2[12];
    if (s_onset_epoch_idx >= 0) snprintf(a1, sizeof(a1), "%d", (int)s_onset_epoch_idx);
    else snprintf(a1, sizeof(a1), "--");
    if (st.onset_label >= 0) snprintf(a2, sizeof(a2), "%d", (int)st.onset_label);
    else snprintf(a2, sizeof(a2), "--");
    snprintf(line, sizeof(line), "Ons %s  OnsL %s", a1, a2);
  }
  graphics_draw_text(ctx, line, f, GRect(4, y, b.size.w - 8, 18),
    GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);
}

static void prv_canvas_update(Layer *layer, GContext *ctx) {
  switch (s_mode) {
    case MODE_RECORDING: prv_draw_recording(layer, ctx); break;
    case MODE_RESULTS:   prv_draw_results(layer, ctx);   break;
    case MODE_HYPNO:     prv_draw_hypno(layer, ctx);     break;
    case MODE_DIAG:      prv_draw_diag(layer, ctx);      break;
    case MODE_RUNS:      prv_draw_runs(layer, ctx);      break;
    case MODE_HISTORY:   prv_draw_history(layer, ctx);   break;
    case MODE_IDLE:
    default:             prv_draw_idle(layer, ctx);      break;
  }
}


static void prv_noop(ClickRecognizerRef r, void *ctx) { }

static void prv_idle_exit(ClickRecognizerRef r, void *ctx) {
  window_stack_pop(true);
}
static void prv_start_long(ClickRecognizerRef r, void *ctx) {
  if (!s_recording) { prv_start_recording(); }
}
static void prv_stop_long(ClickRecognizerRef r, void *ctx) {
  if (s_recording) { prv_stop_recording(); }
}
static void prv_results_down(ClickRecognizerRef r, void *ctx) {
  s_mode = MODE_HYPNO;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}
static void prv_hypno_back_to_results(ClickRecognizerRef r, void *ctx) {
  s_mode = MODE_RESULTS;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}
static void prv_idle_to_results(ClickRecognizerRef r, void *ctx) {
  if (s_recording) return;
  s_mode = MODE_RESULTS;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}
static void prv_results_to_idle(ClickRecognizerRef r, void *ctx) {
  s_mode = MODE_IDLE;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}

static void prv_hist_load(void) {
  s_hist_ok = storage_night_read(s_hist_slot[s_hist_idx], &s_hist_ns);
}

static void prv_idle_to_history(ClickRecognizerRef r, void *ctx) {
  if (s_recording) return;
  uint8_t phys = storage_night_count();
  s_hist_count = 0;
  for (uint8_t i = 0; i < phys; i++) {
    NightSummary tmp;
    if (storage_night_read(i, &tmp)) {
      s_hist_slot[s_hist_count] = i;
      s_hist_count++;
    }
  }
  s_hist_idx = 0;
  s_hist_ok = false;
  if (s_hist_count > 0) prv_hist_load();
  s_mode = MODE_HISTORY;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}

static void prv_hist_older(ClickRecognizerRef r, void *ctx) {
  if (s_hist_count == 0) return;
  if (s_hist_idx + 1 < s_hist_count) { s_hist_idx++; prv_hist_load(); }
  layer_mark_dirty(s_canvas);
}

static void prv_hist_to_idle(ClickRecognizerRef r, void *ctx) {
  s_mode = MODE_IDLE;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}

static void prv_hist_newer(ClickRecognizerRef r, void *ctx) {
  if (s_hist_count == 0 || s_hist_idx == 0) { prv_hist_to_idle(r, ctx); return; }
  s_hist_idx--; prv_hist_load();
  layer_mark_dirty(s_canvas);
}


static void prv_hypno_to_diag(ClickRecognizerRef r, void *ctx) {
  s_mode = MODE_DIAG;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}
static void prv_diag_to_hypno(ClickRecognizerRef r, void *ctx) {
  s_mode = MODE_HYPNO;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}
static void prv_idle_to_runs(ClickRecognizerRef r, void *ctx) {
  s_mode = MODE_RUNS;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}
static void prv_runs_to_idle(ClickRecognizerRef r, void *ctx) {
  s_mode = MODE_IDLE;
  window_set_click_config_provider(s_window, prv_click_config);
  layer_mark_dirty(s_canvas);
}
static void prv_click_config(void *ctx) {
  switch (s_mode) {
    case MODE_IDLE:
      window_long_click_subscribe(BUTTON_ID_SELECT, 1500, prv_start_long, NULL);
      window_single_click_subscribe(BUTTON_ID_SELECT, prv_idle_to_results);
      window_single_click_subscribe(BUTTON_ID_UP, prv_idle_to_history);
      window_single_click_subscribe(BUTTON_ID_DOWN, prv_idle_to_runs);
      window_single_click_subscribe(BUTTON_ID_BACK, prv_idle_exit);
      break;
    case MODE_RECORDING:
      window_long_click_subscribe(BUTTON_ID_SELECT, 1500, prv_stop_long, NULL);
      window_single_click_subscribe(BUTTON_ID_BACK, prv_noop);
      window_long_click_subscribe(BUTTON_ID_BACK, 700, prv_noop, NULL);
      break;
    case MODE_RESULTS:
      window_single_click_subscribe(BUTTON_ID_DOWN, prv_results_down);
      window_single_click_subscribe(BUTTON_ID_BACK, prv_results_to_idle);
      break;
    case MODE_HYPNO:
      window_single_click_subscribe(BUTTON_ID_UP, prv_hypno_back_to_results);
      window_single_click_subscribe(BUTTON_ID_DOWN, prv_hypno_to_diag);
      window_single_click_subscribe(BUTTON_ID_BACK, prv_hypno_back_to_results);
      break;
    case MODE_DIAG:
      window_single_click_subscribe(BUTTON_ID_UP, prv_diag_to_hypno);
      window_single_click_subscribe(BUTTON_ID_BACK, prv_diag_to_hypno);
      break;
    case MODE_RUNS:
      window_single_click_subscribe(BUTTON_ID_UP, prv_runs_to_idle);
      window_single_click_subscribe(BUTTON_ID_BACK, prv_runs_to_idle);
      break;
    case MODE_HISTORY:
      window_single_click_subscribe(BUTTON_ID_UP, prv_hist_older);
      window_single_click_subscribe(BUTTON_ID_DOWN, prv_hist_newer);
      window_single_click_subscribe(BUTTON_ID_BACK, prv_hist_to_idle);
      break;
  }
}

static void prv_window_load(Window *w) {
  Layer *root = window_get_root_layer(w);
  GRect b = layer_get_bounds(root);
  s_canvas = layer_create(b);
  layer_set_update_proc(s_canvas, prv_canvas_update);
  layer_add_child(root, s_canvas);
}

static void prv_window_unload(Window *w) {
  layer_destroy(s_canvas);
}

static void prv_init(void) {
  s_window = window_create();
  window_set_click_config_provider(s_window, prv_click_config);
  window_set_window_handlers(s_window, (WindowHandlers){
    .load = prv_window_load, .unload = prv_window_unload });
  window_stack_push(s_window, true);
  hrv_buf_reset(&s_live_buf);
  hrv_buf_reset(&s_minute_buf);
  hrv_buf_reset(&s_night_buf);
  s_sleep_streak = 0;
  s_onset_mark = 0;
  s_onset_marked = false;
  s_onset_epoch_idx = -1;                            // c-spec-v3 s3.5
  s_base_sample_count = 0;
  s_base_next_mark = 0;
  health_service_events_subscribe(prv_health_handler, NULL);
  prv_set_hrv(true);
  tick_timer_service_subscribe(MINUTE_UNIT, prv_tick_handler);
}

static void prv_deinit(void) {
  if (s_recording) prv_stop_recording();
  prv_set_hrv(false);
  tick_timer_service_unsubscribe();
  health_service_events_unsubscribe();
  window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
