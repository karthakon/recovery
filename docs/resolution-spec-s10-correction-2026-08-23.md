# resolution-spec-s10-correction-2026-08-23

**Corrects `hrv-resolution-readout-spec-v1` §10, frozen at `7a2f58d`. THE SPEC IS NOT EDITED.**
A change is a superseding dated file, never an edit.

**RULE 7: this file records no subject physiological value.**

## The contradiction

**§5 AND §10 OF THE SAME FILE DISAGREE ON HOW MANY LINES DIAG 4 RENDERS.**

**§5 specifies FOUR lines** and gives the render block explicitly:

```
D2 <n>   D4 <n>
D5 <n>   D8 <n>
D10 <n>  Dn <n>
Dm <n>
```

**§10's capture sequence, step 6, says "DOWN to DIAG 4 — capture. THREE LINES. NEW."**

## Which is right, and how that was established

**FOUR IS RIGHT. THREE IS THE ERROR.**

Corroborated three ways, none of them a reading of the other:

1. **The §5 render block enumerates seven values across four lines** and is internally
   consistent with §4's counter table.
2. **The watch was observed rendering four lines with nothing clipped at either end on
   2026-08-22**, after install and before the N29 pre-registration was frozen — recorded in the
   spec's own §6 and marked `[WATCH 08-22]` in handoff v65.
3. **N29's capture confirmed four lines on the recorded night.**

**THE SEVEN VALUES CANNOT FIT ON THREE LINES IN THE TWO-PER-LINE FORM §5 SPECIFIES**, so §10's
figure is arithmetically inconsistent with §4 as well as with §5.

## What is corrected

**§10 STEP 6 READS FOUR LINES.** Nothing else in §10 changes — the capture count of EIGHT, the
five-press walk back, and the ordering of the other steps are all correct as frozen and were
executed correctly on N29.

**NO OTHER SECTION IS AFFECTED.** §5's render block was correct, the implementation followed
it, and no reading from N29 is void or altered by this correction.

## Why it is recorded rather than shrugged off

**IT IS THE RULE 13 EXTENSION IN ITS PUREST FORM: A CONTRADICTION BETWEEN TWO SENTENCES INSIDE
ONE FILE, SERVING DIFFERENT PURPOSES, WHERE NO PAIRWISE CHECK BETWEEN FILES WOULD REACH IT.**
§5 serves the implementer and §10 serves the person holding the watch at stop. **Each was
correct in isolation against what it was checked against, and they were never checked against
each other.**

**THE PRACTICAL RISK WAS REAL AND IT WAS A DATA-LOSS RISK.** §10 is the section a session reads
during the capture sequence, when the values are RAM-only and there is no second chance. **A
person following §10 and seeing four lines has been told to expect three, and the natural
reading of that is that something is wrong with the screen** — at exactly the moment when
hesitating, re-navigating or exiting the app destroys `P-DIDENT` and `P-RESOLUTION` for the
night.

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**
