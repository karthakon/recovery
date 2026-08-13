#pragma once
#include <pebble.h>

#define EPOCHS_PER_KEY 21
#define MAX_EPOCH_KEYS 32
#define MAX_NIGHTS 30

typedef struct __attribute__((packed)) {
  uint16_t mean_ppi;
  uint16_t rmssd;
  uint8_t beat_count;
  uint8_t stage;
  uint8_t quality_pct;
  uint8_t reserved;
} EpochRecord;

#define NIGHT_SUMMARY_VERSION 3
// nights-render-spec-v1 s2: bumped BY HAND in the same commit as any spec that
// changes the classifier. 9 = classifier-spec-v3 (2ae829a, corrected acf88e4):
// Awake decoupled from the OS mask, immobility onset, prv_awake_redecide
// before prv_compute_anchor. 8 was classifier-spec-v2 (ed12fd7), the
// three-term conjunction. 7 was classifier-spec-v1; N1-N16 were 6 and below.
#define CLASSIFIER_SERIES 9
typedef struct __attribute__((packed)) {
  uint8_t version;  // NIGHT_SUMMARY_VERSION at save time; 0 = pre-versioning
  time_t date;
  uint16_t rmssd;
  uint16_t sdnn;
  uint16_t mean_ppi;
  uint16_t epoch_count;
  uint16_t mins_awake;
  uint16_t mins_light;
  uint16_t mins_deep;
  uint16_t mins_rem;
  uint32_t rejected;
  uint32_t rej_range;
  uint32_t rej_jump;
  uint32_t beats;  // true accepted-beat total (epoch_count is minutes)
  time_t start_time;
  time_t end_time;
  uint16_t mean_hr;
  uint32_t baseline_var;  // BASE: frozen post-onset variance
  uint32_t night_var;     // NIGHT: sliding-window variance at stop
  uint8_t batt_start_pct; // battery % at record start (10% resolution)
  uint8_t batt_end_pct;   // battery % at record stop
  // measurement-spec-v1 s3.5: appended at v2, never inserted.
  // Absent on v<2 records - display as -- , never as 0.
  uint32_t v_max;
  uint32_t v_p90;
  uint32_t v_median;
  uint32_t base_min;
  uint32_t base_max;
  uint16_t v_count;
  uint16_t v_over_gate_count;
  // nights-render-spec-v1 s2: appended at v3, never inserted.
  // Absent on v<3 records - display as -- , never as 0.
  uint8_t classifier_series;
} NightSummary;

// Size of a v1 record: the struct minus the 24-byte v2 tail
// (5 x uint32_t + 2 x uint16_t). measurement-spec-v1 s3.5 requires
// the read path to accept both sizes so v1 nights stay readable.
// nights-render-spec-v1 s2: tails are CUMULATIVE from the current sizeof, so
// each historical size stays numerically fixed as the struct grows. Defining
// V1_SIZE against a bare 24 would have shifted it when v3 appended a byte and
// silently rejected every v1 record.
#define NIGHT_SUMMARY_V3_TAIL_BYTES 1
#define NIGHT_SUMMARY_V2_TAIL_BYTES 24
#define NIGHT_SUMMARY_V2_SIZE (sizeof(NightSummary) - NIGHT_SUMMARY_V3_TAIL_BYTES)
#define NIGHT_SUMMARY_V1_SIZE (sizeof(NightSummary) - NIGHT_SUMMARY_V3_TAIL_BYTES \
                                                    - NIGHT_SUMMARY_V2_TAIL_BYTES)

void storage_session_start(void);
void storage_epoch_write(const EpochRecord *rec);
uint16_t storage_epoch_count(void);
bool storage_epoch_read(uint16_t idx, EpochRecord *out);
void storage_epoch_update(uint16_t idx, const EpochRecord *rec);
void storage_night_save(const NightSummary *ns);
uint8_t storage_night_count(void);
bool storage_night_read(uint8_t idx_from_newest, NightSummary *out);

// --- SpO2 (separate keys so NightSummary layout is untouched) ---
#define SPO2_PER_KEY 60
#define MAX_SPO2_KEYS 2

typedef struct __attribute__((packed)) {
  uint16_t offset_min;   // minutes since session start
  uint8_t percent;
  uint8_t quality;       // HRMQuality cast to uint8 (255 = off-wrist)
} SpO2Sample;

typedef struct __attribute__((packed)) {
  uint8_t min_pct;
  uint8_t avg_pct;
  uint8_t count;
  uint8_t reserved;
} SpO2Night;

void storage_spo2_session_start(void);
void storage_spo2_write(const SpO2Sample *rec);
uint16_t storage_spo2_count(void);
bool storage_spo2_read(uint16_t idx, SpO2Sample *out);
// Save alongside the just-saved night (uses the current newest slot).
void storage_night_save_spo2(const SpO2Night *sn);
// False (and zeroed out) for nights recorded before SpO2 support.
bool storage_night_read_spo2(uint8_t idx_from_newest, SpO2Night *out);
