# stillness-run-readout-spec-v1

**INSTRUMENT ONLY. NO OUTPUT CHANGE. NO `CLASSIFIER_SERIES` BUMP. NO
COMPARABILITY GROUP.** It supersedes nothing.

## 1. Why this exists

**THE AWAKE CLAUSE REMOVES A MINUTE FROM THE REM DECISION ENTIRELY.** Read from
source: `prv_awake_redecide` sets `StageAwake` from a disjunction of two
clauses, and `prv_base_redecide` skips `StageAwake` before any of T1, T2 or T3
is evaluated. The comment at the clearing site states the intent outright -- a
minute that clears BOTH clauses goes to Light so that it becomes eligible for
the Light/REM decision. **A minute caught by the heart-rate clause is never
tested for REM at all.**

**SO AN OVER-CALL OF AWAKE AND AN UNDER-CALL OF REM ARE NOT TWO DEFECTS. THEY
ARE ONE EVENT SEEN FROM TWO SIDES.** Every minute the heart-rate clause claims
is a minute subtracted from the REM candidate pool before the pool is scored.

**AND THE TWO STATES ARE KNOWN TO BE AUTONOMICALLY SIMILAR.** The registered
literature holds that autonomic balance during REM is in general similar to
wakefulness, from which it follows that no heart-rate threshold cleanly
separates them. **A heart-rate clause with no second input is therefore
structurally unable to make this distinction, at ANY margin.**

**WHAT COULD MAKE THE DISTINCTION IS SUSTAINED STILLNESS, AND NOTHING RENDERS
IT.** `s_epoch_still` and `s_epoch_mv_known` are RAM-only with no persist site
and no render site, and `EpochRecord` carries no movement field. The existing
movement readout reports a per-minute moved FRACTION distribution; it says
nothing about how those minutes are ARRANGED. **A night of scattered isolated
still minutes and a night of long unbroken still runs can produce identical
readings on every instrument that exists.**

## 2. What is counted

**A STILLNESS RUN IS A MAXIMAL CONSECUTIVE SEQUENCE OF MINUTES THAT ARE BOTH
`still` AND `known`**, read from the movement bitmaps directly.

**UNKNOWN BREAKS A RUN, EXACTLY AS MOVED DOES.** This is inherited deliberately
from the onset rule, whose spec registers the reason -- a run interrupted by
minutes carrying no evidence is not a run of consecutive immobile minutes, and
the alternative silently bridges an off-wrist gap. **A run counted under a
different rule from the onset streak would share the word "run" while measuring
something else.**

**RUNS ARE BUILT FROM MOVEMENT EVIDENCE ONLY AND NEVER FROM STAGE LABELS, AND
THIS IS THE LOAD-BEARING RULE OF THIS FILE.** `classifier-spec-v3` section 1
records the distinction: the actigraphy convention counts consecutive IMMOBILE
minutes, which depend on nothing downstream, while the onset streak counts
consecutive non-Awake minutes AS JUDGED BY THE CLASSIFIER -- "same 5, different
input. That difference is the circularity." **An instrument built to inform a
future change to the Awake decision must not take that decision as input.**

**SPAN.** Runs are computed over the whole recording. **The two counters in
section 4 are restricted to minutes at or after `s_onset_epoch_idx`**, the same
index the clause counters and the Awake-run values branch on, so they are
comparable with the counters they sit beside.

## 3. The 5, DERIVED -- and its weakness registered with it

**`STILL_RUN_MIN` IS 5 MINUTES.**

**PRIMARY DERIVATION, AND IT IS ON-PURPOSE:** submental EMG studies report that
the drop in muscle tone BEGINS ABOUT FIVE MINUTES BEFORE the onset of a REM
episode, with a gradual recovery over the following twenty minutes, and that
extended epochs of muscle atonia occur within NREM sleep with the highest
values immediately BEFORE and AFTER REM episodes. **A minute embedded in five
or more consecutive still minutes therefore sits inside the window in which
atonia is already established.**

**CORROBORATION, AND IT IS PURPOSE-MISMATCHED. IT DOES NOT DERIVE THE VALUE.**
The actigraphy literature comparing 4, 5, 6, 10 and 15 minutes of immobility
found 5 the most accurate for SLEEP LATENCY, with near-optimal total sleep time
and wake after sleep onset. **That is calibrated for ONSET DETECTION, not for
REM candidacy.** Treating it as the derivation would carry a constant
calibrated against a different question -- the same defect class as the `103`
and as the multiplier `classifier-spec-v4` removed.

**NOT A THIRD SOURCE:** `SLEEP_ONSET_MINUTES` is also 5, and
`classifier-spec-v3` section 1 states it is the value the actigraphy literature
validates best. **It IS the corroborating source, not an independent one. A
number appearing at several sites is a COPY, not a corroboration.**
`RUNS_ONSET_RUN` is a fourth 5 governing the label-derived onset; **its
provenance is UNREAD and it is cited as evidence for nothing.**

**THE WEAKNESS, REGISTERED RATHER THAN BURIED: the primary derivation rests on
ONE retrieved line of work.** One study is not a literature. **THE EXPIRY: if a
second on-purpose source is found and disagrees, the constant is re-derived in
a dated correction and is NOT defended by having shipped.**

**NOTHING IN THIS FILE BRANCHES ON THE 5 IN A WAY THAT CHANGES A STAGE.** It
selects which runs are tallied and which minutes are counted. **The moment a
DECISION reads it, this section is the derivation that decision inherits, and
it must be re-examined against the weakness above before it does.**

## 4. What is rendered

**FIVE VALUES ACROSS THREE NEW LINES.**

- **`Sr <p50> <p90> <mx>`** -- the median, ninetieth percentile and maximum of
  the stillness-run lengths, in minutes. Percentile convention is the
  upper-middle element, no averaging and no interpolation, identical to the
  existing anchors.
- **`SrN <n>  Sr5 <n>`** -- the count of stillness runs, and how many of them
  reach `STILL_RUN_MIN`.
- **`C2s <n>  C2n <n>`** -- **THE DECISIVE PAIR.** `C2n` is the count of
  post-onset minutes the heart-rate clause claimed. `C2s` is how many of those
  sit INSIDE a stillness run of at least `STILL_RUN_MIN` minutes.

**THE FRACTION IS NOT RENDERED AND THAT IS DELIBERATE.** Both inputs are on the
screen, so deriving it at scoring time is not a Rule 6 violation -- the same
reasoning the anchor readout gives for not rendering its own threshold.

**UNDEFINED PRINTS `--` AND NEVER `0`**, per `measurement-spec-v1` section 3.6.
`C2s` and `C2n` are undefined when onset is undefined; `Sr` is undefined when
no run exists. **`Sr5 0` WITH A DEFINED `SrN` IS A REAL ZERO AND NOT AN
UNDEFINED** -- it means runs existed and none reached the threshold, which is a
finding rather than a gap.

**VOLATILITY IS INHERITED.** `C2s` and `C2n` key on the RAM-only live onset
index, so they read `--` after an app restart exactly as the existing
onset-keyed values do. **They are captured live or they are lost.**

## 5. Render placement -- and why a screen splits

**DIAG 2 AND RUNS ARE BOTH FULL AT ELEVEN LINES.** The recorded overflow sits
just beyond eleven, the font is never shrunk and no value is dropped, so a
twelfth line on either is not available. **A NEW SCREEN, DIAG 3, IS CREATED.**

**AN AUDIT FOR REDUNDANCY WAS PERFORMED FIRST AND FOUND NOTHING REMOVABLE.**
All three diagnostic screens were read in full. **The one cross-screen
duplication is DELIBERATE and load-bearing** -- the `Fd` median is the same
value as the T1 anchor and is printed on the other screen as a cross-check
which a conditioning check scores. **Removing either side would delete a
conditioning check.** Every other value is uniquely sourced.

**THE MOVEMENT LINES MOVE TO DIAG 3 WITH THE NEW VALUES.** The moved-fraction
distribution and the new stillness-run values are two halves of one question --
how much movement, and how it is arranged -- and reading them off different
screens invites exactly the cross-screen error the label-collision rule exists
to prevent. **This leaves DIAG 2 at nine lines, a height already proven to fit
rather than merely observed to.**

**NAVIGATION.** DOWN from DIAG 2 enters DIAG 3; UP and BACK return to DIAG 2.
This is the existing DIAG-to-DIAG-2 pattern repeated and adds no new idiom.
**THE CAPTURE SEQUENCE GAINS A SEVENTH CAPTURE and the return path from the
last diagnostic screen lengthens by one press.**

**THE RENDER MUST BE VERIFIED ON THE WATCH BEFORE THIS SPEC IS TREATED AS
IMPLEMENTED**, on BOTH screens, checking that nothing is clipped at either end
and that the first line is present.

## 6. What does NOT change

- **No decision, no stage, no minute total, no stored field.** The values are
  accumulated from data the classifier already computes.
- **`AW_MOVED_MIN`, `MV_MOVED_PCT`, the magnitude band and the `103` are NOT
  moved.** This file describes; it does not license.
- **The disjunction is unchanged.** Both clauses fire exactly as before.
- **`SLEEP_ONSET_MINUTES` and `RUNS_ONSET_RUN` are NOT touched.**
- **No spec is edited.** This file supersedes nothing.

## 7. What this CANNOT establish

**IT READS FORWARD ONLY.** The movement bitmaps are RAM-only and no stored
epoch carries a movement field, **so no past recording can be re-scored against
this instrument.** The first reading comes from the next recording.

**IT CANNOT SEE MOVEMENT THE MAGNITUDE GATE DISCARDED.** Stillness here means
what the existing band-only magnitude test calls still. A slow postural roll
preserving one g reads as still to that test and therefore as still here.
**A long run is consistent BOTH with a motionless wearer AND with a wearer
moving in ways the gate cannot see**, and this instrument cannot separate them.

**IT CANNOT SAY WHETHER A MINUTE WAS REM.** `C2s` counts minutes that are
CANDIDATES under a rule that does not exist yet. **A high count is not evidence
that those minutes were REM; it is evidence that a proposed rule would reach
them.**

**IT CANNOT DISTINGUISH QUIET WAKEFULNESS FROM REM.** Lying awake motionless
produces a long stillness run and a wake-like heart rate. **This is the
registered hazard of the direction this instrument informs, and the instrument
does not resolve it** -- it measures how large the affected population is.

## 8. Rule 2 check

**ONE CONSTANT IS INTRODUCED AND IT IS DERIVED, NOT ASSERTED** -- see section 3,
including its single-study weakness and its expiry.

**NO VALUE IN THIS FILE COMES FROM ANY RECORDED NIGHT.** The percentile choice
follows the existing convention rather than being selected. **No band is
registered on any rendered value.**

## 9. Rule 3 check

**NO CRITERION IS REGISTERED AGAINST THESE VALUES IN THIS FILE, AND NONE MAY BE
UNTIL THEY HAVE READ AT LEAST ONCE.** Registering a band before any reading
exists would invent one from nothing. **This is the same restriction the
Awake-run readout placed on itself, and it is honoured here rather than waived.**

**EVERY VALUE HAS A RENDER SITE**, named in section 4, on a screen the capture
sequence will capture. **No future criterion against these values can be
unscorable for want of a render site**, which is the defect this file exists in
part to prevent for the change that follows it.

**A READING THAT WOULD MATTER IN EACH DIRECTION IS NAMED IN ADVANCE:** `C2s`
near `C2n` means the great majority of heart-rate-clause minutes sit inside
sustained stillness and a stillness-gated rule would reach nearly all of them.
`C2s` near zero means they do not, and that such a rule would change almost
nothing. **Both are reachable and they are distinguishable.**

## 10. Status

**Frozen on commit. A change is a superseding dated file, never an edit.**
