#include "sleep_stage.h"

const char *sleep_stage_name(SleepStage s) {
  switch (s) {
    case StageAwake: return "Awake";
    case StageLight: return "Light";
    case StageDeep:  return "Deep";
    case StageREM:   return "REM";
    default:         return "?";
  }
}

SleepStage sleep_stage_classify(const HrvBuffer *minute_buf,
                                uint32_t night_baseline_variance,
                                MovementState mv) {
  // classifier-spec-v3 s4.1: the OS activity mask is REMOVED --
  // health_service_peek_current_activities() is not called. Live Awake is
  // movement state MOVED and nothing else. There is NO live HR clause; the
  // HR term moved to the stop-time pass (s4.2 clause 2) against A (s3.5).
  // UNKNOWN is not MOVED, so it does not declare Awake -- but it does reset
  // the onset streak at the call site (s3.4).
  // Deep is not classified live; it is queried at stop.
  if (mv == MV_MOVED) return StageAwake;
  if (minute_buf->count >= 20 && night_baseline_variance > 0) {
    uint32_t v = hrv_ppi_variance(minute_buf);
    // Light floor: variance inside the normal band around the night
    // baseline (0.5x - 2.0x) is positive Light evidence, so Light is no
    // longer just whatever REM declines to claim.
    if (v * 2 >= night_baseline_variance &&
        v <= night_baseline_variance * 2) return StageLight;
    if (v > night_baseline_variance * 2) return StageREM;
  }
  return StageLight;
}
