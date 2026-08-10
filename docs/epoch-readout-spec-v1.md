# Recovery - Epoch Readout Specification v1

**FROZEN 2026-08-10, BEFORE N17 EXISTS.**

This document defines a READ-ONLY, DISPLAY-ONLY change made after N16 resolved
P-H to BOTH WRONG. It is dated before the first night that follows it, for the
same reason docs/smoothing-spec-v1.md (7f6c250), docs/base-spec-v1.md (876a6d6)
and docs/measurement-spec-v1.md (0670f72) were - so that no parameter in it can
have been chosen by looking at the data it will be judged against.

**Nothing in this document amends 7f6c250, 876a6d6 or 0670f72.** All three
remain frozen and untouched. No threshold moves. No stage decision changes.
EpochRecord is untouched. See section 1.

---

## 0. Why this change exists

N16 read Gate 15 while rec_rem_min read 0. Fifteen minutes exceeded 2 x BASE
and none of them reached the reported total. Both anchor hypotheses were
refuted by their own registered criteria - H1 required Vmax far BELOW BASE and
Vmax 205499 is about 67x BASE 3063; H2 required Vmax below GATE and Vmax is
about 34x GATE 6126.

Source reading established the smoother REM-selection cost from the constants
in smoother.c lines 26 to 51. For a REM-observed run of length k past the
latency window, REM is selected only when 656k is greater than 2545, that is
k at least 4; a run of 4 is then absorbed by min_episode_pass, which requires
5 for REM. **Net requirement - 5 consecutive REM-observed minutes are needed
before any REM is reported at all.**

### The two live explanations, which need OPPOSITE fixes

- **Runs of 5 or more existed pre-smoother** - the smoother is not the cause
  and something downstream of it discarded them.
- **Only runs of 1 to 4 existed** - Viterbi and the minimum-episode pass are
  correctly rejecting scattered noise, and the diagnosis is
  sleep_stage_classify(), exactly as smoothing-spec s8 says.

**The distinguishing number is ALREADY STORED.** smoother.c line 215 writes the
pre-smoother stage into rec.reserved before overwriting rec.stage. N16 epochs
hold it right now. **It has no display.** That is the entire gap this spec closes.

---

## 1. Scope and non-scope

**In scope:** reading stored EpochRecords, computing run-length statistics over
the reserved field, and displaying them.

**Explicitly NOT in scope, and untouched:**

- **Every threshold and multiplier.** Not the 2.0x REM multiplier, not the 0.5x
  Light multiplier, not BASE_SAMPLE_MAX 160, not a transition probability, not
  the 0.75 emission confusion, not a REM latency multiplier, not a minimum
  episode duration.
- **sleep_stage_classify(), prv_base_redecide, prv_measure and smoother_run.**
  All unchanged, including every parameter in 7f6c250.
- **EpochRecord.** It has NO version field, so growing it pulls in a
  silent-data-loss blast radius - queued item 9. This spec does not touch it.
- **NightSummary.** No field added, no NIGHT_SUMMARY_VERSION bump. The
  both-sizes storage_night_read path is NOT undone.
- **OSrest and the Awake branch.** Both judged on their own, separately.

**No function that writes an EpochRecord is modified.** The readout calls
storage_epoch_read only and NEVER storage_epoch_update. This is the mechanical
guarantee behind section 8 inertness claim.

---

## 2. What reserved contains

Per base-spec-v1 s1, reserved holds the RE-DECIDED stage, not the raw
classifier stage - the whole-night-BASE Light/REM decision, taken after
prv_base_redecide and before the smoother. **That is precisely the population
that Gate counts.**

**REGISTERED IDENTITY:** the pre-smoother REM minute count MUST EQUAL the night
v_over_gate_count. On N16 both must read 15. A mismatch means reserved does not
hold what this section claims, and **nothing else on the screen is
interpretable** until that is diagnosed.

**Known limitation, recorded so it is not discovered later:** reserved is
written by smoother_run at stop time. A session killed mid-night leaves it
unwritten. N16 completed normally, so this does not affect the N16 readout.

---

## 3. Definition

Over storage_epoch_read(0 .. storage_epoch_count()-1), mapping reserved through
the same stage semantics the smoother uses:

- **rem_total** - count of epochs whose reserved stage is StageREM.
- **rem_runs** - the number of maximal consecutive runs of those epochs.
- **rem_max** - the longest such run, in minutes.
- **r1 / r2 / r3 / r4 / r5p** - the number of runs of length exactly 1, 2, 3,
  4, and 5-or-more.
- **first_off** - the index of the first REM epoch minus the onset index, where
  onset is computed by the SAME ONSET_RUN 5 consecutive non-Awake rule that
  find_onset uses, applied over reserved. Negative if the first REM epoch
  precedes onset. Displays as a dash if no onset is found or no REM exists.
- **ep_n** - storage_epoch_count(), displayed so that a zeroed count is visible
  rather than silent.

**Integer only. No floats. No sorting. Single pass over the epochs.**

---

## 4. Display

A NEW RUNS screen, reached from DIAG by DOWN; UP or BACK returns to DIAG.
**RESULTS, HYPNO and DIAG are unchanged**, so every existing recorded-value
convention and every prior night screen-reading procedure still applies.

Layout, one screen:

    Runs
    RemN <rem_total> Runs <rem_runs>
    Max <rem_max> Ep <ep_n>
    L1 <r1> L2 <r2> L3 <r3>
    L4 <r4> L5p <r5p>
    Off <first_off>

Values are computed at screen open, held in locals, and NOT persisted. A value
that is undefined displays as a dash and **never as 0** - zero is a meaningful
measured value here and must not be confused with not-recorded.

**RUNS is diagnostic-only and joins IDLE and DIAG under queued item 9.** It is
not a release screen.

---

## 5. Implementation constraints

- **No EpochRecord change. No NightSummary change. No version bump.**
- **No new static array.** Single pass, one EpochRecord on the stack.
- **EPOCHS_PER_KEY and MAX_EPOCH_KEYS unchanged. storage_epoch_read unchanged.**
- **Integer only. No floats.**
- **Single commit** on ~/recovery main - git commit -s plus the
  Co-Authored-By Claude noreply trailer, both counts verified equal to 1,
  karthakon identity, before push.

---

## 6. THE ORDER THIS MUST BE DONE IN - mechanical, not preference

**storage_session_start zeroes epoch_count (storage.c line 18), and
storage_epoch_read refuses any index at or above the count. Starting ONE
recording makes N16 epochs unreadable.** Only one night of epochs exists at a
time - KEY_EPOCH_BASE is a single overwritten buffer, unlike the slotted night
summaries.

1. Install the build.
2. **Open RUNS and read N16 numbers BEFORE starting any session.**
3. Only then pre-register and record N17.

**If step 2 is skipped, N16 adjacency answer is destroyed and cannot be
recovered.**

**Install risk, stated as a HYPOTHESIS and not a finding:** persist data is
keyed to the app UUID (020d4026-08ea-488c-9884-3cabaf9b4337, unchanged), and
the cc305dd install preserved stored night summaries - but that install was NOT
verified against EPOCH records, which have no both-sizes read path. EpochRecord
is unchanged by this spec so its layout matches. **If ep_n reads 0 after
install, the epochs did not survive, the readout is empty, and that must be
REPORTED - do not re-record a night to get data, because a new night cannot
answer a question about N16.**

---

## 7. What the readout CAN and CANNOT establish

### CAN

Whether pre-smoother REM existed as runs of 5 or more or only as fragments; how
many runs there were; how long the longest was; where the first sits relative
to onset; and whether the section 2 identity holds.

### CANNOT

1. **What the multiplier should be.** Measuring a run-length distribution does
   not license fitting a threshold to it. Any threshold change is a separate
   dated spec, frozen before the night that tests it.
2. **Anything about spread.** One night has no spread.
3. **Whether REM is truly absent.** There is no ground truth. Garmin is not
   ground truth.
4. **Whether the classifier REM minutes are CORRECT.** Only whether they were
   contiguous. A run of 6 spurious minutes and a run of 6 real REM minutes are
   indistinguishable on this screen.

### Registered outcomes - ALL are legitimate results

- **r5p greater than 0** - the smoother had qualifying runs available and
  something else discarded them. Points DOWNSTREAM of smoother_run.
- **r5p equal to 0 with rem_total 15** - the smoother behaved exactly as
  specified and the diagnosis is the classifier, per smoothing-spec s8.
- **rem_total not equal to 15** - the section 2 identity FAILED. Diagnose that
  before reading any other field.
- **ep_n equal to 0** - the epochs did not survive the install. Report it.

---

## 8. Comparability

**recovery_commit changes, so N17 opens a NEW comparability group** by the group
definition already in force. classifier_series stays **6** and smoother_version
stays **v1**, because neither the classifier nor the smoother changes.
firmware_version stays **v4.32.0** with no flash - an app install is not a
firmware flash - so watch_slot stays **1**.

**Registered honestly, in advance:** because this change is display-only and no
function that writes an EpochRecord is modified, N17 staging numbers are
mechanically comparable to N13 through N16 even though the group key differs.
**That is an argument for reading them together, NOT a licence to merge the
groups or to extend the closed group spread result.** The 1.72x result was
reported on three nights and stays reported on three nights.
