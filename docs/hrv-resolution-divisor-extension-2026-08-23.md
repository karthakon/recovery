# hrv-resolution-divisor-extension-2026-08-23

**Extends `hrv-resolution-readout-spec-v1`, frozen at `7a2f58d`, implemented at `6439950`.
THAT SPEC IS NOT EDITED.** A change is a superseding dated file, never an edit.

**INSTRUMENT ONLY. NO CLASSIFIER INPUT, NO STAGE DECISION, NO TOTAL, NO STORED FIELD, NO
PERSISTED STRUCTURE.** `CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION` do NOT change and MUST
be VERIFIED UNCHANGED by reading `src/c/storage.h` after implementation, not asserted.

**RULE 7: no subject physiological value is recorded here. N29's readings are in
`~/recovery-nights/nights/C00-N29.md`.**

## §1 — Why this exists

N29 read the MIXTURE branch of `hrv-resolution-readout-spec-v1` §8 — **the one outcome of four
that requires a further night.** That branch instructs the further night to use **"a divisor
set chosen against THIS night's `Dm`."**

**THAT INSTRUCTION IS VACUOUS IN THE CASE THAT ACTUALLY OCCURRED, AND THAT IS RECORDED HERE AS
A DEFECT IN THE BRANCH RATHER THAN WORKED AROUND SILENTLY.** The branch presumes `Dm` came back
at a value the divisor set does not test — 3, 7, 20, 50 — so that it names divisors to add.
**N29's `Dm` is the least informative value that branch can receive**: it excludes every
uniform coarse grid, which is a real and valuable finding, **but it names no divisor.**

**SO THE N30 DIVISOR SET IS CHOSEN AGAINST THE RATIOS INSTEAD OF AGAINST `Dm`.** That is the
correction this file makes to the branch's instruction.

## §2 — What the ratios could not separate, and why it matters

**A MIXTURE WITH A 10 ms COARSE COMPONENT AND A MIXTURE WITH A 20 ms COARSE COMPONENT PREDICT
THE SAME `D5` AND THE SAME `D10`.** Every multiple of 20 is a multiple of 10 and of 5, so both
hypotheses put the same mass in those counters at the same coarse fraction. **THE EXISTING
DIVISOR SET CANNOT SEPARATE THEM AT ALL.**

**THIS IS NOT AN ACADEMIC DISTINCTION. IT IS THE DIFFERENCE BETWEEN A GO AND A STOP.**
`hrv-resolution-readout-spec-v1` §2.1 and §8 register that **a 10 ms step is a GO recorded as a
known noise floor, and that THE STOPPING REGIME IS TENS OF MILLISECONDS — 20 ms or coarser.**
The unresolved question therefore sits exactly on the boundary the whole instrument was built
to locate.

**THE MIXTURE SOFTENS BUT DOES NOT REMOVE THAT.** A coarse component present on only a fraction
of intervals contributes less noise than a uniform grid of the same step. **NO CLAIM IS MADE
HERE ABOUT HOW MUCH LESS**, and `rrv-pipeline-spec-v1` §5.7 remains DEFERRED pending this
reading.

## §3 — The added counters

**TWO COUNTERS ARE ADDED. NO EXISTING COUNTER IS MOVED, REMOVED, RENAMED OR REINTERPRETED.**

| Label | Meaning |
|---|---|
| `D20` | accepted intervals divisible by 20 |
| `D3` | accepted intervals divisible by 3 |

**`D20` IS THE ONE THAT ANSWERS THE QUESTION.** Under a 10 ms coarse component, `D20/Dn` sits
near HALF the coarse fraction plus its own null; under a 20 ms coarse component it sits at the
coarse fraction itself plus its null. **ROUGHLY A TWO-FOLD SEPARATION, AND DECIDABLE ON ONE
NIGHT** because the signature is arithmetic rather than physiological, per the same reasoning
`hrv-resolution-readout-spec-v1` §8 gives for not requiring replication.

**`D3` CLOSES PART OF THE ENUMERATION GAP AND IS SECONDARY.** Nothing in the existing set tests
a non-binary, non-decimal step. **It is added because it is one increment and one comparison,
not because any reading points at 3.** **NO PREDICTION IS MADE ABOUT IT** and a reading near
its null is the expected and uninformative outcome.

**`DO NOT MOVE ANY DIVISOR` IS NOT VIOLATED. ADDING IS NOT MOVING.** The five original divisors
compute exactly what they computed on N29, so **N30 REPLICATES THE N29 READING AND
DISCRIMINATES 10 FROM 20 IN THE SAME NIGHT.** `DM_SENTINEL` is not touched and `Dm` is
unchanged.

## §4 — Identities

**THE EXISTING CHAINS ARE UNCHANGED AND STILL SCORED:**

- `D10 <= D5 <= D2 <= Dn`
- `D8 <= D4 <= D2 <= Dn`

**TWO CHAINS ARE ADDED, BOTH ARITHMETIC NECESSITIES:**

- **`D20 <= D10 <= D5 <= D2 <= Dn`** — every multiple of 20 is a multiple of 10, of 5 and of 2.
- **`D20 <= D4 <= D2 <= Dn`** — every multiple of 20 is a multiple of 4.
- **`D3 <= Dn`** — the only relation `D3` participates in. **NO ORDERING HOLDS BETWEEN `D3` AND
  ANY OTHER DIVISOR AND NONE IS REGISTERED.**

**A VIOLATION MEANS A COUNTER IS MISPLACED AND IS A DEFECT IN THE INSTRUMENT, NOT A FINDING
ABOUT THE SENSOR.** Identical in status to P-DIDENT, whose scope extends to the new chains.

## §5 — Render

**DIAG 4 GAINS ONE LINE, GOING FROM FOUR TO FIVE. THE NINE-LINE CEILING IS NOT APPROACHED.**

```
D2 <n>   D4 <n>
D5 <n>   D8 <n>
D10 <n>  Dn <n>
D3 <n>   D20 <n>
Dm <n>
```

**THE NEW LINE IS INSERTED ABOVE `Dm` SO THAT `Dm` REMAINS THE LAST LINE**, which is where the
capture sequence's reader has learned to find it.

**THE UNDEFINED GUARD KEYS ON `s_session_start`**, identical to the existing lines. Zero prints
`--` and NEVER `0`. **A DEFINED `D20` OF 0 IS A REAL ZERO AND IS A FINDING** — it would mean no
accepted interval was a multiple of 20, which is itself informative.

**NO FRACTION AND NO PERCENTAGE IS RENDERED.** All nine values are on one screen, so deriving
ratios at scoring time is not a RULE 6 violation.

**RULE 11 APPLIES:** `D3` and `D20` verified by `strings` on `pebble-app.elf` in both defined
and undefined variants.

**THE CAPTURE COUNT IS UNCHANGED AT EIGHT** — the new line is on a screen already captured.
**THE FIVE-LINE FIT MUST BE VERIFIED ON THE WATCH WITH NOTHING CLIPPED AT EITHER END BEFORE
N30 IS RECORDED**, on the same standard as the four-line verification of 2026-08-22.

## §6 — The reading, registered BEFORE the night

**NO BAND IS PLACED ON EITHER COUNTER.** What follows is arithmetic from the divisors and takes
RULE 2's narrow exemption because **no decision reads either number.**

**THE DECISION RULE FOR THE COARSE COMPONENT, REGISTERED IN ADVANCE:**

- **`D20/Dn` near the 1 ms null and well below `D10/Dn`** → the coarse component is 10 ms, not
  20. **The mixture is characterised and `rrv-pipeline-spec-v1` §5.7 takes 10 ms as its noise
  floor.**
- **`D20/Dn` close to `D10/Dn`** → the coarse component is 20 ms or coarser. **THAT IS THE
  STOPPING REGIME FOR A UNIFORM GRID AND MUST NOT BE READ AS AN AUTOMATIC STOP HERE**, because
  it is present on only a fraction of intervals. **The pipeline's noise floor is then set
  against 20 ms and the direction is reconsidered on that basis, as a decision and not as an
  arithmetic consequence.**
- **`D20/Dn` between the two** → NOT DECIDED. **A further extension is required and this file's
  own instruction must not be applied recursively without stating what the next divisor would
  separate.**

**`D3` HAS NO DECISION RULE. IT IS RECORDED, NOT SCORED, AND NOT BANDED.**

**THE 1.00 SIGNATURE STILL GOVERNS THE UNIFORM CASE**, unchanged from
`hrv-resolution-readout-spec-v1` §6 — but N29's `Dm` already excludes a uniform coarse grid, so
**a divisor at 1.00 is not the expected outcome and would contradict N29.** If one reads 1.00,
**that contradiction is the finding and takes precedence over anything else on the screen.**

## §7 — What this extension CANNOT say

**WHETHER THE MIXTURE IS TWO COMPONENTS OR MORE.** It separates 10 from 20 at the coarse end.
It does not enumerate the composition.

**WHY SOME INTERVALS ARE QUANTISED AND OTHERS ARE NOT.** That is inside the nonfree Goodix
algorithm and `hrv-resolution-readout-spec-v1` §1 records that it is NOT ESTABLISHABLE FROM
SOURCE.

**WHETHER RRV WOULD WORK OR WOULD HELP CLASSIFY SLEEP.** Unchanged and untouched. **NO READING
FROM THIS INSTRUMENT MAY BE CITED TOWARD THAT SECOND GATE.**

**ANYTHING ABOUT `Hd`.** A coarse component raises coincidence and so explains part of a high
`Hd`. **IT DOES NOT PROVE RE-LATCHING ABSENT AND A FINE READING WOULD NOT PROVE IT PRESENT.**

## §8 — Rule 2 and Rule 3 checks

**RULE 2 — NO NEW CONSTANT ENTERS THE CODE THAT ANY DECISION READS.** `D3` and `D20` are
divisors and are **the definition of the measurement, not thresholds**, exactly as the original
five are. `HRV_STALE_SEC`, the range band, the jump multiplier, `HRV_BUF_MAX` and `DM_SENTINEL`
are read and UNMOVED.

**RULE 3 — the added identities ARE checkable and a night MUST check them.** A reading that
would make them PASS is any correctly-counted night, including one where both new counters read
0. A reading that would make them FAIL is any nesting violation. **NEITHER IS UNREACHABLE.**

**CHECKED AGAINST THE UNEVALUABLE-CHECK DEFECT:** both new values are on DIAG 4, captured live
in the stop sequence, and **DIE WITH THE SCREEN.**

**CHECKED AGAINST THE THIRD VACUITY FORM:** §6 names a reading that establishes a 10 ms
component, one that establishes 20 ms or coarser, and one that decides neither. **None is
unreachable and the failing readings mean what they appear to mean.**

## §9 — Comparability

**INSTRUMENT ONLY.** **`CLASSIFIER_SERIES` STAYS 14 AND `NIGHT_SUMMARY_VERSION` STAYS 3, TO BE
VERIFIED BY READING `src/c/storage.h` AFTER IMPLEMENTATION AND NOT ASSERTED.** N30 remains
comparable to N26 through N29 on every stage line.

**ONE COST IS REGISTERED HONESTLY:** two additional divisibility tests and two increments in
the HRV event handler, which runs once per beat. **NO PREDICTION IS MADE ABOUT POWER AND NONE
MAY BE INFERRED FROM ONE NIGHT.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**

**NOTHING IS IMPLEMENTED FROM THIS FILE UNTIL IT IS FROZEN AND PUSHED.**
