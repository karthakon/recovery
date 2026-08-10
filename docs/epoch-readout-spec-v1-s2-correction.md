# Recovery - Epoch Readout Spec v1, SECTION 2 IDENTITY CORRECTION

**DATED 2026-08-10. Supersedes the REGISTERED IDENTITY of section 2 of
docs/epoch-readout-spec-v1.md (413e850). Nothing else in that document
changes.**

**413e850 is NOT edited.** Its section 2 stands as written and as committed.
This file records that its registered identity was WRONG AS STATED, why, and
what replaces it. Superseding in a new dated file rather than editing the
frozen one is the standing rule for every frozen document in this project.

This is the SECOND correction to 413e850. The first
(docs/epoch-readout-spec-v1-nav-correction.md, same date) corrected section 4
navigation only. **The two are independent and neither amends the other.**

**Nothing else in 413e850 changes.** Sections 0, 1, 3, 5, 6, 7 and 8 stand
unamended. No statistic changes. No threshold moves. No stage decision changes.
EpochRecord and NightSummary remain untouched. No code changes as a result of
this correction.

---

## 0. What was registered, and what happened

413e850 section 2 registered - the pre-smoother REM minute count MUST EQUAL the
night v_over_gate_count, both reading 15 on N16 - and stated that a mismatch
means reserved does not hold what section 2 claims, and that nothing else on
the screen is interpretable until that is diagnosed.

**N16 read RemN 14 against Gate 15. The registered identity FAILED on the first
and only night it was ever tested.**

**The identity was wrong as stated.** reserved holds exactly what section 2
claims it holds. The defect is that the two quantities were asserted to be
equal when the code never computes them over the same population.

---

## 1. The mechanism, established by source read

Read from main.c prv_measure (lines 228 to 268) and prv_base_redecide (lines
280 to 297) as they stand at b0f3a7c.

**The two counts run over different arrays in different states.**

prv_base_redecide runs FIRST and indexes s_epoch_var BY EPOCH INDEX. This is
why measurement-spec-v1 section 3.3 permits sorting only afterward.
prv_measure then COMPACTS the array, dropping every zero entry and shifting the
remainder forward, and then SORTS it ascending in place. **After compaction and
sorting no entry corresponds to any epoch.** s_v_over_gate is counted over that
compacted sorted array and is therefore a count of VALUES exceeding the gate,
with no reference to epochs at all.

prv_base_redecide counts over the unsorted array, per epoch, and applies three
skips that prv_measure does not:

1. **rec.stage equal to StageAwake is skipped** - base-spec-v1 section 3.4
   step 2. A gate-clearing minute classified Awake is counted by Gate and never
   re-decided to REM.
2. **rec.beat_count below 20 is skipped** - base-spec-v1 section 3.4 step 3.
   prv_measure has no beat-count condition.
3. **The loop is CLAMPED to the smaller of storage_epoch_count() and
   s_epoch_var_count.** If those two disagree, the trailing epochs are never
   re-decided while prv_measure still counts their variances.

**Gate is therefore the LOOSER count by construction, and Gate at least RemN is
a STRUCTURAL relation rather than an observation.**

---

## 2. THE REPLACEMENT RELATION

**RemN is LESS THAN OR EQUAL TO Gate.**

- **RemN equal to Gate** - no gate-clearing minute was skipped. Expected on a
  night with no Awake or low-beat-count minutes above gate and no index
  disagreement.
- **RemN less than Gate** - the difference is the number of gate-clearing
  minutes skipped by one of the three mechanisms in section 1. **This is a
  NORMAL reading, not a defect**, and does not invalidate the screen.
- **RemN GREATER THAN Gate** - **THIS remains a genuine failure.** reserved
  would then hold more REM minutes than there were gate-clearing values, which
  no path in the code can produce. Diagnose before reading any other field.

**The difference is NOT attributable to a specific mechanism from these two
numbers alone.** Three candidates exist and this screen cannot separate them.
Attributing the N16 difference to any one of them would be a guess.

---

## 3. Effect on the N16 reading - NONE

N16 read RemN 14, Runs 13, Max 2, L1 12, L2 1, L3 0, L4 0, L5p 0, Off 26,
Ep 494.

**The conclusion drawn from that reading does not depend on the discrepancy.**
Max 2 is the operative number. Source reading of smoother.c lines 26 to 51
establishes that REM selection requires a run of at least 4 on Viterbi cost and
at least 5 after min_episode_pass. **A maximum run of 2 cannot produce reported
REM under any arrangement, and neither can 3** - so even if the missing minute
were adjacent to the longest run, the outcome is unchanged.

**Registered outcome stands: r5p equal to 0. The smoother behaved exactly as
specified and the diagnosis is the classifier, per smoothing-spec-v1 section
8.**

---

## 4. The open item this leaves

**Whether storage_epoch_count() and s_epoch_var_count agree is NOT settled and
is queued, not fixed here.** It bears on mechanism 3 of section 1 and
independently on the N16 observation that rec_classified_min 494 exceeds
rec_duration_min 493 by one. **Whether those two off-by-ones share a cause is a
HYPOTHESIS and is not established.** One change at a time; no code change is
made in response to either.

---

## 5. The lesson, recorded so it is not repeated

**An identity between two quantities must be verified against the code that
computes them BEFORE it is frozen as a registered check.** Section 2 asserted
equality between a value-population count and an epoch-population count that
differ by three documented skip rules, and the assertion failed on its first
test. A registered check that fires spuriously is worse than no check, because
it spends attention on a false alarm and trains the reader to discount the next
one.

**Future registered identities in this series must cite the specific lines that
compute both sides.** Section 1 of this document is the standard.
