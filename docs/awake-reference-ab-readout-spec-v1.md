# awake-reference-ab-readout-spec-v1

**Dated 2026-08-19. An INSTRUMENT ONLY. Frozen before implementation.**

**Reads with `classifier-spec-v5` (`20f48d7`), `classifier-spec-v3` (`2ae829a`)
section 4.2, `awake-clause-counters-spec-v1` (`d9cfd5a`) as corrected by
`e8fe888`, and `awake-anchor-readout-spec-v1` (`ed504f3`) as superseded in part
by v5 section 7. CITED, NOT RESTATED.**

## 1. Why this exists

**`classifier-spec-v5` bumps `CLASSIFIER_SERIES` and therefore breaks
comparability with every prior night on every stage line.** No Awake figure from
before the bump may be compared to one after it. **The change is consequently
UNATTRIBUTABLE by the only method the project has ever used** - reading a
criterion on the next night and comparing it to the last.

**A second confound compounds the first:** the first night under v5 is expected
to differ from the last night before it in more than the classifier. **Even a
large movement in Awake would have at least three candidate causes and the
night would separate none of them.**

**THIS IS NOT SOLVABLE BY WAITING.** A later night is a further night, not a
comparable one, and the comparability boundary does not heal.

**IT IS SOLVABLE BY COMPUTING BOTH REFERENCES ON THE SAME NIGHT.** Every input
both need is already resident in RAM at the point the decision runs. **The
comparison then happens WITHIN one night, against one set of heart-rate values,
one movement bitmap and one onset - so the comparability break does not reach
it.**

## 2. What is counted

**THE WHOLE-NIGHT REFERENCE IS RETAINED AS A COUNTERFACTUAL.** It is computed
exactly as `classifier-spec-v3` section 3.5 specified and as the code computed
it before v5 - the median of `HF` over minutes at or after onset with
`still && known` movement and `HF` defined, upper-middle element, no averaging,
undefined below `A_MIN_MINUTES`.

**NOTHING READS IT. IT DECIDES NOTHING.** It exists to be counted against.

**TWO COUNTERS, BOTH OVER MINUTES AT OR AFTER `s_onset_epoch_idx`** - the same
span the six clause counters use, per `d9cfd5a` section 2:

- **`ABw`** - minutes where c2 WOULD have fired against the whole-night
  reference.
- **`ABl`** - minutes where c2 DID fire against the time-local reference.

**Both apply the identical test** - `HF(m) * 100 > REF * 103` with the
`REF > 0` guard - **differing ONLY in which reference is supplied.** The 103 is
not moved and no threshold differs between the two arms.

**PRE-ONSET MINUTES ARE COUNTED IN NEITHER**, for the same reason `d9cfd5a`
section 2 excludes them: Recovery always starts Awake because the user presses
the button while awake, and including that would put a definitional artifact
into a diagnostic.

**WHEN A REFERENCE IS UNDEFINED ITS ARM COUNTS ZERO FOR THAT MINUTE**, because
the guard makes the clause unable to fire. **A zero from an undefined reference
and a zero from a reference that was not exceeded are NOT distinguished by these
counters** - `Ah`, `k` and `Ahr` are what distinguish them.

## 3. The registered identity

**`ABl` MUST EQUAL `C2e + C2l + Be + Bl`.**

Both sides count minutes at or after onset where c2 held, by two paths: one
accumulated in the clause-counter partition, one accumulated directly.
**BOTH SIDES ARE THE SAME QUANTITY COMPUTED TWO WAYS and the exactness is the
point** - the same reasoning that makes P-AWO and the `Fd` p50 check
legitimate exact-equality tests rather than instances of the mirror-image
defect.

**A MISMATCH MEANS THIS INSTRUMENT IS NOT COUNTING WHAT IT CLAIMS TO COUNT AND
`ABw` IS VOID FOR THAT NIGHT.** It does NOT invalidate the clause counters,
which have their own conditioning check.

**Both sides are rendered by DIAG 2 and the check is scorable without
arithmetic on a prior night.**

## 4. What this establishes, and what it does NOT

**IT ISOLATES THE CLAUSE, NOT THE NIGHT.**

**`ABw` IS NOT "WHAT AWAKE WOULD HAVE BEEN UNDER THE OLD REFERENCE" AND MUST
NEVER BE READ AS THAT.** A different c2 set produces a different Awake set,
which changes what `prv_base_redecide` skips, which changes the anchors, which
changes the smoother's input, which changes every total on RESULTS. **The
counterfactual is FIRST-ORDER ONLY and every downstream effect is unmodelled.**

**WHAT IT DOES ESTABLISH: how many minutes the reference change moved, in the
clause where the change was made, on one night, against one set of inputs.**
That is the attribution question v5 raises and it is answerable no other way.

**IT CANNOT SAY WHICH ARM IS CORRECT.** Neither count carries ground truth.
**A clause firing less is not thereby firing rightly** - `classifier-spec-v5`
section 5 registers the collapse-toward-zero outcome as a REFUTATION, not a
success, and this instrument is what would show it happening at the clause
level.

**IT CANNOT SEPARATE A REFERENCE EFFECT FROM A NIGHT EFFECT ACROSS NIGHTS.**
The within-night comparison is valid within its night and pooling it across
nights is not licensed by this file.

**ANYTHING FROM ONE NIGHT.**

## 5. What does NOT change

**NO DECISION, NO THRESHOLD, NO WINDOW SHAPE, NO CONSTANT AND NO STORED
FIELD.**

- **c2 continues to read the time-local reference and only that.** The
  whole-night value is computed and never consulted by any branch that affects
  a label.
- c1, the disjunction, the 103, `A_MIN_MINUTES`, `A_HR_WIN`, the population
  filter and onset are all untouched.
- The six clause counters, `AwO`, the movement readout and every REM term are
  untouched.
- No `EpochRecord` or `NightSummary` field is added.
- **`CLASSIFIER_SERIES` is NOT bumped and `NIGHT_SUMMARY_VERSION` is NOT
  bumped.** Same treatment as `a7d6939`, `4236e5f`, `b304e1f` and the two
  readout specs. **The bump rule is OUTPUT CHANGES and this changes no output.**
- **This spec opens NO comparability group.** It rides inside the group v5
  opened.

**VERIFICATION REQUIRED BEFORE COMMIT, NOT ASSERTED HERE:** read
`CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION` from `storage.h` after the edit
and confirm both are unchanged from their post-v5 values.

## 6. Render

**THE WHOLE-NIGHT REFERENCE JOINS THE EXISTING `Ahr` LINE:**

```
Ahr <min> <max> w<whole>
```

**AND ONE NEW LINE IS APPENDED:**

```
ABw <n>  ABl <n>
```

**THIS IS THE ELEVENTH LINE ON DIAG 2 AND ELEVEN IS NOT PROVEN TO FIT.** Ten is
proven, and the overflow recorded in the DIAG-split comment - which pushed a
field off the bottom unnoticed - sits in this neighbourhood. **THE RENDER MUST
BE VERIFIED ON THE WATCH BEFORE THIS SPEC IS TREATED AS IMPLEMENTED.**

**IF ELEVEN DOES NOT FIT, THE LINE MOVES TO DIAG 1**, which renders nine and has
the same proven geometry. **It is never dropped and the font is never shrunk** -
that trade was considered and rejected once already when DIAG was split rather
than compressed.

**`w<whole>` RENDERS `--` WHEN THE WHOLE-NIGHT REFERENCE IS UNDEFINED**, per
`measurement-spec-v1` section 3.6. **A `--` there with a non-zero `ABw` would be
a contradiction and means the instrument is broken.**

**DIAG 2 RENDERS FROM RAM STATICS AND DOES NOT SURVIVE APP EXIT.** Same capture
discipline and same consequence as the clause counters: **if capture 4 is
missed these values are gone and no re-read recovers them.**

## 7. Cost

**ONE ADDITIONAL PASS OVER AN ARRAY ALREADY RESIDENT, plus one median over a
population bounded by the night.** `s_epoch_hf`, `s_epoch_still` and
`s_epoch_mv_known` are RAM. **NO `storage_epoch_read` CALL IS ADDED.**

**`s_anchor_scratch` IS REUSED AND THE ORDERING MATTERS.** The whole-night
median must be taken at a point where nothing live occupies it. **Verify from
source before writing** - the same check `classifier-spec-v5` section 8
required.

**Three `uint16_t` of static RAM.**

## 8. Rule 2 check

**NO CONSTANT IS INTRODUCED.** `ABw`, `ABl` and the whole-night reference are
two counts and a measured median, with no band, no threshold and no expected
value. **The 103 is shared between both arms deliberately, so the comparison
isolates the reference and nothing else.**

**NO BRANCH READS ANY NEW STATIC.** Each is written once and read once, in a
draw call.

## 9. Rule 3 check

**ONE CRITERION IS REGISTERED AND IT CAN FAIL:** the section 3 identity, on any
mismatch.

**`ABw` AND `ABl` THEMSELVES ARE RECORDED, NOT SCORED.** No band is registered
on either and none may be, because neither has read once. **A dominance
comparison between them may be registered in a pre-registration** - that is a
comparison between two measured quantities with no invented constant, the same
shape as P-CLAUSE.

**THE RENDER-SITE CHECK:** every value above is rendered by DIAG 2, subject to
section 6's eleven-line verification. **If a value proves unrenderable it is
moved, and no criterion is registered against it until it renders.**

## 10. Status

**Frozen on commit. Implemented in a SEPARATE commit. No frozen spec is edited
by this file and this file supersedes nothing.**
