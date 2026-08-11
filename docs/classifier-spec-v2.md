# Classifier spec v2 — the multi-feature REM decision

**Dated 2026-08-11. Frozen BEFORE the night that tests it (N18).**

## 1. Why this exists

`c5edaee` replaced a non-robust dispersion feature with a robust one and N17 read
`L5p 7 / Max 12 / Off 0`, supporting `c5edaee` section 1. What N17 could not establish —
recorded in `nights/C00-N17.md` under "What N17 does NOT establish" — is whether the
identified minutes are *correctly* REM.

A single dispersion statistic cannot express the defining REM signature. REM is sympathetic
dominance **while** skeletal muscle is atonic. Those are different axes; a scalar dispersion
value collapses them.

`cur_hr`, `baseline_hr` and `movement` are already parameters of `sleep_stage_classify()`
(`sleep_stage.c` 13-17), verified by source read, and are used only in the Awake branch
(lines 21-30). **No new sensor, no firmware change, no additional sampling.**

## 2. Scope

**In scope:** the number and identity of terms in the REM decision; the per-minute
persistence of a stillness bit.

**NOT in scope:**

- `D(m)`, `F(m)` and their windowing — **unchanged from `c5edaee` sections 3.1-3.2.**
- The 2.0x dispersion multiplier and the 0.5x Light multiplier — **unchanged.**
- `876a6d6` BASE in every part; `BASE_SAMPLE_MAX 160`; `prv_base_median()`.
- `7f6c250` in any part.
- The Awake branch of `sleep_stage_classify()`.
- Native Deep, the four-state matrix, SpO2, respiration. **Staging stays three-class.**
- `NightSummary`. The v3 bump is a separate spec and a separate commit.

## 3. Definitions

**3.1 Dispersion.** `D(m)`, `F(m)` as `c5edaee` sections 3.1-3.2, unchanged. The anchor `A`
of `c5edaee` section 3.3 is renamed **`A_D`** for disambiguation. **Its computation is
byte-for-byte unchanged** (`prv_compute_anchor`, `main.c` 321-345).

**3.2 Per-minute heart rate.** `H(m)` = `60000 / rec.mean_ppi` for stored minutes with
`mean_ppi > 0`; missing otherwise. Derived at stop from `EpochRecord`. **Not `s_last_hr`** —
that is the live instantaneous reading and is not persisted per minute.

**3.3 Windowed heart rate.** `HF(m)` = median of `H` over the centred window m-2 .. m+2,
missing excluded, truncated at edges. **Identical window shape to `F(m)`** so both terms
have the same temporal support and neither can fire on a single minute.

**3.4 Heart-rate anchor.** `A_H` = median of `HF(m)` over all non-Awake minutes with `HF`
defined. Computed once at stop, upper-middle selection, no averaging.

**3.5 Stillness.** `S(m)` = true when the minute recorded no movement. Persisted live into
`EpochRecord.reserved` (bit 0), written from the existing `movement` boolean at `main.c`
105-107. `reserved` is currently written as constant 0 (line 104) and is read by nothing.

## 4. Decision

Evaluated at stop, inside the **existing** `prv_base_redecide`. For each stored minute
surviving the existing Awake, `beat_count < 20` and zero skips:

**REM** when all three hold:

1. `F(m) > 2 * A_D`
2. `HF(m) > A_H`
3. `S(m)` is true

Otherwise the existing Light rule applies unchanged: Light when `F(m) * 2 >= A_D` and
`F(m) <= A_D * 2`, else Light.

**Awake is never overwritten. The `s_mins` decrement/increment bookkeeping is unchanged.**

### 4.1 Term roles, stated so they are falsifiable

- **T1 dispersion — the candidate generator.** Required. v2 cannot produce a REM minute v1
  would not have produced.
- **T2 heart rate — corroboration.** A median split on the night's own distribution.
  **Not a fitted threshold.**
- **T3 stillness — a veto.** Grounded in atonia. **See section 4.3.**

### 4.2 No parameter is fitted to any night

`A_H` is a within-night median, exactly as `A_D` is. **N17's P-ANCHOR reading licenses no
threshold and none is taken from it.**

### 4.3 Standing assumption — intact REM atonia

T3 treats movement as disqualifying for REM. This is valid **only for a sleeping subject
with intact REM muscle atonia.** In REM sleep behaviour disorder, and in any condition
producing REM without atonia, movement rises during REM rather than falling, and T3 would
systematically suppress exactly the minutes it is meant to find.

**Recovery does not detect this condition and cannot warn about it.** The assumption is not
testable from anything Recovery records.

**This must appear in the release notes and in the README.** Recorded here so the obligation
is carried, not discovered at publication.

## 5. Implementation constraints

1. **One re-decision pass.** `prv_base_redecide` gains two comparands. **It is not rewritten
   and no second pass is added** — `bae23c3` section 4.
2. `HF` requires `s_epoch_var` un-sorted and epoch-indexed, so it is computed **inside or
   before** the re-decision pass, **never after `prv_measure`** — `bae23c3` section 4.
3. `A_H` is computed in `prv_compute_anchor` alongside `A_D`, returned via an
   out-parameter. One added night-scale sort.
4. New static `s_epoch_hf`, **`uint16_t`** `[EPOCH_VAR_MAX]` — 1920 bytes BSS. Half what a
   `uint32_t` array would cost.
5. `EpochRecord` layout, size and version: **unchanged.** Only the value written to
   `reserved` changes. Per the critical ordering constraint only one night of epochs exists
   at a time, so no historical record can be misread.
6. `NightSummary`, storage layout and persist keys: **unchanged by this spec.**
7. Integer arithmetic throughout, `uint32_t`, no float.
8. **No firmware flash.** `pebble install` only. v4.32.0, slot 1.

## 6. What the first night CAN and CANNOT establish

**CAN, one night:** whether the conjunction still emits runs of 5 or more (`L5p > 0`); the
direction and magnitude of the change in REM total against N17's 47 minutes; whether T2 and
T3 are near-independent of T1 or largely redundant with it.

**CANNOT, one night:** whether the identified REM is *correct* — there is no ground truth
and Garmin is not it. Anything about stability, spread, or REM percentage bands. **Nothing
licenses fitting a multiplier.**

**Registered failure modes:** REM falling to 0 with `L5p 0` means the conjunction is
over-restrictive on this hardware and identifies which term to interrogate — **it is not
grounds to touch `7f6c250` or to retune the 2.0x multiplier.** REM essentially unchanged
from 47 means T2 and T3 are redundant with T1 and the conjunction bought nothing.

## 7. DIAG consequence

`Vmax`, `P90`, `Vmed`, `Vn` continue to measure `D(m)`/`F(m)` and are **unchanged in meaning
from `classifier_series 7`** — N17 remains comparable on those columns.

**`Gate` changes meaning.** It counts minutes over `2 * A_D`, which is now T1 alone rather
than the REM decision. **`RemN <= Gate` remains registered and is now expected to be a
strict inequality**; equality would mean T2 and T3 excluded nothing. `RemN > Gate` remains a
genuine failure.

## 8. Comparability

`classifier_series` moves **7 -> 8**. `recovery_commit` moves. **N18 opens a new
comparability group.** N17 is a closed group of one and is not comparable to N18 on any REM
line. **No night N1-N17 is edited or recomputed.**

## 9. Status

**Frozen on commit. Not retunable. A change is a v3 or a dated correction, never an edit.**
