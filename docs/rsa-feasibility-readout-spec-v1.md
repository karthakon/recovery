# rsa-feasibility-readout-spec-v1

**INSTRUMENT ONLY. NO CLASSIFIER INPUT, NO STAGE DECISION, NO TOTAL, NO STORED FIELD, NO
PERSISTED STRUCTURE.** `CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION` do NOT change and MUST
be VERIFIED UNCHANGED after implementation, not asserted.

**THIS SPEC DOES NOT SPECIFY RSA. IT SPECIFIES THE MEASUREMENT THAT DECIDES WHETHER RSA IS
ATTEMPTABLE AT ALL.** Interpolation, windowing, detrending and spectral method are OUT OF
SCOPE and are not drafted here. **A later spec may specify them; it is BLOCKED ON THIS ONE
READING.**

**WORK-GATE, RECORDED BECAUSE IT IS THE REASON THIS FILE EXISTS FIRST:** `classifier-spec-v6`
is HELD until RRV is available or PROVEN IMPOSSIBLE. **"Proven impossible" must be a finding,
not a feeling** — section 7 registers in advance what readings would establish it.

## §1 — Why this instrument exists, read from source at `093edf8`

**All line numbers are STALE BY CONSTRUCTION. RE-GREP BEFORE RELYING ON ONE.**

RSA recovers respiration from the modulation of beat-to-beat intervals. Standard pipelines
interpolate RR values onto a regularly sampled time vector before spectral analysis;
`hrv-cadence-readout-spec-v1` §8 registered that Recovery has no resampling step and that any
on-device RSA work inherits the requirement.

**THE SOURCE READ FOUND SOMETHING §8 DID NOT ANTICIPATE, AND IT IS WORSE THAN A MISSING
RESAMPLING STEP.**

1. **`HrvBuffer` retains the raw beat series.** `hrv_math.h` line 8 declares
   `uint16_t ppi[HRV_BUF_MAX]` with `HRV_BUF_MAX` 400. **The individual intervals survive**,
   which is why `hrv_rmssd`, `hrv_sdnn` and `hrv_mad2` can derive from it after the fact.
   **The beat series RSA needs is ALREADY IN RAM.**
2. **The buffer SLIDES, it does not pin.** `hrv_math.c` 39–44 appends while `count` is below
   the maximum and otherwise `memmove`s down one and writes the newest at the end. **The
   header comment at line 16 saying `count` pins is about `count`, not about data retention.**
3. **NO PER-BEAT TIMESTAMP IS STORED.** `hrv_buf_add` takes `now` and uses it ONLY for the
   freshness test, then writes it to the single `last_accepted_time` field (`hrv_math.c`
   28–29, 46). **The array holds intervals. The time axis is NOT retained.**
4. **THEREFORE ANY RSA MUST RECONSTRUCT ITS TIME AXIS BY CUMULATIVELY SUMMING THE INTERVALS.**
   That is standard practice and is not itself the problem.
5. **THE PROBLEM: A REJECTED BEAT LEAVES NO GAP MARKER.** The range gate (`ppi_ms < 300 ||
   ppi_ms > 2000`) and the jump gate (`diff * 5 > last_accepted`) DISCARD intervals at
   `hrv_math.c` 20–38. Nothing records that a discard occurred at that position. **A cumulative
   sum therefore treats the interval after a discarded one as if it followed immediately.**
   **A REJECTED BEAT BECOMES A TIMING ERROR IN THE RECONSTRUCTED AXIS, NOT A MISSING SAMPLE.**
6. **THE CODE ALREADY KNOWS WHERE THE SERIES IS DISCONTINUOUS AND DOES NOT RECORD IT.**
   `HRV_STALE_SEC` is **10** (`hrv_math.h` line 5). When more than 10 seconds separate accepted
   intervals, `hrv_buf_add` SKIPS the jump check and resets its reference, because the two
   beats are not consecutive. **That test identifies a real discontinuity and its outcome is
   discarded.**

**`HRV_STALE_SEC` 10 IS A DIFFERENT CONSTANT FROM `HR_STALE_SEC` 180 in `main.c` line 74.**
They govern unrelated things and differ by an order of magnitude. **RULE 9. This spec always
writes `HRV_STALE_SEC` in full and never a bare "stale".**

## §2 — The quantity this instrument measures

**WHETHER A TIME AXIS RECONSTRUCTED FROM THE ACCEPTED INTERVAL SERIES IS SOUND ENOUGH FOR
SPECTRAL ANALYSIS.**

**IT DOES NOT MEASURE RESPIRATION. IT DOES NOT ESTIMATE A RESPIRATORY RATE. IT COMPUTES NO
SPECTRUM.** It measures the damage the existing gates do to the series' timing, which is a
prerequisite question and is answerable without any of that.

## §3 — WHICH BUFFER, and why the choice is not free

**READ FROM SOURCE — `main.c` 386–390 calls `hrv_buf_add` on THREE buffers with the same
interval, and EACH maintains its OWN `rejected`, `rej_range`, `rej_jump`, `last_accepted` and
`last_accepted_time`.**

**THE GATES ARE THEREFORE PER-BUFFER, NOT PER-EVENT.** The same interval can be accepted into
one buffer and rejected by another, because each carries its own reference. **"The rejection
count" has THREE answers that legitimately differ and a counter that does not name its buffer
is meaningless.**

- **`s_live_buf`** — UNGATED, runs outside recording. **WRONG: it includes intervals belonging
  to no session.**
- **`s_minute_buf`** — reset every minute. **WRONG: its accept/reject references restart each
  minute, so it cannot see a discontinuity that spans a minute boundary.**
- **`s_night_buf`** — gated on `s_recording`, accumulates across the session. **THIS IS THE
  ONE.** Its decisions correspond to the session-long series RSA would reconstruct.

**ALL COUNTERS IN §4 ARE DEFINED OVER `s_night_buf` AND OVER NOTHING ELSE.** A later session
changing the buffer changes the quantity and must supersede this section by a dated file.

## §4 — The counters

All session-scoped, all `uint32_t`, all gated on `s_recording` exactly as
`hrv-cadence-readout-spec-v1-recording-gate-correction-2026-08-20.md` §2 requires, and all
reset in `prv_start_recording`.

| Label | Meaning |
|---|---|
| `Gp` | count of accepted intervals whose predecessor was REJECTED — the discard-adjacent positions |
| `Gs` | count of accepted intervals arriving more than `HRV_STALE_SEC` after the previous accepted one |
| `Gmx` | longest run of CONSECUTIVE accepted intervals with no discard and no `HRV_STALE_SEC` gap, in beats |
| `Gn` | number of such clean runs |

**`Gp` REQUIRES A NEW FLAG, NOT A NEW GATE.** `hrv_buf_add` already returns `false` on
rejection (`hrv_math.c` 22, 36). The caller at `main.c` 388–389 discards that return value.
**The flag records the PREVIOUS call's return, and NOTHING ABOUT EITHER GATE CHANGES.**

**REGISTERED IDENTITY: `Gn` is at most `Gp + Gs + 1`.** A clean run ends only at a discard-
adjacency or a gap, and the final run needs no terminator. **A violation means a counter is
misplaced and is a defect in the instrument, not a finding about the signal.**

**NO OTHER COUNTER IS ADDED.** In particular no counter is added to `s_live_buf` or
`s_minute_buf`, and the existing `rej`, `rng` and `jmp` figures on RESULTS are NOT touched,
NOT relabelled and NOT reinterpreted.

## §5 — Render

**DIAG 3, TWO NEW LINES, appended below the HRV cadence lines:** `Gp <n>  Gs <n>` and
`Gmx <n>  Gn <n>`.

**DIAG 3 WILL THEN RENDER NINE LINES.** It rendered seven at `093edf8`, verified on the watch
2026-08-20. **NINE MUST BE VERIFIED ON THE WATCH WITH NOTHING CLIPPED AT EITHER END BEFORE ANY
NIGHT IS RECORDED**, and the DIAG 2 precedent shows nine fits at `GOTHIC_14`.

**THE UNDEFINED GUARD KEYS ON `s_session_start`**, identical to `hrv-cadence-readout-spec-v1`
§5. Zero prints `--` and NEVER `0`, per `measurement-spec-v1` §3.6. **A DEFINED `Gp` OF 0 IS A
REAL ZERO AND IS A FINDING** — no discard-adjacent positions — exactly as a defined `SrN` with
`Sr5 0` is.

**NO FRACTION AND NO RATE IS RENDERED.** Both inputs of every ratio are on the screen and
`rec_duration_min` supplies duration, so deriving at scoring time is not a RULE 6 violation —
the same reasoning the anchor and stillness readouts give.

## §6 — What this instrument CANNOT say

**WHETHER RSA WOULD WORK.** A sound time axis is necessary and NOT sufficient. The modulation
must also be strong enough to detect, the PPG noise floor must permit it, and the spectral
computation must fit on the watch. **NONE of those is measured here.**

**WHETHER THE DISCARDED BEATS WERE ARTEFACT OR SIGNAL.** The gates were derived for HRV
statistics, not for timing reconstruction. **A high `Gp` does NOT mean the gates are wrong**
and is NOT grounds to move the range band, the jump multiplier or `HRV_STALE_SEC`.

**WHAT THE RESPIRATORY RATE IS.** No spectrum is computed.

**THE CEILING ON THE WHOLE DIRECTION, REGISTERED SO IT IS NOT DISCOVERED LATE:** the RSA
literature this project has cited used dedicated airflow or respiratory-effort channels.
**Wrist-derived respiration is reported around 0.5 to 1.5 breaths per minute error with watch
fit as the largest error source.** Recovery has a wrist PPG and a fit that varies nightly.
**A later session must not describe wrist RSA as equivalent to the cited work.**

**AND RSA IS NOT A NEW SIGNAL.** It is a different feature of the SAME inter-beat intervals
that already feed `hrv_rmssd` and `hrv_mad2`. **`s_epoch_hf` IS NOT HIGH-FREQUENCY HRV POWER**
— `main.c` 561 and 573 compute it as `60000 / mean_ppi`, a per-minute HEART RATE in bpm, and
it is what c2 and T2 both read. **RSA WOULD BE THE FIRST SPECTRAL QUANTITY IN THE SYSTEM**,
and a later session must not describe it as a variant of an existing one.

## §7 — THE EXIT CONDITIONS, registered BEFORE the first reading

**NO BAND IS PLACED ON ANY OF THE FOUR VALUES FOR THEIR FIRST NIGHT**, following
`awake-runs-readout-spec-v1` §9, the same clause in `stillness-run-readout-spec-v1`, and
`hrv-cadence-readout-spec-v1` §6. **THE NIGHT AFTER THEY FIRST READ MAY BAND THEM.**

**INTERPRETIVE GUIDANCE, EXPLICITLY NOT A CRITERION, AND IT SCORES NOTHING:**

- **`Gmx` LARGE relative to a respiratory period** — at sleeping heart rate a breath spans
  roughly 3 to 6 beats, and a spectral window needs many periods. Clean runs of hundreds of
  beats would mean the axis is reconstructable over analysable spans.
- **`Gmx` SMALL with `Gn` LARGE** — the series is shredded into fragments too short to
  analyse, and **THAT IS THE PROVEN-IMPOSSIBLE FINDING IN ITS CLEAREST FORM.**
- **`Gs` SUBSTANTIAL** — the stream is not continuous at beat resolution for long stretches,
  which contradicts N27's finding of one event per beat and would need diagnosis before
  anything else is read.
- **`Gp` HIGH AND CLUSTERED rather than scattered** — clustering is the damaging pattern,
  because a burst of discards puts a large localised error in the axis where scattered ones
  put small distributed ones. **`Gmx` and `Gn` together are what distinguish the two; `Gp`
  alone cannot.**

**WHAT WOULD PROVE IT IMPOSSIBLE, STATED SO THE HOLD HAS AN EXIT:** clean runs too short to
contain enough respiratory periods for any spectral method, on repeated unconfounded nights.
**THAT IS A MEASUREMENT AND NOT AN ARGUMENT.** **A hold with no falsifiable exit would block
`classifier-spec-v6` indefinitely, which is the defect class RULE 13 names.**

**WHAT WOULD CLEAR IT:** clean runs long enough to analyse, on repeated unconfounded nights.
**CLEARING THIS INSTRUMENT DOES NOT MAKE RSA WORK.** It permits the next spec to be drafted
and NOTHING MORE.

## §8 — Rule 2 and Rule 3 checks

**RULE 2 — NO NEW CONSTANT ENTERS THE CODE.** `HRV_STALE_SEC` 10 is EXISTING and is READ, not
introduced, and this spec MUST NOT MOVE IT. The range band, the jump multiplier and
`HRV_BUF_MAX` are likewise read and unmoved. **The "3 to 6 beats per breath" in §7 is
ARITHMETIC IN THE GUIDANCE, NOT A CONSTANT IN THE CODE**, and no decision reads it.

**RULE 3 — this spec registers NO SCORED CRITERION**, so there is nothing to check for
vacuity. **The four values are RECORDED, NOT SCORED, on their first reading.** Every quantity
in §7's guidance is rendered by a screen the night will capture, and §4's identity is checkable
from the four rendered values alone.

**CHECKED AGAINST THE UNEVALUABLE-CHECK DEFECT:** `Gp`, `Gs`, `Gmx` and `Gn` are all on DIAG 3,
which is captured live in the stop sequence. **They key on RAM statics and DIE WITH THE
SCREEN** — captured live or lost, exactly as the cadence and stillness values are.

## §9 — Comparability

**INSTRUMENT ONLY.** No classifier input, no stage decision, no total, no stored field, no
persisted structure. **`CLASSIFIER_SERIES` STAYS 14 AND `NIGHT_SUMMARY_VERSION` STAYS 3, TO BE
VERIFIED BY READING `src/c/storage.h` AFTER IMPLEMENTATION AND NOT ASSERTED.**

**THE FIRST NIGHT UNDER THIS INSTRUMENT REMAINS COMPARABLE TO N26 AND N27 ON EVERY STAGE
LINE.** The `stage_comparability_key`, `base_comparability_key` and `rri_comparability_key` are
unaffected.

**ONE COST IS REGISTERED HONESTLY: this adds a flag write and four counter increments in the
HRV event handler, which runs once per beat.** **NO PREDICTION IS MADE about its power cost and
NONE MAY BE INFERRED FROM ONE NIGHT** — P4's band and battery's integer-percent resolution are
unchanged by this spec.

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**

**NOTHING IS IMPLEMENTED FROM THIS FILE UNTIL IT IS FROZEN AND PUSHED.**
