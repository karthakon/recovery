#include "hrv_math.h"

void hrv_buf_reset(HrvBuffer *b) {
  b->count = 0;
  b->last_accepted = 0;
  b->last_accepted_time = 0;
  b->rejected = 0;
  b->rej_quality = 0;
  b->rej_range = 0;
  b->rej_jump = 0;
  b->total_accepted = 0;
}

bool hrv_buf_add(HrvBuffer *b, uint16_t ppi_ms, uint8_t quality, uint32_t now) {
  if (quality == 0) {
    b->rejected++;
    b->rej_quality++;
    return false;
  }
  if (ppi_ms < 300 || ppi_ms > 2000) {
    b->rejected++;
    b->rej_range++;
    return false;
  }
  // Jump gate only applies to consecutive beats. If more than HRV_STALE_SEC
  // elapsed since the last accepted interval, the two are not consecutive
  // (sparse overnight stream), so skip the jump check and reset the reference.
  bool fresh = (b->last_accepted_time > 0) &&
               ((now - b->last_accepted_time) <= HRV_STALE_SEC);
  if (b->last_accepted > 0 && fresh) {
    uint32_t diff = (ppi_ms > b->last_accepted) ?
      (ppi_ms - b->last_accepted) : (b->last_accepted - ppi_ms);
    if (diff * 5 > b->last_accepted) {
      b->rejected++;
      b->rej_jump++;
      return false;
    }
  }
  if (b->count < HRV_BUF_MAX) {
    b->ppi[b->count++] = ppi_ms;
  } else {
    memmove(b->ppi, b->ppi + 1, (HRV_BUF_MAX - 1) * sizeof(uint16_t));
    b->ppi[HRV_BUF_MAX - 1] = ppi_ms;
  }
  b->last_accepted = ppi_ms;
  b->last_accepted_time = now;
  b->total_accepted++;
  return true;
}

static uint32_t prv_isqrt(uint32_t n) {
  uint32_t x = n, y = (x + 1) / 2;
  if (n < 2) return n;
  while (y < x) {
    x = y;
    y = (x + n / x) / 2;
  }
  return x;
}

uint16_t hrv_mean_ppi(const HrvBuffer *b) {
  if (b->count == 0) return 0;
  uint32_t sum = 0;
  for (uint16_t i = 0; i < b->count; i++) sum += b->ppi[i];
  return (uint16_t)(sum / b->count);
}

uint16_t hrv_rmssd(const HrvBuffer *b) {
  if (b->count < 2) return 0;
  uint64_t sumsq = 0;
  for (uint16_t i = 1; i < b->count; i++) {
    int32_t d = (int32_t)b->ppi[i] - (int32_t)b->ppi[i - 1];
    sumsq += (uint64_t)(d * d);
  }
  return (uint16_t)prv_isqrt((uint32_t)(sumsq / (b->count - 1)));
}

uint32_t hrv_ppi_variance(const HrvBuffer *b) {
  if (b->count < 2) return 0;
  uint32_t mean = hrv_mean_ppi(b);
  uint64_t sumsq = 0;
  for (uint16_t i = 0; i < b->count; i++) {
    int32_t d = (int32_t)b->ppi[i] - (int32_t)mean;
    sumsq += (uint64_t)(d * d);
  }
  return (uint32_t)(sumsq / (b->count - 1));
}

uint16_t hrv_sdnn(const HrvBuffer *b) {
  return (uint16_t)prv_isqrt(hrv_ppi_variance(b));
}

// classifier-spec-v1 s3.1. Static scratch: HRV_BUF_MAX uint16_t is 800 bytes,
// too large for a stack frame on this platform. Single-threaded, called only
// from prv_close_minute, so no reentrancy concern.
static uint16_t s_mad_scratch[HRV_BUF_MAX];

// Insertion sort ascending over the first n entries of s_mad_scratch.
static void prv_sort_scratch(uint16_t n) {
  for (uint16_t i = 1; i < n; i++) {
    uint16_t key = s_mad_scratch[i];
    uint16_t j = i;
    while (j > 0 && s_mad_scratch[j - 1] > key) {
      s_mad_scratch[j] = s_mad_scratch[j - 1];
      j--;
    }
    s_mad_scratch[j] = key;
  }
}

uint32_t hrv_mad2(const HrvBuffer *b) {
  if (b->count < 2) return 0;
  uint16_t n = (b->count > HRV_BUF_MAX) ? HRV_BUF_MAX : b->count;
  for (uint16_t i = 0; i < n; i++) s_mad_scratch[i] = b->ppi[i];
  prv_sort_scratch(n);
  // Upper-middle element, integer division, no averaging. Same convention as
  // measurement-spec-v1 s3.2 v_median so the two are the same statistic.
  uint16_t med = s_mad_scratch[n / 2];
  for (uint16_t i = 0; i < n; i++) {
    uint16_t v = b->ppi[i];
    s_mad_scratch[i] = (v > med) ? (v - med) : (med - v);
  }
  prv_sort_scratch(n);
  uint32_t mad = (uint32_t)s_mad_scratch[n / 2];
  return mad * mad;
}
