#pragma once
// Temporal smoother for the per-minute sleep-stage array.
// Frozen spec: recovery-smoothing-spec-v1.md (commit 7f6c250).
// Runs once at stop time as a post-pass over stored epochs.
// Three states only (Awake, Light, REM); Deep is next-action 4.
// Rewrites each EpochRecord.stage to the smoothed value and preserves
// the raw classifier stage in EpochRecord.reserved. Does NOT touch
// onset marking or BASE sampling (spec section 0).
//
// out_mins receives recomputed minute totals indexed by SleepStage,
// for StageAwake/StageLight/StageREM only. Caller keeps StageDeep.
void smoother_run(uint16_t out_mins[4]);
