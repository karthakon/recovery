# Recovery — Whole-Night BASE Specification v1

**FROZEN 2026-08-05, BEFORE N13 EXISTS.**

This document defines the change to Recovery's BASE anchor made after the
N10-N12 comparability group closed. It is dated before the first night that
runs it, for the same reason `docs/smoothing-spec-v1.md` was: so that no
parameter in it can have been chosen by looking at the data it will be judged
against.

**Nothing in this document amends `docs/smoothing-spec-v1.md` (`7f6c250`).**
That spec remains frozen and untouched. See section 1.

Closes next-actions 18 and 25 as a pair.

---

## 0. Why this change exists

Three nights (N10, N11, N12) ran an identical classifier, smoother, and
firmware, with no controlled variable moved:

| Night | BASE (n24) | ctx |
|---|---|---|
| N10 | 1747 | alcohol (2 beers), salty snack |
| N11 |  459 | alcohol-free, headache-delayed onset |
| N12 |  641 | alcohol (2 drinks), late meal |

Spread **1747 / 459 / 641 = 3.81x max/min**. N12 landed outside both prior
+/-20% bands (pre-registered as P1c, "neither"), which argues for **genuine
scatter rather than a two-valued BASE**. Across three nights with nothing
changed, BASE does not reproduce.

`BASE_SAMPLE_MAX 24` caps sampling at 24 x 400 = 9600 accepted beats past
onset - roughly the first 143 minutes, about a quarter of a long night. A
window that only sees early sleep tracks whatever dominated early sleep that
night, and early-sleep composition is exactly what alcohol, late meals, and
delayed onset perturb.

**This is stated as a hypothesis the context pattern supports, not a proven
cause.** Three points cannot separate "cap too short" from "early sleep is
genuinely more variable." What the group does establish is narrower and
sufficient: **24 as-is is not defensible.**

### Literature basis (this is the part that is genuinely pre-data)

The published wrist-PPG staging algorithms that disclose their normalization
use a **whole-night** anchor, not an early window:

- **Walch et al., SLEEP 2019** ("Sleep stage prediction with raw acceleration
  and photoplethysmography heart rate data derived from a consumer wearable
  device"): each individual is normalized by dividing by the 90th percentile
  of the absolute difference between each heart-rate measurement and the mean
  heart rate **over the sleep period**. The anchor is computed retrospectively
  across the entire night.
- **SLEEP 2020** Apple Watch / Oura / PSG+actigraphy comparison: tested
  normalization at the **night level**, transforming each feature to a
  z-score within the night.

No published wrist-HRV staging algorithm located during this review anchors on
an early-sleep window.

**Garmin is not a source here.** Garmin's stage classifier is proprietary and
its baseline handling is not disclosed. Garmin's *HRV Status* baseline is
documented (a 7-day rolling average, ~19 nights to establish) but that is a
**cross-night** baseline for a different feature, and must not be read as how
Garmin anchors stage classification. Nothing in this spec is derived from
Garmin behaviour.

The 2019/2020 methods normalize retrospectively, with the whole night in hand.
That is the shape this spec adopts, adapted to on-watch constraints in
section 3.

---

## 1. Relationship to the frozen smoothing spec

`docs/smoothing-spec-v1.md` (`7f6c250`) is **not modified, superseded, or
renegotiated by this document.** Specifically:

- **No numeric parameter in that spec moves.** Not a transition probability,
  not the 0.75 emission confusion, not the REM latency multipliers, not a
  minimum episode duration, not any section 6 four-state value.
- **That spec placed `BASE_SAMPLE_MAX 24` outside its own boundary.** Its
  section 0 "Explicitly NOT in scope, and untouched by this spec" lists
  "**`BASE_SAMPLE_MAX 24`.** Separate question, deferred past N7." This spec
  operates in the region the smoothing spec declared it does not govern.
- **The structural invariant that spec was built to protect is preserved.**
  Its section 0 reasoning: "BASE sampling gates on onset; onset depends on
  Awake. If the smoother could move onset, it would move BASE, and no future
  BASE-stability comparison would be interpretable." This spec leaves the
  Awake decision live and untouched (section 2), so onset remains live and
  BASE sampling remains mechanically independent of both post-passes.

**One code comment becomes imprecise and is corrected in the same commit.**
`smoother.h` lines 6-7 describe `EpochRecord.reserved` as preserving "the raw
classifier stage." After this change, `reserved` holds the **re-decided**
stage. The smoothing spec's section 7 requirement - keep the pre-smoother
array available for comparison - is still satisfied; only the word "raw" is
corrected. This is a comment edit in `smoother.h`, not a spec change.

---

## 2. Scope and non-scope

**In scope:** the definition of the BASE anchor, and the re-decision of
**Light vs REM only** at stop time against that anchor.

**Explicitly NOT in scope, and untouched:**

- **The Awake branch of `sleep_stage_classify()`.** It is BASE-independent by
  construction - it depends on `health_service_peek_current_activities()`,
  `movement`, and `cur_hr` vs `baseline_hr`, none of which involve
  `night_baseline_variance`. It is also **unreproducible at stop time**: the
  activity mask is a live peek that does not exist after the session. Awake
  minutes are carried through the re-decision pass unchanged.
- **Onset marking.** `s_onset_marked` is set live and stays live.
- **The Light/REM threshold multipliers themselves** (0.5x / 2.0x). Only the
  value they are measured against changes. The 2.0x REM multiplier remains
  untouched per standing instruction.
- **The wake rule.** The standing Awake over-call divergence is judged on its
  own trend, separately. This spec does not correct it.
- **Deep.** Still not natively classified; `s_mins[StageDeep]` continues to be
  overwritten at stop from `HealthMetricSleepRestfulSeconds`. Next-action 4
  remains held.
- **The smoother.** Runs afterward, unchanged, on the re-decided array.
- **`EpochRecord` and `NightSummary` storage layouts.** Neither changes. See
  section 4.

---

## 3. Definition

### 3.1 Sampling

`BASE_SAMPLE_MAX` is raised to a value that **does not bind on a physiological
night**: **160**.

- 160 x 400 = 64,000 accepted beats, about 13 hours at 82 bpm.
- N12, the longest-beat night in the group, accepted 39,149 beats, about 98
  samples. 160 leaves substantial headroom.
- The cap is retained rather than removed so the array stays statically sized
  and bounded. **It is a safety bound, not a modelling parameter.** If it ever
  binds on a real night, that is a defect to report, not a result to
  interpret.

Sampling otherwise continues exactly as today: one sample every
`HRV_BUF_MAX` (400) accepted beats past onset, each sample being
`hrv_ppi_variance(&s_night_buf)` over the trailing 400-beat window.

**Consequence for recorded data:** the on-screen BASE `/n` count will now read
roughly 90-110 on a full night instead of a fixed 24. This is expected and is
the visible signature that the change is live.

### 3.2 Live BASE during the night

`s_night_baseline_var` continues to update live as samples arrive, now as an
**expanding median over all samples so far** rather than one that freezes at
24. The live SESSION screen continues to classify against it.

This live value is **not** the whole-night anchor. It is what the initially
stored per-minute stages are produced against, and it is superseded at stop.

### 3.3 Whole-night BASE (the anchor)

At stop time, before any post-pass:
```
BASE_final = median( all s_base_samples[0 .. s_base_sample_count-1] )
```
This is the value written to `ns.baseline_var` and displayed on RESULTS. **The
BASE figure recorded for a night is the whole-night median from N13 onward.**

### 3.4 Re-decision pass

For each stored epoch index `i` in `0 .. storage_epoch_count()-1`:

1. Read the `EpochRecord`.
2. **If `stage == StageAwake`, skip.** Awake is out of scope (section 2).
3. **If `beat_count < 20`, skip.** Those minutes fell through to Light without
   ever consulting BASE, matching the classifier's existing
   `minute_buf->count >= 20` gate. Re-deciding them would invent a decision
   the classifier never made.
4. **If `BASE_final == 0` or the stored per-minute variance is 0, skip.**
   Matches the classifier's existing `night_baseline_variance > 0` guard.
5. Otherwise apply the **unchanged** thresholds against `BASE_final`, using
   the per-minute variance `v` for that epoch:
   - `v * 2 >= BASE_final && v <= BASE_final * 2` -> `StageLight`
   - `v > BASE_final * 2` -> `StageREM`
   - otherwise -> `StageLight`
6. If the stage changed, write it back via `storage_epoch_update(i, &rec)` and
   adjust `s_mins[StageLight]` / `s_mins[StageREM]` accordingly.

The arithmetic is byte-identical to the live classifier's Light/REM block. The
only substitution is `BASE_final` for `s_night_baseline_var`.

### 3.5 Ordering - forced, not chosen

```
prv_close_minute()
-> BASE re-decision pass (this spec)
-> smoother_run(s_mins) (7f6c250, unchanged)
-> s_mins[StageDeep] overwrite from HealthMetricSleepRestfulSeconds
-> NightSummary assembly
```

**The re-decision pass MUST run before `smoother_run()`.** Running it after
would overwrite smoothed labels with unsmoothed ones and defeat the smoother
entirely. `s_mins` must be corrected by the re-decision pass before it is
handed to the smoother.

---

## 4. Implementation constraints

- **No storage format change.** Per-minute variance is held in a **RAM-only**
  parallel array, filled in `prv_close_minute()` at the moment the minute
  buffer's variance is computed and **before `hrv_buf_reset(&s_minute_buf)`**.
  It is not persisted and does not survive app exit.
  - Rationale: persisting it would require growing `EpochRecord` from 8 to 12
    bytes. `EpochRecord` has **no version field** (unlike `NightSummary`), so
    that pulls in next-action 8's blast radius. Out of scope for a one-change
    session.
  - **Accepted consequence:** if the app is killed mid-night, the re-decision
    pass cannot run for that session. The stored per-minute stages remain the
    live-BASE ones. This is a known, documented limitation, not a defect.
- **Memory.** Per-minute variance array capped at **960 entries** (16 hours) x
  4 bytes = 3840 bytes, degrading gracefully past the cap rather than
  overrunning - the same bound and the same failure discipline the smoothing
  spec section 7 sets. `s_base_samples` grows from 24 to 160 entries = 640
  bytes (up from 96). Total added static footprint about 4.4 KB, against a
  measured app footprint of 11,115 bytes and 119,957 bytes free heap.
- **`prv_base_median()` sort index type must widen.** It currently uses
  `int8_t j`, which overflows above 127 elements. With the cap at 160 this
  **must** become `int16_t`. The scratch array `tmp` becomes `static` to keep
  640 bytes off the stack.
- **Fixed-point / integer only.** No floats. Unchanged from existing practice.
- **Single commit** on `~/recovery` `main`: `git commit -s` plus the
  `Co-Authored-By: Claude <noreply@anthropic.com>` trailer, both counts
  verified = 1, `karthakon` identity, before push.

---

## 5. What N13 can and cannot falsify

**N13 is the first night on this change. It opens a NEW comparability group.**
N10-N12 was the last group of the old code.

### N13 CANNOT falsify the core claim

The claim is about **spread**, and **a single night has no spread.** N13
establishes the first anchor of the new group and nothing more. The spread
test requires **N13 plus at least one further unchanged night**, and a fair
comparison against the old group's 3.81x requires **three**, matching N10-N12.

Registering this explicitly, in advance, so that a single striking N13 BASE
value is not over-read in either direction.

### N13 CAN falsify these

1. **The change is live.** BASE `/n` count reads substantially above 24
   (expect about 90-110 on a full night). If it reads 24, the change did not
   take effect and nothing else on the night is interpretable.
2. **The cap does not bind.** `/n` count is strictly below 160.
3. **The re-decision pass runs and does something.** Light and REM totals are
   internally consistent with `rec_classified_min` = Awake + REM + Light
   (OSrest excluded), as on every prior night.
4. **No regression in the untouched lines.** P2/P4/P5/P6 behave as before -
   see section 6.

### The honest null

**If BASE spread across the new group turns out to be comparable to 3.81x,
the cap was not the cause**, and the hypothesis in section 0 is falsified. In
that case the next diagnosis is upstream of sampling - `hrv_ppi_variance`
itself, the 400-beat window's uneven time-sampling (next-action 19), or
genuine night-to-night physiological variability in PPI variance. **That
outcome is a real result and must be reported as one, not explained away.**

---

## 6. Standing lines carried into N13

| Line | Status under this change |
|---|---|
| **P2 Awake %** | **Expected UNCHANGED in mechanism.** Awake is not touched. Any movement is night-to-night variation, not an effect of this change. Band as before. |
| **P3 REM structure** | **Expected to MOVE.** REM is re-decided against a different anchor. Episode count and REM total may both shift. **A shift is not a regression** - there is no ground truth to regress against. Judge on structure (consolidated bands, not confetti), per smoothing spec section 8. |
| **P3b D=R anomaly** | Recurrence watch only. Did not recur on N12; priority not raised. |
| **P4 Battery %/hr** | **Expected unchanged.** The added work is a stop-time pass over at most 960 entries plus a slightly larger median sort. Negligible against an 8-hour recording. A material battery change would be a surprise worth investigating. |
| **P5 Rejection %** | **Expected unchanged.** `hrv_buf_add` and both gates are untouched. |
| **P6 IDLE HR-PPI sign** | **Expected unchanged.** Unrelated code path. Seven consecutive positive nights standing. |
| **P7 mean_bpm** | **Expected unchanged in mechanism.** Derived night-wide from `rec_beats`; not BASE-dependent. Context only, loose. |

**P1 is replaced for the new group.** The old P1 (does BASE reproduce across
identical-classifier nights) becomes the group-level test described in
section 5, and cannot be scored on N13 alone.

---

## 7. Validation of the change itself

Judge the change on section 5's four falsifiable N13 lines first. **If line 1
fails - `/n` still reads 24 - stop and diagnose the build/install before
interpreting any other number on the night.**

The core spread claim is judged at group close, not on N13.

**If spread narrows materially versus 3.81x:** the cap hypothesis is
supported, and next-actions 18 and 25 close together.

**If spread does not narrow:** see section 5's honest null. **Do not retune
the thresholds in response.** They are not what changed, and changing a second
thing in response to the first change's result is how a comparability group
gets destroyed.
