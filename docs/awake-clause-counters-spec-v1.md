# awake-clause-counters-spec-v1

**Frozen 2026-08-17. An INSTRUMENT ONLY. It changes NO decision and NO output.**

## 1. Why this exists

`prv_awake_redecide` declares a minute Awake when EITHER of two independent
clauses holds. **Which clause fired is recorded nowhere.** Both `moved` and the
heart-rate comparison are local variables discarded at the end of each loop
iteration, and `EpochRecord` has no field carrying either.

Awake has read above its registered band on four consecutive nights. **No
change to either clause can be argued from evidence while the firing clause is
unknown**, and a change aimed at the wrong clause is wasted work that also
opens a comparability group for nothing.

## 2. What is counted

Read from source at `0a1f3d2`, `main.c` 492 to 523. The two clauses are:

- **c1** - `moved >= AW_MOVED_MIN`, where `moved` counts MOVED minutes over the
  centred window `i-2 .. i+2` truncated at edges. UNKNOWN is not MOVED.
- **c2** - `a_hr > 0 && s_epoch_hf[i] > 0 && s_epoch_hf[i] * 100 > a_hr * 103`.

`SleepStage ns_stage = (c1 || c2) ? StageAwake : StageLight;`

**SIX counters, three partitions across two spans.** The partition is over
minutes where `(c1 || c2)` holds, and it is EXHAUSTIVE and DISJOINT by
construction - every such minute falls in exactly one of c1-only, c2-only,
both.

The two spans are measured from `s_onset_epoch_idx`:

- **EARLY** - epoch index at or after onset and below onset + 60.
- **LATE** - epoch index at or above onset + 60.

**Minutes BEFORE onset are counted in NEITHER span.** Recovery always starts
Awake because the user presses the button while awake, and pre-onset Awake is
already excluded and recorded separately under P-ARCH limb 3. Including it
would put a definitional artifact into a diagnostic about a suspected defect.

**When onset is undefined (`s_onset_epoch_idx < 0`) all six counters stay
ZERO.** They are not backfilled from index 0. A night with no onset cannot
speak to an early-versus-late split at all, and a zero is honest where a
substitute origin would be an invention.

## 3. The 60

**60 minutes is a SPAN BOUNDARY for a readout, NOT a threshold in any
decision.** No branch anywhere reads it. It is one hour, chosen as a round
division of a night for a first reading, and **it is NOT taken from any
measurement on any night.**

**Rule 2 note, stated plainly rather than hedged:** a constant taken from no
measurement is normally the same defect with the sign flipped. It is
acceptable HERE and only here because **nothing branches on it.** Moving it to
45 or 90 would change which bucket a minute is reported in and would change no
stage, no total, and no decision. **If this number is ever read by a decision,
this paragraph stops applying and it must be derived.**

## 4. What does NOT change

**NO DECISION CHANGES.** `c1`, `c2`, `AW_MOVED_MIN`, the `103`, the window
shape, the anchor `a_hr`, and the `(c1 || c2)` disjunction are ALL byte
identical. The counters are incremented from the SAME `c1` and `c2` booleans
the decision uses - they are not recomputed, so they cannot disagree with it.

**NO OUTPUT CHANGES.** No stage, no minute total, no stored field.

**`CLASSIFIER_SERIES` IS NOT BUMPED and `NIGHT_SUMMARY_VERSION` IS NOT
BUMPED.** Same treatment as the transition counters. A night recorded after
this commit remains comparable to N23 on every stage line.

**NO `EpochRecord` CHANGE.** Six static `uint16_t` in RAM. The struct is not
touched and the three-sizes read path is not touched.

## 5. Render

Two lines appended to DIAG 2 below `Unk`.

```
C1e <n> C2e <n> Be <n>
C1l <n> C2l <n> Bl <n>
```

`e` is EARLY, `l` is LATE. `B` is both clauses in the same minute.

**DIAG 2 renders from RAM statics and does NOT survive app exit.** These lines
are captured at stop in capture 4, with the rest of DIAG 2.

## 6. The registered identity, and it CAN fail

**`C1e + C2e + Be + C1l + C2l + Bl` MUST EQUAL the Awake minutes falling at or
after onset.** It will NOT equal the RESULTS `Awake` total, because RESULTS
includes pre-onset Awake and these counters exclude it.

**A mismatch means the counters are not observing the decision they claim to
observe, and the reading is void.** This is the check that makes the
instrument falsifiable rather than merely descriptive.

## 7. What this CANNOT establish

**Whether either clause is WRONG.** A clause firing often is not thereby
mistaken, and the counters carry no ground truth.

**Whether c2 firing early is a defect or is structural.** `a_hr` is a median
over the whole night from onset, and early-night heart rate is normally the
highest sleeping heart rate of the night, so **a 3 pct margin against a
whole-night median may fire early BY CONSTRUCTION.** That is a hypothesis from
the shape of the code and these counters do not settle it - they establish
only WHICH clause fires and WHEN.

**Anything from one night.** First readings, no band, and NO CONSTANT MAY BE
FITTED TO THEM.

## 8. Status

Frozen on commit. A change is a v2 or a dated correction, never an edit.
