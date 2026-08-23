# onwatch-timing-readout-spec-v1

**INSTRUMENT ONLY. NO CLASSIFIER INPUT, NO STAGE DECISION, NO TOTAL, NO STORED FIELD, NO
PERSISTED STRUCTURE.** `CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION` do NOT change and MUST
be VERIFIED UNCHANGED by reading `src/c/storage.h` after implementation, not asserted.

**THIS SPEC DOES NOT SPECIFY RRV AND IT DOES NOT SPECIFY A SPECTRAL METHOD.** It specifies the
measurement that establishes whether a Lomb-Scargle periodogram of the shape the RRV pipeline
would need FITS IN THE COMPUTE BUDGET THIS WATCH HAS. That is one of the three registered
gates on drafting the pipeline spec, and it is the only one of the three that requires NO
NIGHT.

**WHY IT EXISTS:** `hrv-resolution-readout-spec-v1` §2.1 registers three preconditions on the
RRV pipeline draft. Resolution is measured by that instrument and read on N29. Whether the
spectral computation fits on the watch is recorded there as **UNREAD AND UNMEASURED**. This
file closes it by measurement rather than by estimate.

## §1 — What the source read established, and where it stopped

**Read 2026-08-23. All line numbers are STALE BY CONSTRUCTION. RE-GREP BEFORE RELYING ON ONE.**

**FLOATS ARE NOT ABSENT FROM THE PLATFORM. THEY ARE EMULATED.** The SDK's own doc comment on
`difftime` at `~/.local/share/pebble-sdk/SDKs/4.33.1/sdk-core/pebble/emery/include/pebble.h`
states that Pebble uses SOFTWARE FLOATING POINT EMULATION and that including a function
returning a double SIGNIFICANTLY INCREASES BINARY SIZE. **THIS IS THE BASIS FOR THE
INTEGER-ONLY REQUIREMENT AND IT IS STRONGER THAN THE ABSENCE OF FLOATS FROM `~/recovery`** —
the earlier grep over `src/c/` returned three hits and **ALL THREE ARE COMMENTS**, which
establishes only that this codebase has not used floats, not that it must not.

**FIXED POINT AT SCALE IS ALREADY PROVEN ON THIS DEVICE.** `smoother.c` runs a three-state
Viterbi lattice over up to `MAX_EPOCHS` 960 epochs using `int16_t` log2-probabilities scaled
by 256, with `TRANS`, `EMIT_HIT`, `EMIT_MISS` and the REM-latency ladder **ALL PRECOMPUTED AS
CONSTANTS AND NOTHING TRANSCENDENTAL EVALUATED AT RUNTIME.** The inner loop is add-and-compare.

**THE HEAVIEST ARITHMETIC CURRENTLY SHIPPING IS `prv_isqrt`** — Newton's method with an
integer division inside the loop, `hrv_math.c`. It is called from `hrv_rmssd` and `hrv_sdnn`,
which are per-minute, not per-beat.

**`uint64_t` ACCUMULATION IS ALREADY IN USE** — `sumsq` in `hrv_rmssd` and
`hrv_ppi_variance`.

**AN 800-BYTE STACK FRAME ALREADY DID NOT FIT.** `hrv_math.c` uses a file-static scratch for
`hrv_mad2` and its comment states `HRV_BUF_MAX` `uint16_t` is too large for a stack frame on
this platform. **ANY DESIGN THAT NEEDS A SECOND 400-ELEMENT ARRAY INHERITS THAT PROBLEM.**

**NO PER-BEAT TIMESTAMP IS STORED.** `hrv_buf_add` writes only `last_accepted_time`, so a time
axis must be reconstructed by cumulatively summing intervals — `rsa-feasibility-readout-spec-v1`
§1.

**WHERE THE READ STOPPED, AND IT IS A FINDING RATHER THAN A GAP:** **THE HCPU CORE FREQUENCY
IS NOT ESTABLISHABLE FROM THE OPEN FIRMWARE SOURCE.** Read at tag `v4.33.2`: `profiler.c`
obtains it on this platform via `HAL_RCC_GetHCLKFreq(CORE_ID_HCPU)`, a RUNTIME HAL CALL into
the SiFli vendor HAL, and no compile-time literal backs it in `src/fw/`. **`board_obelix.h`
CARRIES NO CLOCK DEFINITION.**

**THE 48000000 LITERALS IN `src/fw/drivers/rtc/sf32lb.c` AND `src/fw/drivers/sf32lb52/rc10k.c`
ARE THE HXT48 CRYSTAL USED AS AN RC-CALIBRATION REFERENCE. THEY ARE NOT THE CPU CLOCK AND MUST
NOT BE READ AS IT.** RULE 9. **This is the same error shape as reading
`GH3X2X_HRV_SAMPLING_RATE` 100 as implying 10 ms interval quantisation** — a plausible number,
in a plausible file, measuring a different thing. **A LATER SESSION MUST NOT RESOLVE THE
COMPUTE BUDGET BY ARITHMETIC ON 48 MHz.**

**THEREFORE THE BUDGET IS SETTLED BY WALL-CLOCK MEASUREMENT ON THE DEVICE AND BY NOTHING
ELSE.** That is what this instrument does.

## §2 — The quantity this instrument measures

**THE WALL-CLOCK MILLISECONDS A FIXED, DETERMINISTIC, LOMB-SCARGLE-SHAPED WORKLOAD TAKES TO
RUN ON THE WATCH.**

It measures nothing about respiration, nothing about the classifier, nothing about accuracy,
and **nothing about whether the periodogram it imitates would produce a useful answer.** It
answers one question — **does the arithmetic fit in the time available.**

## §2.1 — What this DOES and DOES NOT decide

**IT DOES NOT DECIDE WHETHER RRV WORKS AND IT DOES NOT DECIDE WHETHER RRV HELPS CLASSIFY
SLEEP.** Those are the two gates `hrv-resolution-readout-spec-v1` §2.1 separates, and this
instrument touches neither. **A WORKLOAD THAT RUNS IN TWO MILLISECONDS IS NOT EVIDENCE THAT
THE SPECTRUM MEANS ANYTHING.**

**IT DOES NOT MEASURE POWER.** A CPU burst has an energy cost and this instrument reports
TIME. **NO POWER PREDICTION MAY BE INFERRED FROM ANY READING HERE**, consistent with the same
clause in the RSA and resolution specs. P4's band is unchanged by this spec.

**IT DOES NOT ESTABLISH THE CORE FREQUENCY** and must not be used to back-derive it. A time is
a time.

## §3 — What is timed, and why the workload is synthetic

**THE TIMED WORKLOAD USES A DETERMINISTIC SYNTHETIC INTERVAL SERIES, NOT `s_night_buf`.**

**THE REASON IS COMPARABILITY.** A timing figure must be reproducible on demand and must mean
the same thing on two different days. `s_night_buf` holds whatever the last 400 accepted beats
were, its `count` may be below `HRV_BUF_MAX`, and it is empty outside a session. **A timing
run against it would be a different workload every time and could not be compared to
itself.**

**THE SYNTHETIC SERIES IS GENERATED IN PLACE, NOT STORED.** A 400-element `uint16_t` array is
800 bytes, which §1 records as too large for a stack frame — **so the generator is an
arithmetic function of the index, evaluated inside the loop, and NO NEW ARRAY IS
ALLOCATED.** The values need only be plausible interval magnitudes and identical between runs.
**They are NOT required to be physiological and NOTHING may be concluded from them.**

**THE GENERATOR MUST COST ONE OR TWO CYCLES AND MUST USE NO DIVISION AND NO MODULO. THIS IS A
REQUIREMENT, NOT A PREFERENCE.** In the real pipeline the next interval is a near-instant array
read from `ppi[]`. **A generator inside the timed loop is timed ALONGSIDE the periodogram
arithmetic, so an expensive one INFLATES `Tf` AND WOULD PRODUCE A FALSE STOP** — the spec would
report that the maths does not fit when what did not fit was the scaffolding. Integer
multiply-and-mask or shift-and-add only, for example `base + ((i * 13) & 0x7F)`. **DIVISION AND
MODULO ARE EXPENSIVE ON THIS CORE — `prv_isqrt` is the only divide currently shipping in a loop
and §1 records it as the heaviest arithmetic in the codebase.**

**THE TRIGONOMETRY MUST COME FROM THE SDK's INTEGER LOOKUP AND MUST NOT COME FROM `<math.h>`.**
**READ FROM THE HEADER 2026-08-23:** `int32_t sin_lookup(int32_t angle)` and
`int32_t cos_lookup(int32_t angle)` are declared, with `TRIG_MAX_ANGLE` `0x10000` for a full
turn and `TRIG_MAX_RATIO` `0xffff` as the largest returned value. **`cos_lookup(a)` is
documented as equivalent to `sin_lookup(a + TRIG_MAX_ANGLE / 4)`.**

**THIS IS MANDATED RATHER THAN LEFT OPEN BECAUSE THE OBVIOUS ALTERNATIVE IS A TRAP.** Calling
`sin()` from `<math.h>` and casting the result to an integer **PULLS IN THE SOFTWARE FLOAT
EMULATION §1 EXISTS TO AVOID**, inflating both binary size and execution time — and it would do
so invisibly, since the code would compile and produce correct-looking numbers. **A CUSTOM
TABLE IS NOT REQUIRED AND SHOULD NOT BE WRITTEN.** An earlier draft of this spec assumed one
was needed; **the platform already ships the lookup and that assumption was WRONG.**

**A FULL TURN OF 0x10000 IS WHY THE PHASE ACCUMULATOR IS FREE.** Phase may be carried in a
`uint32_t` and reduced with a mask of `0xFFFF` rather than a division or a conditional — **the
modular reduction that normally costs something is a bitwise AND here.** The returned scale is
`int32_t` against `TRIG_MAX_RATIO`, **NOT `int16_t`**, and the accumulator width must be chosen
against that.

**THE WORKLOAD IS THE INNER LOOP ONLY.** Per frequency, over `N` points, the periodogram needs
the four running sums that Lomb-Scargle requires, with sine and cosine of an accumulated
phase. **That is what is timed.** Windowing, detrending, normalisation and peak-picking are
OUT OF SCOPE and are not timed, because they are O(N) or O(F) once and are dominated by the
O(N x F) inner loop.

## §4 — The counters

All session-independent, all `uint32_t`, all computed on demand when the timing run is
triggered, all reset at the start of each run.

| Label | Meaning |
|---|---|
| `Tf` | milliseconds for `TIME_REPS` repetitions of the inner loop at `TIME_N` points, ONE frequency |
| `Tt` | milliseconds for the trig-only path over the same point count and repetitions, no sums |
| `Tn` | the point count actually used, `TIME_N` |
| `Tr` | the repetition count actually used, `TIME_REPS` |

**`Tf` AND `Tt` ARE TIMES, NOT COUNTS, AND PARTICIPATE IN NO SUBSET IDENTITY.**

**`Tt` EXISTS TO SEPARATE THE TWO COSTS.** If the workload is dominated by the sine table and
the phase multiply, that is a different optimisation problem from being dominated by the
multiply-accumulates. **Reporting only a total would hide which.** `Tf` includes the trig, so
**`Tf` is expected to exceed `Tt`; `Tt` exceeding `Tf` is an INSTRUMENT DEFECT and the reading
is VOID.**

**THE DERIVED QUANTITIES ARE COMPUTED AT SCORING TIME AND NEVER TYPED (RULE 6):** per-frequency
cost is `Tf / Tr`; a full periodogram of `F` frequencies is `F x Tf / Tr`. **`Tr` IS RENDERED
RATHER THAN ASSUMED PRECISELY SO THAT DIVISION IS CHECKABLE — IF `Tr` RENDERS 0 THE RUN DID NOT
COMPLETE, NO DERIVED QUANTITY MAY BE COMPUTED FROM IT, AND THE READING IS VOID.** A run that
aborts or a `TIME_REPS` edited to 0 during testing must not reach a divide. **Both inputs are
rendered, so deriving the quotient at scoring time is not a RULE 6 violation** — the same
reasoning the anchor, stillness, cadence, RSA and resolution readouts give.

**`TIME_REPS` EXISTS BECAUSE THE CLOCK IS COARSER THAN THE WORKLOAD.** `time_ms` resolves to 1
ms and a single inner loop may fall below that. **Repetition is what makes the quantity
measurable and it is NOT a tuning parameter** — it is divided back out at scoring time.

## §4.1 — The elapsed-time computation, and the defect it avoids

**READ FROM THE SDK DECLARATION 2026-08-23:** `uint16_t time_ms(time_t *tloc, uint16_t
*out_ms)` returns the **MILLISECONDS PORTION** of Unix time and optionally writes the seconds
portion through `tloc`.

**THE MILLISECONDS PORTION WRAPS EVERY SECOND. AN ELAPSED TIME COMPUTED FROM `out_ms` ALONE IS
WRONG ONCE PER SECOND.** Elapsed MUST be computed from BOTH parts.

**THE MANDATED FORM, AND THE CASTS ARE PART OF THE REQUIREMENT:**

```
int32_t elapsed = ((int32_t)s_end - (int32_t)s_start) * 1000
                + ((int32_t)ms_end - (int32_t)ms_start);
```

**WHY THE CASTS ARE MANDATED, STATED PRECISELY SO A LATER SESSION DOES NOT RECORD THE WRONG
HAZARD:** two `uint16_t` operands are PROMOTED TO `int` before subtraction on this platform,
because `int` is 32-bit and represents every `uint16_t` value, **so a bare `ms_end - ms_start`
on `uint16_t` variables is ALREADY SIGNED AND ALREADY CORRECT.** **THE HAZARD IS THE STORAGE
TYPE, NOT THE SUBTRACTION.** The natural implementation stores the milliseconds beside a
`time_t` and reaches for `uint32_t`; **at `uint32_t` the promotion does not happen, `100 - 900`
evaluates to 4294966396, and the elapsed time is wildly wrong.** The explicit `int32_t` casts
foreclose that whatever the storage type, which is why they are required rather than
recommended.

**THIS IS REGISTERED BECAUSE IT WOULD PRODUCE A PLAUSIBLE WRONG NUMBER**, not a crash, and a
sub-second workload would appear correct in most runs and absurd in a minority.

## §4.2 — THE OPTIMIZER WILL DELETE AN UNUSED LOOP, AND THAT IS THE LARGEST THREAT TO THIS INSTRUMENT

**A LOOP WHOSE RESULT IS NEVER USED HAS NO OBSERVABLE SIDE EFFECT AND MAY BE REMOVED ENTIRELY
BY THE COMPILER.** The SDK builds at `-Os`. **`Tt` IS THE MOST EXPOSED VALUE IN THIS SPEC**
because the trig-only path exists precisely to compute values it then discards — **which is
the exact signature of dead code.** `Tf` is exposed the same way if its four running sums are
never read.

**THE FAILURE MODE IS NOT A CRASH AND NOT AN OBVIOUS ZERO.** It is `Tt` reading 0, 1 or 2 ms —
bare loop overhead — **which looks like a fast trig path and would be reported as one.** It
would then be carried into the pipeline spec as evidence that the sine lookup is cheap. **THAT
IS A PLAUSIBLE WRONG NUMBER OF THE WORST KIND: it is in the expected direction and has an
appealing explanation.**

**THE REQUIREMENT:** the terminal accumulator of EACH timed workload MUST be written to a
`volatile` sink after the timed region, or otherwise consumed in a way the compiler cannot
prove is dead. **THE SINK IS WRITTEN AFTER THE CLOCK IS READ SO IT DOES NOT ENTER THE
MEASUREMENT.** A `static volatile uint32_t` at file scope is sufficient and costs four bytes.

**VERIFICATION IS MANDATORY AND IS NOT SATISFIED BY THE VALUE LOOKING REASONABLE.** `Tt` must
be checked to SCALE WITH `TIME_REPS` — doubling `TIME_REPS` must roughly double `Tt`. **A `Tt`
that does not move when `TIME_REPS` moves HAS BEEN OPTIMISED AWAY, and the reading is VOID.**
This is a RULE 11-shaped check applied to behaviour rather than to strings — **a clean build
does not prove the loop survived it.**

## §5 — Render, and when it may run

**A NEW SCREEN, DIAG 5, FOUR LINES:**

```
Tf <n>   Tt <n>
Tn <n>   Tr <n>
```

**HOW IT ATTACHES:** exactly as DIAG 4 attached to DIAG 3 and DIAG 3 attached to DIAG 2 — a
`MODE_DIAG5` enum member, a DOWN handler on `MODE_DIAG4`, and a new `case MODE_DIAG5`
subscribing UP and BACK back to `MODE_DIAG4`. **NO EXISTING NAVIGATION IS RESTRUCTURED.**

**THE NINE-LINE CEILING IS NOT APPROACHED.** DIAG 5 renders two lines of values plus its
title.

**THE RUN IS TRIGGERED BY SELECT ON DIAG 5 AND NEVER AUTOMATICALLY.** Values print `--` until
a run has completed.

**IT MUST NOT RUN WHILE `s_recording` IS TRUE. THIS IS THE ONE HARD GUARD IN THIS SPEC.** A
multi-hundred-millisecond CPU burst during a night perturbs the very thing every other
instrument is measuring, and the capture sequence walks through the DIAG screens AT STOP while
the session statics are still live. **SELECT ON DIAG 5 WHILE `s_recording` IS TRUE MUST NOT
RUN THE WORKLOAD.** Recorded plainly because the natural place to press it is precisely the
wrong place.

**IT MUST NOT FAIL SILENTLY EITHER. WHEN THE GUARD BLOCKS A RUN, THE VALUE LINES RENDER `REC`
IN PLACE OF `--`.** A press that produces no change at all is indistinguishable from a dead
button, a wedged app or a handler that never fired, **and a tester who cannot tell those apart
will press it again during a session, which is the one thing this guard exists to prevent.**
`REC` costs no memory and confirms the guard fired. **`--` THEREFORE MEANS NOT YET RUN AND
`REC` MEANS REFUSED WHILE RECORDING — TWO DIFFERENT STATES THAT MUST NOT SHARE A RENDER**
(RULE 9).

**THE CAPTURE SEQUENCE IS NOT CHANGED BY THIS SPEC.** DIAG 5 carries no night data, is not
part of any pre-registered line, and **IS NOT A NINTH CAPTURE.** The walk back up the chain
becomes six presses only if a session has walked down to DIAG 5, which the capture sequence
does not do.

**RULE 11 APPLIES:** the four new labels are verified by running `strings` on
`pebble-app.elf` and finding each in both its defined and undefined variant.

## §6 — The reading, registered BEFORE the first run

**NO BAND IS PLACED ON `Tf` OR `Tt`.** What follows is arithmetic against an EXISTING cadence
already in the code, not a constant fitted to a measurement.

**THE BUDGET REFERENCE IS THE EXISTING MINUTE CADENCE.** `prv_close_minute` runs once per
minute and the UI timer is registered at 1000 ms. **A periodogram computed once per window on
a minute cadence has a whole minute of wall clock available and shares it with a UI tick.**
**60000 ms IS READ FROM THE EXISTING STRUCTURE AND IS NOT A NEW CONSTANT**, and no decision
reads it — it is arithmetic in this guidance only.

**THE INTERPRETIVE BRANCHES, EXPLICITLY NOT SCORED:**

- **A full periodogram derived at well under 1000 ms** — the computation is not the
  constraint. **This is the expected outcome given that Viterbi over 960 epochs already
  runs.**
- **A full periodogram in the low seconds** — it fits a once-per-minute cadence but not a
  once-per-second one, and the pipeline spec must size its window and cadence around that.
  **That is a design input, not a stop.**
- **A full periodogram in TENS of seconds or more** — it does not fit a minute cadence with
  margin. **THAT IS THE STOPPING REGIME FOR THIS GATE**, and the response is to reduce `F`,
  reduce `N`, or reconsider the method — **NOT to conclude RRV is impossible**, since this
  gate is about arithmetic and not about signal.
- **`Tt` close to `Tf`** — the workload is trig-bound and a coarser or finer sine table is the
  lever. **`Tt` far below `Tf`** — it is multiply-accumulate-bound and the table size is not
  worth optimising.

**A FAST READING IS NOT A LICENCE.** It settles ONE of the three preconditions in
`hrv-resolution-readout-spec-v1` §2.1 and NOTHING MORE. **RESOLUTION IS STILL UNDECIDED AFTER
N29 AND THAT BLOCK IS UNAFFECTED BY ANY READING HERE.**

## §7 — What this instrument CANNOT say

**WHETHER RRV WOULD WORK.** Compute budget is one input assumption among several.

**WHETHER RRV WOULD HELP CLASSIFY SLEEP.** Nothing in this project has yet tested that.

**WHAT THE CORE FREQUENCY IS.** §1 records that it is not establishable from open source, and
a wall-clock time does not recover it.

**WHAT THE POWER COST IS.** Time is not energy.

**WHETHER THE PERIODOGRAM IS CORRECT.** The synthetic series is chosen for reproducibility and
the sums are never inspected. **A TIMING RUN THAT COMPUTES GARBAGE FAST WOULD READ THE SAME AS
ONE THAT COMPUTES CORRECTLY FAST**, and this instrument cannot tell them apart. **Correctness
is the pipeline spec's problem and is registered there, not here.**

**WHETHER THE COMPILER PRODUCED THE CODE THE SPEC DESCRIBES.** §4.2's scaling check detects a
loop removed ENTIRELY. **It does NOT detect partial optimisation** — hoisting, strength
reduction or vectorisation of part of the body. **A timing figure is a measurement of what the
compiler actually emitted, which is the right thing to measure for a budget question and the
wrong thing to read as a per-operation cost.**

**ANYTHING ABOUT A WORKLOAD OF A DIFFERENT SHAPE.** `Tf` is measured at `TIME_N` points for
one frequency. **Scaling to other `N` assumes the loop is linear in `N`, which is expected but
is NOT measured by a single point.** A later session wanting that must vary `TIME_N` and read
more than one value.

## §8 — Rule 2 and Rule 3 checks

**RULE 2 — TWO NEW CONSTANTS ENTER THE CODE AND BOTH ARE INSTRUMENT PARAMETERS READ BY NO
DECISION.** `TIME_N` is set to `HRV_BUF_MAX` 400 so the timed workload matches the real buffer
length — **it is READ FROM THE EXISTING CONSTANT AND IS NOT AN INDEPENDENT CHOICE.**
`TIME_REPS` is a repetition count whose only requirement is that `Tf` land comfortably above
the 1 ms clock resolution, and **it is DIVIDED BACK OUT at scoring time so its value cannot
influence any derived quantity.** **NEITHER IS A THRESHOLD. NO DECISION READS EITHER.** The
moment any decision reads one, this paragraph stops applying and it must be derived.

**`TRIG_MAX_ANGLE` AND `TRIG_MAX_RATIO` ARE SDK-PROVIDED AND ARE NOT NEW CONSTANTS.** They are
read from the platform header, not chosen by this spec.

**NO EXISTING CONSTANT IS MOVED.** `HRV_BUF_MAX`, `HRV_STALE_SEC`, the range band, the jump
multiplier, every classifier parameter and every divisor in
`hrv-resolution-readout-spec-v1` are read-only to this spec. **`DM_SENTINEL` IS NOT
TOUCHED.**

**RULE 3 — THIS SPEC REGISTERS NO SCORED CRITERION**, with one exception: **`Tt` EXCEEDING
`Tf` IS A CHECKABLE DEFECT CONDITION** and a run MUST check it. A reading that would make it
PASS is any run where the fuller workload takes at least as long as its own subset; a reading
that would make it FAIL is the reverse. **Neither is unreachable.**

**CHECKED AGAINST THE UNEVALUABLE-CHECK DEFECT:** all four values are rendered on DIAG 5, and
**unlike every other instrument in this project they are REPRODUCIBLE ON DEMAND AND DO NOT DIE
WITH A SCREEN.** A lost reading is re-run at no cost. **THIS IS THE ONLY INSTRUMENT HERE WITH
THAT PROPERTY** and it is the direct consequence of the workload being synthetic.

**CHECKED AGAINST THE THIRD VACUITY FORM:** the defect condition can pass and can fail, and §6
names a reading for each interpretive branch. **No line here is a criterion that cannot fail or
one whose failing reading means something other than what it appears to mean.**

## §9 — Comparability

**INSTRUMENT ONLY.** No classifier input, no stage decision, no total, no stored field, no
persisted structure. **`CLASSIFIER_SERIES` STAYS 14 AND `NIGHT_SUMMARY_VERSION` STAYS 3, TO BE
VERIFIED BY READING `src/c/storage.h` AFTER IMPLEMENTATION AND NOT ASSERTED.**

**A NIGHT RECORDED AFTER THIS COMMIT REMAINS COMPARABLE TO N26 THROUGH N29 ON EVERY STAGE
LINE**, provided the guard in §5 holds. **THE GUARD IS WHAT PROTECTS COMPARABILITY** — an
unguarded timing run during a session would inject a CPU burst into a measured night, and that
night would not be comparable to anything.

**NO PER-BEAT COST IS ADDED.** Unlike the RSA and resolution instruments, this one adds
NOTHING to the HRV event handler. It runs only when triggered.

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**

**NOTHING IS IMPLEMENTED FROM THIS FILE UNTIL IT IS FROZEN AND PUSHED.**
