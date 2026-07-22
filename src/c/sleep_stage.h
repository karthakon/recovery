#pragma once
#include <pebble.h>
#include "hrv_math.h"

typedef enum {
  StageAwake = 0,
  StageLight = 1,
  StageDeep = 2,
  StageREM = 3
} SleepStage;

SleepStage sleep_stage_classify(const HrvBuffer *minute_buf,
                                uint32_t night_baseline_variance,
                                uint16_t cur_hr,
                                uint16_t baseline_hr,
                                bool movement);
const char *sleep_stage_name(SleepStage s);
