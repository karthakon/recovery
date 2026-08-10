# Classifier spec v1 — the per-minute REM feature

**Dated 2026-08-10. Frozen BEFORE the night that tests it (N17).**

## 1. Why this exists

N16 read `RemN 14 / Runs 13 / Max 2` across 494 minutes: twelve isolated single REM
minutes and one pair. `7f6c250` requires 5 consecutive REM-observed minutes before any
REM is reported. The smoother behaved exactly as specified; smoothing-spec section 8
directs the diagnosis to `sleep_stage_classify()`.

Source read of `sleep_stage.c` 31-39, `hrv_math.c` 78-87 and `main.c` `prv_close_minute`
establishes two structural defects, both stated as findings:

1. **Scale mismatch.** `hrv_ppi_variance` is applied to a ~60-beat minute buffer and to
   the 400-beat `s_night_buf` (`HRV_BUF_MAX 400`). The 400-beat window admits
   low-frequency content the one-minute window structurally cannot. BASE and the
   per-minute value are therefore different estimators, not one statistic at two scales.
   N16 measures it: Vmed 1832 vs BASE 3063 -- the median minute sits at 0.60x the anchor
   it is compared against.
2. **Non-robust feature.** `hrv_ppi_variance` is a plain sum of squared deviations, no
   trimming, no detrending. `hrv_buf_add` admits any beat-to-beat step under 20%
   (`diff * 5 > last_accepted` rejects), and that deviation enters squared. N16:
   Vmax 205499 against P90 4544 -- the maximum is 45x the 90th percentile, implying
   ~453 ms within-minute SD. That is an outlier process, not a sleep stage.

Together these predict exactly what RUNS read: gate crossings drawn from a memoryless
artifact process produce singletons; sustained physiological elevation sits between Vmed
and P90, below the gate.

**Recorded as contributing, NOT causal:** REM and Light have no persistence mechanism
(`AWAKE_DEBOUNCE 3` and `s_awake_streak` exist for Awake only). Debounce suppresses
isolated calls; it does not manufacture runs. Adding hysteresis alone would move REM
from 0 to 0.

## 2. Scope

**In scope:** the per-minute dispersion feature; the anchor that feature is compared
against; the window over which the feature is evaluated.

**Explicitly NOT in scope:**

- The 2.0x REM multiplier and the 0.5x Light multiplier -- **unchanged, both remain.**
- `7f6c250` in any part. This spec changes the classifier's input feature, not the
  smoothing of its output.
- `876a6d6`'s BASE definition, `BASE_SAMPLE_MAX 160`, and the existing re-decision pass.
- HR as a REM term. `cur_hr` / `baseline_hr` keep their present role -- Awake veto only.
  Deferred; one variable per group.
- Native Deep, the four-state matrix, SpO2, respiration. Staging stays three-class.
- Awake decoupling, OSrest, `EpochRecord` versioning.

## 3. Definition

### 3.1 Per-minute dispersion D(m)

Replaces `hrv_ppi_variance(&s_minute_buf)` as the classifier's per-minute feature.
`D(m)` = the square of the median absolute deviation of the minute's accepted PPI values
about their median. Squaring keeps it on a variance scale. Integer arithmetic throughout,
`uint32_t`, no float.

`D(m)` is undefined and marked missing when `beat_count < 20` -- the existing gate at
`sleep_stage.c` line 31, unchanged.

### 3.2 Windowed feature F(m)

`F(m)` = the median of `D` over the centred five-minute window m-2 .. m+2, missing values
excluded. At the night's edges the window is truncated to available minutes. Minutes
classified Awake contribute their `D` to the window (windows stay contiguous) but are
excluded from 3.3.

### 3.3 Anchor A

`A` = the median of `F(m)` over all non-Awake minutes of the night with `F` defined.
Computed once at stop.

### 3.4 Decision

REM when `F(m) > 2 * A`. Light when `2 * F(m) >= A` and `F(m) <= 2 * A`. Otherwise Light.
Awake logic unchanged (`sleep_stage.c` 28-30).

### 3.5 Why this satisfies the 5-run constraint

A five-minute median cannot be moved by one outlier minute; three of five must be
elevated. A run of 5 or more REM minutes therefore requires roughly seven consecutive
elevated minutes. Real REM episodes run 10-25 minutes and clear that; artifact singletons
cannot produce any REM minute at all. **Runs arise because the feature is sustained, not
because a threshold was relaxed.**

## 4. Implementation constraints

1. `F` and `A` require the whole night, so the REM/Light decision is made at **stop**, in
   a pass over stored per-minute `D`. The live per-minute call keeps writing a provisional
   stage to `EpochRecord`; the stop pass overwrites Light/REM. **Awake is never
   overwritten.**
2. `s_epoch_var` changes contents from raw variance to `D(m)`. RAM-only,
   `EPOCH_VAR_MAX 960` unchanged. **See section 5, D2 -- this requires a dated correction
   to `0670f72`.**
3. Ordering: the new pass runs **after** the existing `prv_base_redecide` and **before**
   the smoother.
4. Median of 5 and median of the night are both integer selections on `uint32_t`; the
   night-scale sort already exists in `prv_measure`.
5. No change to `EpochRecord`, `NightSummary`, storage layout, or persist keys.
6. No firmware flash. `pebble install` only. Watch stays v4.32.0, slot 1.

## 5. Resolved decisions

- **D1 -- resolved 2026-08-10.** Anchoring REM on `A` (3.3) rather than BASE is a
  **classifier change**, in scope for this spec. `876a6d6` is untouched: BASE keeps its
  definition, its `BASE_SAMPLE_MAX 160`, its computation, its DIAG display and its `/n`
  count. It ceases to be the REM anchor and nothing else.
- **D2 -- resolved 2026-08-10.** Section 4.2 changing `s_epoch_var` contents from raw
  variance to `D(m)` requires a **dated correction to `0670f72`**, not an edit to it. The
  seven `rec_v_*` columns and `Vmax` / `P90` / `Vmed` / `Gate` keep their names and
  positions but measure `D(m)` from N17 forward. **N1-N16 values stay as recorded and are
  not comparable to N17+ on those columns** -- the same discontinuity as
  `classifier_series`, and the group boundary already marks it.

## 6. What the first night CAN and CANNOT establish

**CAN, from RUNS alone, one night:** whether the classifier now emits REM runs of 5 or
more. `L5p > 0` answers it directly. `Max`, `Runs` and the L1-L5p histogram give the
shape. This is a binary structural question and reproduces or does not.

**CANNOT, on one night:** whether identified REM is *correct* -- a run of 6 spurious and
a run of 6 real read identically here. Whether REM totals are plausible or stable.
Anything about REM percentage bands, latency, or Garmin agreement. Those need three
nights, as every prior group did.

**Failure modes registered in advance:** REM still 0 with `L5p 0` refutes section 1's
mechanism and points instead at the feature not being REM-specific at all, robust or not.
REM appearing in runs of 2-4 means the feature gained persistence but not enough
amplitude, and is **NOT** grounds to touch `7f6c250`.

## 7. Comparability

`classifier_series` moves **6 -> 7**. `recovery_commit` moves. **N17 opens a new
comparability group.** N13-N16 remain a closed group and are not comparable to it on any
REM line. `firmware_version v4.32.0`, `smoother_version v1`, `watch_slot 1`,
`platform emery` all unchanged. **No night N1-N16 is edited or recomputed.**

## 8. Status

**Frozen on commit. Not retunable. A change is a v2 or a dated correction, never an edit.**
