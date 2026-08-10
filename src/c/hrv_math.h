#pragma once
#include <pebble.h>

#define HRV_BUF_MAX 400
#define HRV_STALE_SEC 10

typedef struct {
  uint16_t ppi[HRV_BUF_MAX];
  uint16_t count;
  uint16_t last_accepted;
  uint32_t last_accepted_time;
  uint32_t rejected;
  uint32_t rej_quality;
  uint32_t rej_range;
  uint32_t rej_jump;
  uint32_t total_accepted;  // running total; count pins at HRV_BUF_MAX
} HrvBuffer;

void hrv_buf_reset(HrvBuffer *b);
bool hrv_buf_add(HrvBuffer *b, uint16_t ppi_ms, uint8_t quality, uint32_t now);
uint16_t hrv_rmssd(const HrvBuffer *b);
uint16_t hrv_sdnn(const HrvBuffer *b);
uint16_t hrv_mean_ppi(const HrvBuffer *b);
uint32_t hrv_ppi_variance(const HrvBuffer *b);

// classifier-spec-v1 s3.1: squared median absolute deviation of the buffer's
// PPI values about their median. Robust per-minute dispersion; replaces
// hrv_ppi_variance as the CLASSIFIER's per-minute feature. hrv_ppi_variance
// is unchanged and still backs BASE over s_night_buf (base-spec-v1).
// Returns 0 when count < 2. Integer only, no floats.
uint32_t hrv_mad2(const HrvBuffer *b);
