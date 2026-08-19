# awake-clause-counters-spec-v1-identity-correction-2026-08-18

**Dated 2026-08-18. Supersedes SECTION 6 of `awake-clause-counters-spec-v1`
(`d9cfd5a`) AND NOTHING ELSE.** Sections 1, 2, 3, 4, 5 and 7 are UNCHANGED and
remain authoritative in the original file. The six counters, the two spans, the
pre-onset exclusion, the `AWC_EARLY_MIN` exemption and its expiry, and the
list of what the instrument cannot establish are ALL untouched.

## 1. Why section 6 needed correcting

Section 6 registered this identity:

> **`C1e + C2e + Be + C1l + C2l + Bl` MUST EQUAL the Awake minutes falling at or
> after onset.** It will NOT equal the RESULTS `Awake` total, because RESULTS
> includes pre-onset Awake and these counters exclude it.

**THE QUANTITY ON THE RIGHT-HAND SIDE IS RENDERED BY NO SCREEN.** RESULTS
`Awake` is the POST-smoother total; the counters increment from the
PRE-smoother booleans. **The identity was unevaluable as written, not merely
unevaluated**, and it was recorded NOT PERFORMED on its first attempt rather
than passed.

**THIS IS A RULE 3 DEFECT IN INSTRUMENT FORM.** A check that no reading can
perform is not a falsifiable check. It is the mirror of a criterion that no
reading can fail.

**THE COUNTERS ARE NOT IMPLICATED.** Section 4's guarantee holds - they are
incremented from the SAME booleans the decision reads and cannot disagree with
it. The defect is in what the identity was written against.

## 2. What the corrected identity is

**A NEW RENDERED COUNT IS REQUIRED AND THIS CORRECTION REGISTERS IT:
`AwO` - the count of epochs whose PRE-SMOOTHER stage is Awake and whose index
is at or after `s_onset_epoch_idx`.**

**THE CORRECTED IDENTITY:**
C1e + C2e + Be + C1l + C2l + Bl == AwO

**Both sides are the SAME QUANTITY COMPUTED TWO WAYS** - one accumulated during
the decision, one recovered afterward from stored epochs. **That is what makes
an exact-equality check legitimate here**, and it is the same reasoning that
licenses the `Fd` p50 versus `AD` check.

**A MISMATCH MEANS THE COUNTERS ARE NOT OBSERVING THE DECISION THEY CLAIM TO
OBSERVE AND THE READING IS VOID.** Unchanged in force from the original
section 6.

## 3. Why the identity closes EXACTLY - read from source

Read at `4236e5f`.

**`main.c` 538:** when `!(c1 || c2)` and the stored stage IS Awake, the minute
is NOT skipped - it is rewritten to Light. **After `prv_awake_redecide`
returns, the set of Awake epochs is EXACTLY the set where `(c1 || c2)` held.**

**`main.c` 629:** `prv_base_redecide` skips `StageAwake` and therefore cannot
add to or remove from that set.

**The pre-smoother stage series is written to `EpochRecord.reserved`** and is
what `prv_compute_runs` already walks.

**THEREFORE `AwO` COUNTS PRECISELY THE POPULATION THE SIX COUNTERS PARTITION**,
restricted to the same span, and the identity is exact rather than approximate.
**No tolerance is registered and none is appropriate.**

## 4. Where it is computed and rendered

**Computed inside the EXISTING loop in `prv_compute_runs`** - the pass that
already reads `rec.reserved` for the transition counters and the label onset.
**No new pass, no new traversal, no `EpochRecord` change, no struct growth.**

**Rendered on RUNS**, appended to the existing readout.

**RUNS READS PERSIST.** Unlike DIAG 2, which is destroyed at app exit, the
conditioning check can be re-read at IDLE after the app closes. **This is a
strict improvement in the recoverability of the check** and is the reason RUNS
is the correct screen for it rather than DIAG 2.

**`AwO` IS UNDEFINED WHEN `s_onset_epoch_idx` IS NEGATIVE** and prints `--`,
never `0`, per `measurement-spec-v1` section 3.6. **When onset is undefined all
six counters stay zero by section 2 of the original spec, so an identity
between a zero sum and an undefined count must not be asserted as a pass** - it
is recorded NOT PERFORMED, exactly as this correction's predecessor was.

## 5. What does NOT change

**NO DECISION CHANGES.** `c1`, `c2`, `AW_MOVED_MIN`, the `103`, the window
shape, the anchor and the `(c1 || c2)` disjunction are ALL byte identical.

**NO STAGE, NO MINUTE TOTAL, NO STORED FIELD CHANGES.** `AwO` is derived at
render time from epochs already stored.

**`CLASSIFIER_SERIES` IS NOT BUMPED and `NIGHT_SUMMARY_VERSION` IS NOT
BUMPED.** Same treatment as the six counters and as the transition counters
before them. **A night recorded after the implementing commit remains
comparable on every stage line to every night in `classifier_series 13`.**

**THE PRE-ONSET AWAKE COUNT IS STILL NOT RENDERED and this correction does NOT
add it.** It was never needed for the identity - only for reconciling against
RESULTS `Awake`, which this correction abandons as the referent. **Any future
reconciliation of the counters against RESULTS remains unavailable, and that
limitation is registered rather than hedged.**

## 6. What this still CANNOT establish

**Everything in section 7 of the original spec stands unchanged.** The
corrected identity establishes that the counters observe the decision. **It
establishes NOTHING about whether either clause is correct**, and it carries no
ground truth.

## 7. Status

**Frozen on commit. A change is a v2 or a further dated correction, never an
edit.** **The implementation is a SEPARATE commit and does not precede this
freeze.**
