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
                                uint16_t cur_hr,
                                uint16_t baseline_hr,
                                bool movement) {
  HealthActivityMask act = health_service_peek_current_activities();
  // HR at least 3% below the night mean is positive sleep evidence and
  // vetoes a mask-driven Awake. Inert until a baseline exists.
  bool hr_says_sleep = (cur_hr > 0) && (baseline_hr > 0) &&
                       (cur_hr * 100 <= baseline_hr * 97);
  // Deep is no longer classified live; it is queried as finalized
  // HealthMetricSleepRestfulSeconds at session stop. Live restful minutes
  // fall through to Light here so total sleep stays correct.
  // Wake needs the mask AND corroboration: either movement, or HR that is
  // not below the night mean. Still and low-HR stays asleep.
  if (!(act & HealthActivitySleep) && !(act & HealthActivityRestfulSleep)) {
    if (movement || !hr_says_sleep) return StageAwake;
  }
  if (minute_buf->count >= 20 && night_baseline_variance > 0) {
    uint32_t v = hrv_ppi_variance(minute_buf);
    if (v * 2 > night_baseline_variance * 3) return StageREM;
  }
  return StageLight;
}
