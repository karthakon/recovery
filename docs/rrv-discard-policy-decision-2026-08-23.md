# rrv-discard-policy-decision-2026-08-23

**Resolves the OPEN FORK registered in `rrv-pipeline-spec-v1` §5.6. THAT FILE IS A DRAFT AND
IS NOT YET FROZEN, so this decision is folded into it before freezing rather than superseding
it.**

**RULE 7: no subject physiological value is recorded here. The counts this reasoning rests on
are `Gp`, `Gs`, `Gn` and `Gmx` in `~/recovery-nights/nights/C00-N28.md` and `C00-N29.md`.**

## 1. The fork as drafted, and why it was drawn wrong

§5.6 offered two options — analyse only windows containing no discard adjacency, or analyse
all windows passing §5.5 and accept the axis error. **BOTH ARE BAD, AND THE SECOND IS WORSE
THAN THE DRAFT STATED.**

## 2. What is wrong with CLEAN-ONLY — a yield argument, and it is legitimate

A clean 400-beat window requires a clean run of at least `RRV_N`. Mean clean run length is
computable from the recorded `Gn` and the accepted beat count on each night, **and it differs
by roughly a factor of two between N28 and N29.**

**TAKING RUN LENGTHS AS ROUGHLY EXPONENTIAL — AND THAT IS AN ASSUMPTION, NOT A MEASUREMENT,
BECAUSE THE RUN-LENGTH DISTRIBUTION IS NOT CAPTURED ANYWHERE** — the fraction of positions
that start a fully clean window differs by ABOUT FIVE FOLD between those two adjacent nights.
**The estimate is order-of-magnitude and must not be quoted as a figure.**

**THE METHODOLOGICAL OBJECTION DOES NOT DEPEND ON THE ESTIMATE'S PRECISION.** Under
clean-only, **DATA YIELD BECOMES A FUNCTION OF SIGNAL QUALITY.** A night with more artefact
contributes fewer windows, so `Rn` varies with artefact rather than with physiology, and
cross-night comparison is confounded by the very quantity the gates exist to manage. **THAT IS
AN ARGUMENT ABOUT WHAT THE INSTRUMENT MEASURES, NOT ABOUT WHICH POLICY PRODUCES NICER
NUMBERS**, and §5.6's prohibition is respected.

**THE SLIDING AMPLIFIER MAKES IT WORSE, per §5.6.** A disqualifying event stays resident for
the next `RRV_N` accepted beats, so one discontinuity invalidates six or seven consecutive
overlapping windows rather than one.

## 3. What is wrong with ALL-WINDOWS — and this CORRECTS the draft

**`rsa-feasibility-readout-spec-v1` §1.5 STATES THAT A REJECTED BEAT BECOMES A TIMING ERROR IN
THE RECONSTRUCTED AXIS, NOT A MISSING SAMPLE. THE DRAFT UNDER-READ THAT SENTENCE.**

**A DISCARD DOES NOT PERTURB ONE POINT. THE CUMULATIVE SUM RUNS SHORT BY THE DISCARDED
INTERVAL, SO EVERY SUBSEQUENT `t_i` IN THE WINDOW IS OFFSET, AND THE OFFSETS ACCUMULATE.**

At the discard-adjacency rates recorded on N28 and N29, a `RRV_N`-length window carries several
discards, and the accumulated offset by the end of a window reaches **seconds** against a
window spanning **minutes**. **AT THE RESPIRATORY FREQUENCIES THIS PIPELINE TARGETS, SECONDS OF
ACCUMULATED OFFSET IS AN APPRECIABLE FRACTION OF A CYCLE.** The effect is not graceful
degradation — **it smears the peak the pipeline exists to find**, and it does so more on
exactly the windows that already have more artefact.

**THIS IS REGISTERED AS A CORRECTION TO `rrv-pipeline-spec-v1` §5.6 AS DRAFTED**, which
described the axis error as something to "accept" without stating that it accumulates.

## 4. THE DECISION — option three, which dissolves the tradeoff

**RECONSTRUCT THE TIME AXIS FROM EVERY DELIVERED INTERVAL, AND TAKE VALUES ONLY FROM ACCEPTED
ONES.**

**THE DURATION OF A REJECTED INTERVAL IS KNOWN AT THE CALL SITE.** `hrv_buf_add` returns
`false` on rejection, but `ppi` is still in hand in `prv_health_handler` — **the caller already
captures that return value for `Gp`, read from source.** Nothing new is measured and no gate
changes.

**THE CONSEQUENCE:** a discard becomes **A GENUINE MISSING SAMPLE AT A CORRECT TIME** rather
than a timing error. **THAT IS PRECISELY THE CASE LOMB-SCARGLE EXISTS TO HANDLE** — a
non-uniformly sampled series with gaps — and it is why the method was chosen over anything
requiring resampling.

**BOTH PROBLEMS GO AWAY TOGETHER.** No window is discarded for containing discards, so yield
does not track artefact; and no accumulated offset enters the axis, so the peak is not smeared.

## 5. The objection the draft raised against this, and why it was wrong

**THE DRAFT'S §1.9 CONSTRAINT WAS MISAPPLIED.** `hrv_math.c` records that `HRV_BUF_MAX`
`uint16_t` is too large **for a stack frame** and uses a file-static scratch for `hrv_mad2` for
that reason. **A STATIC ALLOCATION IS NOT A STACK FRAME AND THE CONSTRAINT DOES NOT REACH
IT.** The build report on `b710b6f` shows tens of kilobytes of free heap.

**SO A PARALLEL STATIC ARRAY OF PER-ACCEPTED-BEAT TIME OFFSETS IS AFFORDABLE**, and §1.9's
prohibition in the draft — "no second 400-element array may be allocated" — **is corrected to
apply to STACK allocation only.**

**THE HONEST COST, STATED RATHER THAN MINIMISED:** this adds a per-beat accumulation in
`prv_health_handler`, which is the path every other instrument has so far avoided touching, and
a static array. **NO POWER PREDICTION IS MADE AND NONE MAY BE INFERRED FROM ONE NIGHT.**

## 6. What is owed before this is implemented

1. **A dated readout spec for the time-axis accumulator**, on the same standard as the
   resolution and RSA instruments — it names a new quantity maintained per beat and must say
   what it is defined over and what would falsify it.
2. **The `rrv-pipeline-spec-v1` draft is amended at §3 stage 2, §5.6 and §1.9 before
   freezing**, per this file.
3. **NOTHING IS IMPLEMENTED FROM EITHER FILE UNTIL BOTH ARE FROZEN AND PUSHED.**

## 7. What this decision does NOT establish

**WHETHER THE RESULTING SPECTRUM MEANS ANYTHING.** A correct time axis is necessary and not
sufficient, exactly as `rsa-feasibility-readout-spec-v1` §6 says of a sound axis generally.

**WHETHER THE DISCARDED BEATS WERE ARTEFACT OR SIGNAL.** Unchanged and unknowable here. **A
HIGH `Gp` IS STILL NOT GROUNDS TO MOVE THE RANGE BAND, THE JUMP MULTIPLIER OR
`HRV_STALE_SEC`.**

**THE RUN-LENGTH DISTRIBUTION.** §2's estimate rests on an exponential assumption that nothing
has measured. **IF A LATER SESSION NEEDS THAT NUMBER RATHER THAN THE ORDER OF IT, IT MUST BE
MEASURED AND NOT ASSUMED.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**
