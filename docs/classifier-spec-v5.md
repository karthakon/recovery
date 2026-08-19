# classifier-spec-v5

**Dated 2026-08-19. Frozen BEFORE implementation.**

**THE SECOND-LARGEST CHANGE EVER MADE TO THE CLASSIFIER**, after
`classifier-spec-v4`. **It changes the SCOPE of one reference and nothing
else.**

**Supersedes `classifier-spec-v3` (`2ae829a`) section 3.5 ONLY** - the
construction of the Awake clause's heart-rate reference. **Section 4.2's clause
structure, the disjunction, c1, the 103, the population filter and the onset
definition are UNCHANGED and are NOT superseded.**

**Reads with `awake-anchor-readout-spec-v1` (`ed504f3`), whose section 2 this
file supersedes for the MEANING of `Ah` only, and
`movement-gate-readout-spec-v1` (`bac5156`). CITED, NOT RESTATED.**

## 1. What is wrong, read from source

**c2 compares a 5-minute windowed heart-rate median against a WHOLE-NIGHT
median of the same quantity, and fires when the former exceeds the latter by
3 percent.** `main.c` 498 assigns the reference as `s_anchor_scratch[k / 2]`,
the upper-middle element of the sorted qualifying set. `main.c` 523 to 524 is
its only read.

**THREE DEFECTS, ALL STRUCTURAL AND NONE REQUIRING A MEASUREMENT TO STATE:**

**1.1 IT IS A MEDIAN SPLIT.** Half the reference population lies above its own
median BY CONSTRUCTION. A 3 percent margin trims some of that half and no more.
**The firing rate is therefore set by the shape of the comparison rather than by
the physiology it claims to detect**, and a clause that fires on a large
fraction of any night cannot mean "elevated." **This is the same defect class
as the `2 x A_D` multiplier that `classifier-spec-v4` removed, and as review
finding 4's standing note on `t2`.**

**1.2 THE REFERENCE IS WHOLE-NIGHT AND THE QUANTITY IS NOT STATIONARY.**
Searched 2026-08-19, before this file was written: heart rate falls after sleep
onset toward a nadir near the core-body-temperature minimum, then rises again
before waking, so the within-night course is **U-SHAPED and NOT a monotonic
decline**. Superimposed on that, autonomic balance shifts toward parasympathetic
dominance in NREM and toward sympathetic modulation in REM.

**A stationary reference applied to a non-stationary quantity fires at the ends
of the night BY CONSTRUCTION**, independently of whether the wearer was awake.
**THE EARLIER FRAMING THAT THIS WOULD PRODUCE AN EARLY-NIGHT EXCESS ONLY WAS
INCOMPLETE AND IS CORRECTED HERE: the same mechanism predicts a LATE excess
too.**

**1.3 THE REFERENCE POPULATION IS NARROWER THAN THE DECISION POPULATION.**
`main.c` 481 to 490 filters the reference to minutes at or after onset with
`still && known`. `main.c` 523 evaluates c2 on EVERY minute. **The calmest
subset sets the bar for the whole night.** **THIS SPEC DOES NOT FIX THIS.** See
section 6.

## 2. The change

**THE REFERENCE BECOMES TIME-LOCAL. NOTHING ELSE CHANGES.**

For each minute `m`, the reference `A_hr(m)` is the median of `HF` over the
minutes `j` satisfying ALL of:

- `j` within the centred window `[m - A_HR_WIN, m + A_HR_WIN]`, **truncated at
  both ends of the recording**;
- `j >= s_onset_epoch_idx`;
- movement at `j` is `still && known`;
- `HF(j) != 0`.

**The median is the upper-middle element of the sorted set, no averaging** -
identical to `main.c` 498 and to `A_H` and `A_D`.

**c2 becomes** `HF(m) * 100 > A_hr(m) * 103`, **with the guard
`A_hr(m) > 0` unchanged.**

**WHY THIS SHOULD REDUCE FIRING, STATED AS A HYPOTHESIS AND NOT AS A FACT:**
the 3 percent margin is FIXED while the SPREAD of `HF` about its reference is
not. A whole-night reference carries the U-shaped course plus stage variation,
so the spread is wide and a fixed small margin admits a large share of the
night. **A local reference removes the slow course from the comparison, the
residual spread narrows, and the same 3 percent then admits only genuine local
excursions.**

**THIS IS A CLAIM ABOUT THE SPREAD OF A DISTRIBUTION THAT HAS NEVER BEEN
MEASURED**, because no per-minute `HF` readout exists. **It may be wrong. A
local median could equally fire MORE, since half of any local window also lies
above its own local median.** The falsifier is registered in section 5.

## 3. The window, DERIVED

**`A_HR_WIN` is 60 epochs, giving a centred window of 121 minutes.**

**A BRANCH READS IT** - it selects which minutes enter the median - **so Rule 2
applies IN FULL and the narrow exemption available to `AWC_EARLY_MIN` does NOT
apply here.** It is derived, not chosen.

**THE DERIVATION.** Searched 2026-08-19 before this file was written, from the
US National Academies sleep volume citing Carskadon and Dement, and
independently from Harvard's sleep division: **the first NREM-REM cycle averages
70 to 100 minutes and later cycles approximately 90 to 120 minutes.**

**THE CONSTRAINT: the window must span at least one full cycle.** A window
shorter than a cycle tracks the NREM-REM oscillation itself, and since REM is
autonomically similar to wakefulness, a short window would RAISE the reference
during REM and hide genuine wake there. **The reference must average over the
cycle, not follow it.**

**120 minutes is the upper end of the cited range, so 120 is the minimum full
width. 121 is the smallest CENTRED window meeting it, giving `A_HR_WIN` 60.**

**IT IS A DERIVED LOWER BOUND AND NOT AN OPTIMUM.** A longer window would
readmit the circadian course this change exists to remove. **NOTHING WAS FITTED
TO ANY NIGHT AND NO RECORDED VALUE ENTERED THIS DERIVATION.**

**`A_MIN_MINUTES` IS CARRIED AT 20, AND ITS MEANING CHANGES.** It guarded a
whole-night population and now guards a 121-minute window, so **20 of 121 is a
PROPORTIONALLY STRICTER guard than 20 of a full night.** **Carrying it
therefore tightens rather than loosens**, and where the window cannot supply 20
qualifying minutes the reference is undefined and **c2 CANNOT FIRE** - the safe
direction, and the same principle as `classifier-spec-v3` section 4.2's rule
that a missing baseline must never make Awake easier to declare. **Stated
explicitly so that carrying it is a decision and not an inheritance.** `main.c`
539 is its only consumer; nothing else in `src/` reads it.

## 4. What does NOT change

- **c1 is byte-identical.** `AW_MOVED_MIN` stays 3, the 5-wide window stays.
- **The 103 is NOT moved.** Its provenance remains unrecovered and that is
  recorded, not repaired here.
- **The disjunction stays a disjunction.** `(c1 || c2)`. **No hierarchy and no
  conjunction is introduced by this file.**
- **The population filter is UNCHANGED** - `still && known`, at or after onset.
  Defect 1.3 stands.
- **Onset, the magnitude band, `MV_MOVED_PCT`, T1, T2, T3, the anchors `A_H`
  and `A_D`, the smoother and every one of its constants are untouched.**
- **The six clause counters and `AwO` are untouched** and continue to partition
  exactly as before.
- **No `EpochRecord` or `NightSummary` field is added.**

**`CLASSIFIER_SERIES` IS BUMPED 13 TO 14. THIS CHANGES OUTPUT AND OPENS A NEW
COMPARABILITY GROUP.** **No night before this bump is comparable to any night
after it on any stage line.** `NIGHT_SUMMARY_VERSION` is NOT bumped - no stored
field changes.

**`tools/fields.py` NEEDS NO CHANGE** - `classifier_series` is an integer with
no enum, and `smoother_version` stays `v3`. **Verify by reading the file before
freezing the pre-registration that names series 14.**

## 5. The registered falsifier

**THE PRIMARY QUESTION IS WHETHER AWAKE FALLS.** It is scored against the
band that has now missed high on six consecutive nights, and **that band is NOT
MOVED by this file.**

**IF AWAKE DOES NOT FALL, THE TIME-LOCAL DIRECTION IS REFUTED** and the work
goes to the hierarchy - c1 sufficient alone, c2 requiring confirmation.
**Registered in advance so that a null result is a result and not an occasion
for retuning `A_HR_WIN`.**

**`A_HR_WIN` MUST NOT BE MOVED IN RESPONSE TO ANY NIGHT'S READING.** It is
derived from a cycle length, not fitted. **A change is a further dated
correction with a new derivation.**

**IF AWAKE COLLAPSES TOWARD ZERO, THAT IS THE OPPOSITE FAILURE AND IS ALSO A
REFUTATION**, not a success. The standing near-zero branch governs and requires
diagnosis before any other line is interpreted.

## 6. Registered risks

**6.1 A LOCAL REFERENCE IS STRUCTURALLY BLIND TO SUSTAINED WAKE.** If the
wearer is awake for a substantial part of the window, the local median becomes
the awake heart rate and no elevation is detected. **The whole-night reference
did not have this weakness.** **c1 is the clause that should catch sustained
wake, and c1 has not fired on either of the last two nights** - so **there is
currently no working fallback and this risk is NOT hypothetical.**
`movement-gate-readout-spec-v1` reads on the same night and is what says whether
c1 can fire at all.

**6.2 THE TRUNCATED ENDS ARE WEAKEST WHERE THE PROBLEM LIVES.** At the start of
the recording the centred window truncates to roughly half its width - less
than one cycle - **so the derivation's guarantee does NOT hold there**, and the
start of the night is exactly where the over-call has been observed.
**Truncation matches the existing convention for every other window in the
project and no new edge rule is introduced**, but the limitation is registered
rather than hidden.

**6.3 DEFECT 1.3 IS NOT FIXED.** The reference population remains narrower than
the decision population. **Deliberate: this file changes ONE variable so that a
null result is attributable.**

**6.4 THE MEDIAN-SPLIT PROPERTY IS NOT ELIMINATED, ONLY RESCALED.** Half of any
local window still lies above its own local median. **This change bets that the
residual spread is small relative to 3 percent. Section 5 is how that bet is
settled.**

## 7. Readout, and what it supersedes

**`awake-anchor-readout-spec-v1` SECTION 2 IS SUPERSEDED FOR THE MEANING OF
`Ah` AND `k` ONLY.** The reference is now a series, not a scalar.

- **`Ah`** - the median of the defined `A_hr(m)` series.
- **`k`** - the count of minutes with a defined reference.
- **NEW LINE `Ahr <min> <max>`** - the minimum and maximum of the defined
  series.

**`Ahr` IS THE CHEAPEST POSSIBLE CHECK THAT THIS CHANGE DID ANYTHING: if min
equals max the reference is not tracking and v5 is inert.** **A criterion may
be registered against it once it has read; registering a band before any
reading exists would invent one from nothing.**

**Everything else on DIAG 2 is unchanged. This takes DIAG 2 to TEN lines and
ten is NOT proven to fit** - nine is, and the recorded overflow sits near
eleven. **The render MUST be verified on the watch before this spec is treated
as implemented.** If ten does not fit, `Ahr` moves or the screen splits, and
**that is a render decision, never a reason to drop the value.**

## 8. Cost

**Computing a median per minute over a 121-minute window is O(n x w) with
insertion sort, on RAM arrays already resident.** `s_epoch_hf`,
`s_epoch_still` and `s_epoch_mv_known` are all RAM; **no `storage_epoch_read`
call is added**, so review finding 10's unverified stop-time watchdog concern is
not engaged.

**Scratch space is bounded by the window, not the night**, and
`s_anchor_scratch` is sized by `EPOCH_VAR_MAX` and therefore already
sufficient. **Verify from source that no live value occupies it at the point
the local medians are computed.**

## 9. What this CANNOT establish

**WHETHER THE AWAKE MINUTES ARE CORRECT.** There is no ground truth.

**WHETHER c2 SHOULD EXIST AT ALL.** The literature registered against this
project holds that autonomic balance during REM is in general similar to
wakefulness, from which it follows that **no heart-rate threshold cleanly
separates REM from wake.** **This change improves the reference. It does not
answer that objection**, and the hierarchy remains the direction that would.

**WHETHER c1 CAN FIRE.** That is the movement readout's question.

**ANYTHING FROM ONE NIGHT.**

## 10. Rule 2 check

**ONE CONSTANT IS INTRODUCED: `A_HR_WIN` 60, and it is DERIVED** from a cited
cycle length, with the derivation in section 3. **A branch reads it and the
narrow exemption was NOT taken.**

**NO OTHER CONSTANT IS INTRODUCED, MOVED OR RE-DERIVED.** `A_MIN_MINUTES` is
carried at 20 with its changed meaning stated in section 3. The 103,
`AW_MOVED_MIN`, `MV_MOVED_PCT`, `AWC_EARLY_MIN` and every smoother constant are
untouched.

**NO VALUE IN THIS FILE WAS TAKEN FROM ANY NIGHT'S MEASUREMENT.**

## 11. Rule 3 check

**THE CHANGE IS FALSIFIABLE AND THE FALSIFYING READINGS ARE NAMED IN ADVANCE:**
Awake failing to fall refutes it; Awake collapsing toward zero refutes it; `Ahr`
min equal to max means it is inert.

**EVERY QUANTITY A CRITERION COULD READ IS RENDERED BY A SCREEN THE NIGHT WILL
CAPTURE** - Awake on RESULTS, the counters and `Ah`, `k`, `Ahr` on DIAG 2.
**Subject to the ten-line render check in section 7.**

## 12. Status

**Frozen on commit. Implemented in a SEPARATE commit. The pre-registration for
the first night under it is frozen SEPARATELY and AFTER, and names
`classifier_series 14`.**
