# awake-runs-readout-spec-v1

**Dated 2026-08-19. An INSTRUMENT ONLY. Frozen before implementation.**

**Reads with `epoch-readout-spec-v1` (`413e850`) as corrected by `dff61ec`,
`465f93e` and `46c1ddb`, and with
`awake-clause-counters-spec-v1-identity-correction-2026-08-18.md` (`e8fe888`).
CITED, NOT RESTATED.**

## 1. Why this exists

**THE PROJECT MEASURES AWAKE AS A WHOLE-NIGHT TOTAL AND HAS NEVER MEASURED ITS
EPISODE STRUCTURE.** The scored Awake criterion is a percentage of the
recording, and a percentage cannot distinguish **one long block from many short
ones.**

**THAT DISTINCTION IS THE ENTIRE CONTENT OF THE USER'S SELF-REPORT.** The
reported pattern is falling asleep within roughly 5 to 20 minutes of lying down,
and mid-night wakes that rarely exceed 5 minutes. **Neither of those is a
statement about a nightly total. Both are statements about EPISODE LENGTH and
about WHERE the episode falls.**

**A night can therefore sit inside the Awake band and still be flatly
inconsistent with how the user sleeps, and a night can sit outside it for
reasons the band cannot name.** The total is the wrong instrument for the claim
and always was.

**THE STRUCTURE IS NOT READABLE FROM WHAT EXISTS.** RUNS breaks down REM run
lengths - longest, count, and a distribution across five buckets - and provides
**NOTHING equivalent for Awake.** The hypnogram shows blocks but
`prv_draw_hypno` OVERPAINTS when epochs exceed plot pixels, so a block's length
cannot be read off it and no adjacency may be inferred from it. **The per-minute
sequence has no readout at all.**

**THE ASYMMETRY IS HISTORICAL, NOT PRINCIPLED.** REM run lengths were
instrumented because REM was the open problem. **Awake is the open problem now
and carries no equivalent instrument.**

## 2. What is counted

**POST-SMOOTHER ONLY, from `EpochRecord.stage`.** RESULTS, the hypnogram and
every reported total are post-smoother, and **the self-report is a claim about
what the app reports**, so the pre-smoother series would answer a different
question. **The pre-smoother Awake count already exists as `AwO` and is not
duplicated here.**

**THE SPAN BOUNDARY IS `s_onset_epoch_idx`**, the live immobility onset - the
same index `AwO` and the six clause counters branch on. **NOT `onset_label`.**
Using a different onset would make these values incomparable with the counters
they sit beside.

**FOUR VALUES:**

- **`AwMx`** - the length in minutes of the LONGEST Awake run at or after onset.
  **This is the mid-night wake figure and the value the self-report speaks to.**
- **`AwP`** - Awake minutes BEFORE onset. **Recovery ALWAYS starts Awake because
  the user presses the button while awake**, so this is a definitional artifact
  and is separated deliberately rather than folded into the others. It is also
  the sleep-latency figure, against a reported 5 to 20 minutes.
- **`AwR`** - the count of Awake runs at or after onset.
- **`Aw5`** - how many of those runs exceed 5 minutes.

**A RUN IS A MAXIMAL CONSECUTIVE SEQUENCE OF EPOCHS LABELLED AWAKE.** A run
straddling onset is counted from onset onward, so `AwP` and the run values never
double-count a minute.

**THE FAILED-READ BEHAVIOUR IS INHERITED, NOT DESIGNED.** `prv_compute_runs`
skips an epoch whose `storage_epoch_read` fails, and the existing REM run
counter lets a run persist across that gap rather than breaking it. **The Awake
runs mirror that exactly**, because two run-counters in one loop behaving
differently on the same fault would be worse than either behaviour alone.
**Recorded as inherited so it is not later mistaken for a decision.**

## 3. The 5

**`AW_RUN_LONG` is 5 minutes and it comes from the USER'S SELF-REPORT, not from
a measurement and not from the literature.**

**NOTHING BRANCHES ON IT.** It selects which runs are tallied into one reported
count and is read by no decision. Moving it to 4 or 10 would change a number
that is REPORTED and would change no stage, no total, no label and no decision.

**THE NARROW EXEMPTION APPLIES AND ITS EXPIRY IS REGISTERED WITH IT: the moment
any decision reads this constant, the exemption stops applying and it must be
derived.** Same standing as `AWC_EARLY_MIN` and the percentile choice in the
movement readout.

**IT IS A TOLERANCE, NOT A THRESHOLD, AND MUST NOT ACQUIRE THE STATUS OF A
MEASURED VALUE BY REPETITION.**

## 4. Undefined, and the volatility this inherits

**WHEN `s_onset_epoch_idx` IS UNDEFINED, ALL FOUR VALUES ARE UNDEFINED.** They
render `--` and NEVER `0`, per `measurement-spec-v1` section 3.6. **They are not
backfilled from index 0** - a night with no onset cannot speak to an
at-or-after-onset span at all, and a substitute origin would be an invention.
Same reasoning as `d9cfd5a` section 2.

**THESE VALUES INHERIT `AwO`'s VOLATILITY AND THIS IS THE COST OF KEYING THEM
CORRECTLY.** `s_onset_epoch_idx` is a RAM static reset on app launch, so **all
four read `--` on any re-read after the app exits, even though the epochs they
would count are still stored** - exactly as recorded for `AwO` in `6cfc319`.

**THEY MUST THEREFORE BE CAPTURED WITH DIAG-2 DISCIPLINE, NOT RUNS DISCIPLINE**,
despite living on RUNS. **If the capture is missed they cannot be recovered.**

**KEYING THEM ON `onset_label` INSTEAD WOULD MAKE THEM SURVIVE AND IS REJECTED
FOR THE SAME REASON IT WAS REJECTED FOR `AwO`:** `onset_label` is a DIFFERENT
onset, and using it would change which span is counted and break comparability
with the counters these sit beside.

## 5. Render

**TWO LINES APPENDED TO RUNS, AS THE TENTH AND ELEVENTH:**

```
AwMx <n>  AwP <n>
AwR <n>  Aw5 <n>
```

**RUNS CURRENTLY RENDERS NINE LINES.** DIAG 2 has been verified on the watch
rendering ELEVEN at the same font on the same screen, so eleven is expected to
fit here - **but RUNS has its own header and its own `y` progression and
EXPECTED IS NOT VERIFIED.** **THE RENDER MUST BE CHECKED ON THE WATCH BEFORE
THIS SPEC IS TREATED AS IMPLEMENTED**, and the check must confirm the FIRST line
is still present as well as the last, since the recorded overflow removed a
field silently.

**IF ELEVEN DOES NOT FIT, RUNS SPLITS** - the same remedy applied to DIAG, for
the same reason. **The font is never shrunk and no value is dropped.**

## 6. What does NOT change

**NO DECISION, NO THRESHOLD, NO WINDOW SHAPE, NO STORED FIELD.**

- Read-only over stored epochs, `storage_epoch_read` only, **never
  `storage_epoch_update`.**
- **No new pass and no new traversal** - the values are accumulated inside the
  loop `prv_compute_runs` already runs, exactly as `AwO` was.
- No `EpochRecord` field, no `NightSummary` field. `RunStats` grows, and it is a
  RAM-only struct with no persist site.
- The REM run breakdown, the transition counters, `Off`, `Ons`, `OnsL` and
  `AwO` are untouched.
- **`CLASSIFIER_SERIES` is NOT bumped and `NIGHT_SUMMARY_VERSION` is NOT
  bumped.** **The bump rule is OUTPUT CHANGES and this changes no output.**
- **This spec opens NO comparability group.**

**VERIFICATION REQUIRED BEFORE COMMIT, NOT ASSERTED HERE:** read both version
constants from `storage.h` after the edit and confirm `git diff --stat` names
`src/c/main.c` alone.

## 7. What this CANNOT establish

**WHETHER THE AWAKE MINUTES ARE CORRECT.** A run-length distribution describes
what the classifier produced. **It carries no ground truth and the self-report
is not ground truth either** - it is a report, recorded as one.

**WHERE THE RUNS FALL.** Only the longest is reported, not its position in the
night. **Two nights with identical values may have completely different
structure.** Locating the runs needs the per-minute sequence, which this spec
does NOT build.

**WHY A RUN IS LONG.** The clause counters say which clause fired across the
night; **they do not attribute a particular run.**

**ANYTHING ABOUT A PAST NIGHT.** These are computed from stored epochs, so a
past night's values could in principle be recovered - **but only while its
epochs remain, and `storage_session_start` zeroes the count when the next
recording begins.** In practice the previous night only.

**ANYTHING FROM ONE NIGHT.**

## 8. Rule 2 check

**ONE CONSTANT IS INTRODUCED: `AW_RUN_LONG` 5, taken from the self-report and
NOT from a measurement.** Section 3 registers the narrow exemption and its
expiry. **No other constant is introduced, moved or re-derived.**

**NO BRANCH READS ANY NEW VALUE.** Each is accumulated once and read once, in a
draw call.

## 9. Rule 3 check

**THIS SPEC REGISTERS NO CRITERION AND MAKES NO PREDICTION.** It is an
instrument. **A pre-registration may register criteria against these values once
they have read at least once.**

**A CRITERION BUILT ON THEM MUST NOT BE A RESTATEMENT OF THE SELF-REPORT DRESSED
AS A PREDICTION.** The self-report is what the instrument is being built to
compare against; **registering "AwMx will be small because the user says he
wakes briefly" predicts nothing and would fail the vacuity check.** A criterion
must name a reading that would make it FAIL.

**THE RENDER-SITE CHECK:** all four values are rendered by RUNS, subject to
section 5's verification. **If a value proves unrenderable it is moved, and no
criterion is registered against it until it renders.**

## 10. Status

**Frozen on commit. Implemented in a SEPARATE commit. No frozen spec is edited
by this file and this file supersedes nothing.**
