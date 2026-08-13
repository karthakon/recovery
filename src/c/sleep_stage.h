#pragma once
#include <pebble.h>
#include "hrv_math.h"

typedef enum {
  StageAwake = 0,
  StageLight = 1,
  StageDeep = 2,
  StageREM = 3
} SleepStage;

// classifier-spec-v3 s3.1: movement is three-valued. UNKNOWN is NEVER STILL.
typedef enum {
  MV_MOVED = 0,
  MV_STILL = 1,
  MV_UNKNOWN = 2
} MovementState;

// classifier-spec-v3 s4.1: cur_hr and baseline_hr are REMOVED. There is no
// live HR clause and the OS activity mask is not consulted.
SleepStage sleep_stage_classify(const HrvBuffer *minute_buf,
                                uint32_t night_baseline_variance,
                                MovementState mv);
const char *sleep_stage_name(SleepStage s);
