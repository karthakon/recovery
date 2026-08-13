# Classifier spec v3 - Awake decoupling, immobility onset, and the re-decision pass

**Dated 2026-08-13. Frozen BEFORE the code that implements it and BEFORE N20.**
**Supersedes `8793680` section 4's pass structure and the Awake branch of
`sleep_stage_classify`. `8793680` and `2401455` are NOT edited.**

## 1. Why this exists

`8793680` section 2 placed "the Awake branch of `sleep_stage_classify()`" explicitly OUT
of scope. That exclusion is now the defect. The mechanism was read from source at
`e2e79eb` and is reproduced here in full so this spec does not depend on a handoff:

1. `sleep_stage.c` 28-29: when the OS mask reports neither `HealthActivitySleep` nor
   `HealthActivityRestfulSleep`, `movement || !hr_says_sleep` returns `StageAwake`.
2. `sleep_stage.c` 21-22: `hr_says_sleep` requires `baseline_hr > 0`.
3. `main.c` 163: `s_night_baseline_hr` is assigned only when
   `s_onset_marked && s_night_hr_count >= 20 && s_night_baseline_hr == 0`. The `== 0`
   makes it a ONE-SHOT LATCH.
4. `main.c` 129-130: marking onset zeroes `s_night_hr_sum` and `s_night_hr_count`, so the
   20 samples count FROM ONSET.
5. Therefore pre-onset `baseline_hr` is 0, `hr_says_sleep` is false, and every pre-onset
   minute the mask has not called sleep returns Awake regardless of stillness or HR.
6. `main.c` 125: Awake zeroes `s_sleep_streak`, so onset never marks, so the baseline
   never latches. The deadlock breaks only when the OS mask flips.
7. `AWAKE_DEBOUNCE 3` (`main.c` 72, 119) substitutes `s_last_stage` for the first two
   Awake minutes only; a sustained block passes it.
8. `prv_base_redecide` (`main.c` 414-445) emits ONLY `StageLight` or `StageREM`. There is
   NO `StageAwake` path. Live Awake labels reach RESULTS untouched.
9. `prv_stop_recording` (`main.c` 449-453) calls `prv_compute_anchor` BEFORE
   `prv_base_redecide`, so anchors are computed from LIVE labels.
10. `prv_compute_anchor` skips Awake at both anchors (`main.c` 374, 396), and both anchors
    are medians over the surviving set.

**Consequence: the mislabelled Awake block at session start is deleted from the median
that sets the REM threshold for the whole night.** Measured contiguous blocks at session
start: N17 69 min, N18 70 min, N19 68 min.

**The onset constant is not the defect.** `SLEEP_ONSET_MINUTES 5` (`main.c` 35) is the
value the actigraphy literature validates best. Recovery counts 5 consecutive non-Awake
minutes AS JUDGED BY THE CLASSIFIER; the convention counts 5 consecutive IMMOBILE minutes,
which depends on nothing downstream. Same 5, different input. That difference is the
circularity.

## 2. Scope

**In scope:** the Awake decision and its inputs; the definition of sleep onset; the order
of the stop-time passes; the within-night HR reference `A`; the freshness guard on HR and
on movement.

**NOT in scope, unchanged:**

- `D(m)`, `F(m)`, their windowing, and the 2.0x / 0.5x multipliers - `c5edaee` 3.1-3.2.
- `A_D` and `A_H` computation - `8793680` 3.1, 3.4. **Only the label set they filter on
  changes, not their arithmetic.**
- T1, T2, T3 as the REM conjunction - `8793680` section 4. **No term is added, removed or
  retuned.**
- `S(m)` and the `s_epoch_still` bitmap - `2401455` section 3.
- `876a6d6` BASE in every part, `BASE_SAMPLE_MAX 160`, `prv_base_median()`.
- `7f6c250` in any part, including the 5/7/18 effective REM floor.
- `EpochRecord` layout, size, version and field semantics. `reserved` retains its meaning
  as the pre-smoother stage written at `smoother.c` 215.
- Native Deep and the four-state matrix. **HELD.** Section 9 states where it will go and
  does not specify it.
- SpO2, respiration. Staging stays three-class.
- The cross-night HR baseline `B`. **Registered in 6.3 as future work, NOT specified
  here.**
- The live Light/REM statistic. **See 4.1's open item.**

## 3. Definitions

**3.1 Minute movement state.** Three-valued, replacing today's boolean:

- **MOVED** - `s_mv_min_samples > 0` and
  `s_mv_min_moved * 100 >= s_mv_min_samples * MV_MOVED_PCT`.
- **STILL** - `s_mv_min_samples > 0` and the above is false.
- **UNKNOWN** - `s_mv_min_samples == 0`.

**UNKNOWN is NEVER treated as STILL.** `prv_accel_peek` (`main.c` 19) runs only inside
`prv_health_handler` (`main.c` 176), so losing health events zeroes the accel sample
count. Under an immobility-based onset, treating UNKNOWN as STILL makes off-wrist read as
instant sleep onset. Review finding 6, pulled forward.

**3.2 HR freshness.** A new timestamp `s_last_hr_time`, assigned at `main.c` 180 in the
same branch that assigns `s_last_hr`. `s_last_hr` is FRESH at minute close when
`s_last_hr > 0` and `now - s_last_hr_time <= HR_STALE_SEC`; otherwise MISSING.

`s_last_hr` is never cleared - `main.c` 180 assigns only when `hr > 0` - so a presence
test passes forever on a stale value. `HRV_STALE_SEC` is 10 (`hrv_math.h` 5) and governs
the jump gate's consecutive-beat test inside `hrv_buf_add` (`hrv_math.c` 25-37). It is a
beat-to-beat constant and is NOT reused here.

**`HR_STALE_SEC` = 180 seconds. FROZEN, on a measurement.**

**Measured 2026-08-13** from the IDLE `ev H` counter, wrist worn, still, daytime:
13:13 `ev H 0` to 13:28 `ev H 896`. **896 events / 15 min = about 59.7 per minute, roughly
one HR event per second.** 180 s is about 180x the mean inter-arrival.

**Caveat, recorded:** this is a daytime still-wear sample and a MEAN, NOT A TAIL. It is
retained as sufficient because at that headroom no plausible tail closes the gap; 30 s
would also have been safe. **`s_hr_events` (`main.c` 10, 178) is APP-LIFETIME with no
session reset** and therefore cannot be divided by a session length - a rate over a known
interval was measured instead.

**3.2.1 What this guard gates - grepped, not inferred.** `s_night_hr_sum` has exactly two
consumers (`main.c` 99, 164, 478-479):

- **`main.c` 164 to `s_night_baseline_hr`, passed at 113.** Under 4.1 this consumer is
  **DEAD** and is removed by 4.1.1.
- **`main.c` 478-479 to `ns.mean_hr`.** Survives. **NOT a classifier input.**

**Therefore `HR_STALE_SEC` gates `ns.mean_hr` and NOTHING ELSE.** It does not gate onset
(movement-only, 3.4), `A`, either anchor, the Awake decision or REM. **A wrong value could
not corrupt any N20 decision** - a second, independent reason the constant is safe.

**Why the guard is nonetheless worth having:** `mean_hr` is the input to the future
cross-night baseline `B` (next-action 21). **A silently contaminated `mean_hr` today
becomes a contaminated cross-night baseline at 14 nights** - the same shape as the
`rec_classified_min` defect: harmless-looking now, structural later.

**3.3 Consequence for `s_night_hr_sum`.** `main.c` 98-99 sums `s_last_hr` unconditionally
when non-zero. Under 3.2 it accumulates ONLY WHEN FRESH.

**3.4 Sleep onset.** Onset marks at the close of the FIFTH CONSECUTIVE STILL MINUTE.
`SLEEP_ONSET_MINUTES` stays **5**, unchanged in name and value.

- MOVED resets the streak to 0.
- **UNKNOWN resets the streak to 0**, same as MOVED. A run interrupted by minutes carrying
  no evidence is not five consecutive immobile minutes; the alternative silently bridges
  an off-wrist gap.
- Onset depends on ACCELEROMETER DATA ALONE. It does not read the OS mask, any stage
  label, any HR value, any HRV statistic, or any stored night. **It works on night 1 and
  after total data loss.** Non-negotiable.
- Onset is marked in the LIVE pass and is never re-decided at stop.

**3.5 The within-night HR reference `A`.**

> `A` = median of **`HF(m)`** over every stored minute at or after the onset index with
> `HF(m)` defined and movement state STILL. Computed AT STOP, upper-middle selection, no
> averaging. If fewer than `A_MIN_MINUTES` minutes qualify, `A` is undefined.

**`A` is defined over `HF(m)`, not `H(m)`, deliberately.** Section 4.2 clause 2 compares
`HF(m)` against `A`; defining `A` over raw `H(m)` would put the two sides on different
statistics - the `NightSummary` scope mix (next-action 13) and review finding 3's
"different statistic, different reference" reappearing inside the spec written to fix
them. Both sides live on the same smoothed series, and `A` becomes exactly parallel to
`A_H` in `8793680` 3.4: a median of a windowed median.

Properties, stated so they are checkable:

1. **It is a whole-night statistic, not an early-night one.** The early-high bias - the
   first ~20 minutes past onset carry awake-adjacent HR - applies to a FIRST-20-SAMPLES
   definition. **It does not apply to this one.**
2. **It is a median, matching `A_D` and `A_H`.** No new estimator class enters the
   project.
3. **It is not fitted.** Within-night, no constant taken from any reading.
4. **It is NOT circular with the decision it feeds.** `A` filters on STILL, which is
   accelerometer-derived. `A_H` filters on Awake labels and is the self-referential half
   of review finding 4. **This is why `A` is defined here rather than reusing `A_H`.**

**Recorded, not fixed:** STILL includes quiet wake, whose HR sits above sleep HR, so `A`
is biased slightly HIGH, making clause 2 slightly harder to satisfy. Same sign as the bias
property 1 rules out, much smaller, and in the safe direction.

**`A_MIN_MINUTES` = 20**, carried from the existing `s_night_hr_count >= 20` at `main.c`
163 so the count is not silently changed while its meaning is.

**When `A` is undefined, no minute is scored Awake by the HR term.** The movement term
still applies. **A missing baseline must NEVER make Awake easier to declare** - that is
today's failure and it is not to be reproduced with the sign flipped.

**3.6 Windowed Awake score.** `AW(m)` is computed over the centred window `m-2 .. m+2`,
missing excluded, truncated at edges - **the same window shape as `F(m)` and `HF(m)`**
(`8793680` 3.3), so no term in the project fires on a single isolated minute.

**DEPARTURE FROM COLE-KRIPKE, recorded deliberately.** Cole-Kripke scores over seven
asymmetric past-heavy terms - four preceding minutes, the current minute, two following.
Section 3.6 uses a symmetric 5-minute centred window, chosen for internal consistency with
`F(m)` and `HF(m)`. **This is a trade, not an oversight.** Recorded so a later session
does not rediscover the mismatch and try to correct it.

## 4. The Awake decision

### 4.1 Live pass - `sleep_stage_classify`, at minute close

**The OS mask is REMOVED.** `health_service_peek_current_activities()` is not called.

**Live Awake is movement state MOVED, and nothing else.** There is NO live HR clause.
Reasons, recorded:

- Under 3.5, `A` is a stop-time statistic. A live provisional `A` reintroduces a
  latch-shaped early-night quantity, which is the thing being removed.
- The live labels' load-bearing uses were: onset marking (now movement-only, 3.4), the
  awake debounce, and the Awake skips in `prv_compute_anchor` and `prv_base_redecide`.
  The last two are superseded by section 5's pass order.

**`AWAKE_DEBOUNCE 3` is RETAINED and is NOT vestigial.** `main.c` 119 substitutes
`s_last_stage` into `st` BEFORE `rec.stage = st` at 132, so it shapes the STORED stage.
Live Awake labels are read at `main.c` 272 (`prv_measure`), 374 and 396
(`prv_compute_anchor`) and 421 (`prv_base_redecide`); under section 5 the last three read
re-decided labels. **`EpochRecord.reserved` is written at `smoother.c` 215, AFTER both
re-decision passes, and therefore does NOT carry the live label** - the debounce does not
reach RUNS through it.

**FINDING 3 - resolved in ONE direction, stated: correct the `hrv_math.h` comment, do NOT
change `sleep_stage.c`'s statistic.** The comment claiming `hrv_mad2` "replaces
`hrv_ppi_variance` as the CLASSIFIER's per-minute feature" is inaccurate and is corrected
to say it replaces it in the stop-time re-decision pass only.

> **CORRECTED BY SOURCE READ, 2026-08-13.** An earlier form of this section justified the
> above by claiming live Light/REM labels are overwritten at stop for every non-Awake
> minute. **That is FALSE.** `prv_base_redecide` skips `rec.beat_count < 20` (`main.c`
> 422) and `s_epoch_f[i] == 0` (424); those minutes RETAIN their live labels through to
> RESULTS, the hypnogram and the smoother. **Changing the live statistic WOULD change
> output**, on the skipped minutes. That change is OUT OF SCOPE here and is registered as
> an open item, not silently absorbed. Finding 3's "decide which; do not do both silently"
> is satisfied by recording which.

### 4.1.1 `s_night_baseline_hr` is REMOVED

With the live HR clause gone, `s_night_baseline_hr` has no reader. **It is REMOVED, not
retained-unused.** Sites, from a grep run 2026-08-13:

- `main.c` 58 - declaration
- `main.c` 113 - the argument passed into `sleep_stage_classify`
- `main.c` 163-164 - the one-shot latch
- `main.c` 218 - the session-start reset

**`baseline_hr` AND `cur_hr` are dropped from `sleep_stage_classify`'s signature** -
`sleep_stage.c` 15-16 and `sleep_stage.h` 14-15. `hr_says_sleep` (`sleep_stage.c` 21-22)
is deleted. **Grepped 2026-08-13: `cur_hr` and `baseline_hr` occur ONLY at the signature
and inside `hr_says_sleep`. The Light/REM branch (`sleep_stage.c` 31-38) reads neither.**

**`s_night_hr_sum` and `s_night_hr_count` are RETAINED** - they feed `ns.mean_hr`
(`main.c` 478-479), which is not a classifier input.

**Rationale for removal over retention:** the one-shot latch is precisely the mechanism
section 1 exists to document as removed. Leaving it in the file is how a later reader
reconstructs a mechanism that no longer exists.

### 4.2 Re-decision pass - Awake scoring

Awake JOINS the re-decision pass. For each stored minute, Awake when EITHER:

1. `AW(m)` reports MOVED in at least **`AW_MOVED_MIN` = 3** of the 5 window minutes, OR
2. `A` is defined, `HF(m)` is defined, and `HF(m) * 100 > A * 103`.

**`AW_MOVED_MIN 3 of 5` is an UNWEIGHTED majority where the literature uses a weighted
sum. It is a deliberate simplification and is UNVALIDATED against PSG**, whereas
Cole-Kripke reports about 88 percent agreement in 41 subjects. Recorded so windowed
scoring here is not later treated as literature-backed in a stronger sense than it is.

**On the 103 - constant carried, reference NOT carried.** It mirrors `sleep_stage.c` 22's
`cur_hr * 100 <= baseline_hr * 97`, the same 3 percent band applied as "above baseline"
rather than "not below baseline." **But 97 was applied against `baseline_hr`, a MEAN over
post-onset minutes (`main.c` 163-164); 103 is applied against `A`, a MEDIAN over
post-onset STILL minutes.** Different estimator, different filter. The constant is
carried; its reference is not. **Not fitted, and frozen here rather than chosen after a
reading.**

**What transfers from Cole-Kripke is the STRUCTURE - scoring a minute over a window of
surrounding minutes - not the constants.** No Cole-Kripke coefficient appears in this
spec. **Footnote, as direct evidence they are calibration-bound:** published forms of the
same algorithm differ in SCALING for identical weights - one gives
`SI = 0.001 x (106 A-4 + 54 A-3 + ...)`, another
`S = 0.0033 x (1.06 a-4 + 0.54 a-3 + ...)` - because count units differ by device.

## 5. Pass order

**Today (`main.c` 449-453):** `prv_close_minute`, `prv_base_median`,
`prv_compute_anchor`, `prv_base_redecide`, `prv_measure`, `smoother_run`.

**Under v3:**

1. `prv_close_minute`
2. `prv_base_median` - unchanged
3. **`prv_awake_redecide`** - NEW. Computes `A` (3.5), scores both clauses (4.2), writes
   `StageAwake` / clears it. **This is the ONLY pass that writes `StageAwake`.**
4. `prv_compute_anchor` - **unchanged arithmetic**, now filtering on re-decided labels
5. `prv_base_redecide` - **unchanged**, Light/REM only, still one pass (`bae23c3` s4)
6. `prv_measure` - unchanged, still after the re-decision
7. `smoother_run` - unchanged

**Step 3 MUST run before step 4.** That single ordering is the whole mechanism: anchors
computed over a corrected label set.

**A stated design decision, not an accident:** live and re-decision labels will DISAGREE
about Awake. That is intended. Onset is live and movement-only; Awake scoring is at
re-decision. **A re-decision-only Awake change with a stage-dependent onset would leave
the live path still driving onset** - which is why 3.4 moves onset to accelerometer data.

**`s_mins` bookkeeping:** step 3 decrements the old stage and increments the new, the
pattern `prv_base_redecide` uses at `main.c` 441-442. **The four-stage double count at
`main.c` 467 is untouched** - classified stays Awake+Light+REM, no four-stage sum is
rendered (review finding 5).

## 6. Registered predictions - frozen BEFORE the code exists

**6.1 THE ANCHOR-BIAS PREDICTION.** If the anchor-bias mechanism is the dominant cause of
the REM shortfall:

- **`anchor_hr` FALLS on series 9** relative to series 8.
- **`RemN` RISES.**
- **With NO change to the 2.0x multiplier and NO change to T2's definition.**

**The discriminating outcome: if Awake is corrected and `RemN` does NOT move, the
anchor-bias mechanism is WRONG and review finding 4's median-split problem is the real
one.** Both branches are registered NOW, before either can be observed.

**6.2 Awake block.** The contiguous Awake block at session start - N17 69, N18 70, N19 68,
three consecutive nights, one block each - **SHORTENS on series 9.** No band is
registered: two mechanisms (mask removal, immobility onset) change together and this spec
does not claim to apportion them.

**6.3 NOT PREDICTED, recorded as unknown.** The sign of the effect on `A_D` (T1) is
**UNKNOWN and is not guessed.** The direction argument for `A_H` - that the deleted
minutes carry the night's lowest HR, and removing low values raises a median - is a
reasoned physiological expectation, **NOT established on Kristopher's own data.** It is
checkable from `s_epoch_hf` and has not been checked.

## 7. What one night can and cannot establish

**CAN:** the direction of `anchor_hr`; the direction of `RemN`; whether the session-start
Awake block shortens; whether onset moves toward the ~10-minute self-reported latency.

**CANNOT:** whether the identified REM is CORRECT - there is no ground truth and Garmin is
not it. Anything about stability, spread or bands. **Nothing licenses fitting any
constant.**

**Registered failure modes:**

- **Awake collapsing to near zero** means the removal of the OS mask left no working wake
  evidence, and identifies clause 1 or clause 2 to interrogate. **It is NOT grounds to
  restore the mask or to retune `MV_MOVED_PCT`.**
  **If this branch occurs, Webster's rescoring rules are the ESTABLISHED remedy and are to
  be reached for before anything is invented** - canonically, the first sleep minute
  following at least 4 wake minutes is rescored wake, extending to 4 sleep minutes after
  15 wake minutes. **They push toward Wake and must NOT be adopted now**, because
  Recovery's current defect is OVER-calling Awake and they would worsen it. Registered
  against this branch only. **Caution for whenever it arrives: the constants are heuristic
  with no clear recalibration procedure for a new population or device.**
- **Onset marking within the first 5 minutes of every session** means UNKNOWN is being
  read as STILL somewhere the spec did not catch.

**N20 is the first night on BOTH v4.33.1 AND `classifier_series 9`.** The bundling was
chosen deliberately: one boundary instead of two. **Accepted cost, recorded: if N20 reads
strangely, firmware and classifier CANNOT be separated.**

## 8. Implementation constraints

1. **One Awake re-decision pass.** No second pass, no rewrite of `prv_base_redecide`.
2. `A` is computed inside `prv_awake_redecide`, BEFORE `prv_compute_anchor` runs.
3. New static `s_last_hr_time`. New `HR_STALE_SEC`, `A_MIN_MINUTES`, `AW_MOVED_MIN` as
   named defines, never bare literals.
4. Movement state is three-valued at minute close and its per-minute value must be
   available to the re-decision pass. **`EpochRecord` is NOT to grow for this** - a
   RAM-only array or a second bitmap, index-parallel to `s_epoch_var`, following the
   pattern `2401455` section 3 established. **`EpochRecord.reserved` is NOT touched.**
5. **Grep for readers AND writers before asserting any field or bit is free.** One write
   site proves nothing - `2401455` section 1.
6. Integer arithmetic throughout, no float.
7. **No firmware flash.** `pebble install` only. Stock v4.33.1.
8. **BUILD AFTER EVERY EDIT.** Python edit passes locate BY CONTENT with an assert on a
   unique token.
9. `CLASSIFIER_SERIES` **8 to 9**, bumped in `src/c/storage.h` **in the IMPLEMENTATION
   commit, NOT in this spec's commit.**

## 9. Where native Deep will go - NOT specified here

When native Deep is taken up under explicit go, it becomes a **fourth branch in the
conjunction block** (`main.c` 433-439), replacing the wholesale
`s_mins[StageDeep] = deep_min` overwrite at `main.c` 467. That overwrite lands AFTER
`smoother_run`, so Deep today bypasses BOTH the re-decision and the smoother. **HELD. This
spec does not specify Deep and no part of it may be read as doing so.**

## 10. Comparability

`classifier_series` moves **8 to 9**. `recovery_commit` moves. **N20 opens a new
comparability group.** `classifier_series 8` closed with two nights, N18 and N19, **both
confounded and DIFFERENTLY confounded - no clean night and no context-matched pair.** N18
and N19 are not comparable to N20 on any REM line and **are NOT re-scored.**

**No night N1-N19 is edited or recomputed**, including `rec_classified_min`.

**`Gate` continues to count T1 ALONE**, unchanged in meaning from series 8.
`RemN <= Gate` remains registered as a strict inequality.

**`watch_slot` is recorded EMPTY from N20 onward.** Slot tracking ended when the watch
moved to stock v4.33.1; the active slot cannot be queried from the device. **Read from
`tools/validate.py` 2026-08-13: the empty-and-NA continue executes BEFORE the INT check,
so empty is legal and skips validation.** `-1` is NOT legal - it is in `BAD_SENTINELS` and
`watch_slot` is not in `SIGNED_FIELDS`. The field is RETAINED in `fields.py`, the column
count stays 81, and N1-N19's recorded values stand. **Meaning: not applicable, the device
runs stock firmware and the active slot is not queryable.**

## 11. Status

**Frozen on commit. Not retunable. A change is a v4 or a dated correction, never an
edit.**
