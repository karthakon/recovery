# awake-anchor-readout-spec-v1

**Dated 2026-08-19. An INSTRUMENT ONLY. Frozen before implementation.**

**Reads with `classifier-spec-v3` (`2ae829a`) section 4.2 and
`awake-clause-counters-spec-v1` (`d9cfd5a`) as corrected by
`awake-clause-counters-spec-v1-identity-correction-2026-08-18.md` (`e8fe888`).
CITED, NOT RESTATED.**

## 1. Why this exists

**The Awake decision is a disjunction of two clauses and the second one compares
a windowed heart-rate median against a reference that NO SCREEN RENDERS.**

Read from source at `6cfc319`:

- `main.c` 477 declares `a_hr` as a LOCAL inside `prv_awake_redecide`.
- `main.c` 498 assigns it `s_anchor_scratch[k / 2]`, the upper-middle element
  of the sorted qualifying set.
- `main.c` 523 to 524 is the only read: c2 holds when
  `s_epoch_hf[i] * 100 > a_hr * 103`.
- `a_hr` is never written to any static, never persisted, and never rendered.
  A grep for `a_hr` across `main.c` returns exactly those four sites.

**`s_anchor_hr`, rendered on DIAG 1 as `AH`, IS A DIFFERENT QUANTITY.** It is
assigned at `main.c` 684 from `prv_compute_anchor`, whose population is
non-Awake minutes across the whole night (`main.c` 562 to 583). `a_hr`'s
population is minutes at or after onset with `still && known` movement
(`main.c` 479 to 490). **The two medians are computed over different sets by
different functions and neither may be substituted for the other.** `AH` is in
part a median of minutes that c2 did not fire on, so it cannot serve as a proxy
for the threshold c2 used.

**CONSEQUENCE, AND IT IS THE REASON THIS SPEC EXISTS: the six clause counters
report HOW OFTEN c2 fired and cannot report WHAT IT COMPARED AGAINST.** A
reading in which c2 is the sole clause is therefore uninterpretable at the
level that matters, because the threshold is `a_hr * 103 / 100` and `a_hr` is
invisible.

**IT IS NOT RECOVERABLE RETROACTIVELY.** `a_hr` depends on `s_epoch_still` and
`s_epoch_mv_known`, which `awake-clause-counters-spec-v1` section 2 and the
v59 source reads establish are RAM-only bitmaps with no persist site.
`EpochRecord` carries no movement field. **A past night's `a_hr` cannot be
recomputed from stored epochs.** The instrument must exist before a night, not
after it, for the same reason the clause counters did.

## 2. What is rendered

**TWO VALUES, BOTH ALREADY COMPUTED, MIRRORED INTO RAM STATICS AT THE POINT OF
COMPUTATION:**

- **`Ah`** - the value of `a_hr` at the end of `prv_awake_redecide`'s anchor
  block, in beats per minute.
- **`k`** - the count of qualifying minutes the median was taken over, the same
  `k` the guard `k >= A_MIN_MINUTES` tests.

**`Ah` IS UNDEFINED WHEN `a_hr` IS ZERO** - either onset is undefined or fewer
than `A_MIN_MINUTES` minutes qualified. **It renders `--` and NEVER `0`**, per
`measurement-spec-v1` section 3.6. **`k` renders its true value in both cases**,
because a zero or small `k` is the diagnosis when `Ah` is undefined.

**`Ah --` IS NOT A ZERO AND IS NEVER READ AS ONE.** When `a_hr` is zero, c2
cannot fire at all - the `a_hr > 0` guard at `main.c` 523 is explicit - so
`Ah --` alongside non-zero clause counters would mean c1 produced all of them.

**NO THIRD VALUE IS ADDED.** The threshold itself is not rendered: it is
`Ah * 103 / 100` and computing it on the watch would put a derived number on a
screen where the two inputs are already present. **Deriving it at scoring time
from `Ah` is not a Rule 6 violation** because the constant is frozen in source
and the input is read from a screen, exactly as `Off` is read against `OnsL`.

## 3. What does NOT change

**NO DECISION, NO THRESHOLD, NO WINDOW SHAPE, NO CONSTANT AND NO STORED FIELD.**

- `a_hr` is computed exactly as before, at the same site, over the same
  population. **The mirror is an assignment, not a recomputation.**
- c1, c2 and the disjunction are byte-identical.
- The six clause counters are untouched.
- No `EpochRecord` field is added. No `NightSummary` field is added.
- **`CLASSIFIER_SERIES` is NOT bumped and `NIGHT_SUMMARY_VERSION` is NOT
  bumped.** Same treatment as the transition counters at `a7d6939`, the clause
  counters at `4236e5f` and `AwO` at `b304e1f`. **The bump rule is OUTPUT
  CHANGES and this changes no output.**
- **This spec opens NO comparability group.** A night carrying it is comparable
  on every stage line to a night without it, all else equal.

**VERIFICATION REQUIRED BEFORE COMMIT, NOT ASSERTED HERE:** read
`CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION` from `storage.h` after the
edit, and confirm `git diff --stat` names `src/c/main.c` alone.

## 4. Render

**The two values go on DIAG 2, NOT DIAG 1.**

Read from source at `6cfc319`: `prv_draw_diag2` begins at `main.c` 1084 with
`y = 2`, draws a 26-pixel header, then six 18-pixel lines - the two `Fd` lines,
`Vib`/`QT`, `Unk`, and the two clause-counter lines - and its final draw has no
trailing `y` increment. `prv_draw_diag` fills the same screen with nine lines.

**DIAG 2 IS THE CORRECT SCREEN FOR TWO REASONS.** It has the vertical room that
DIAG 1 does not, and **the DIAG split was forced once already when a line fell
off the bottom of a full screen** - that is a recorded defect and not a
hypothetical. And `Ah` belongs beside the clause counters it explains, not
beside the REM anchor it is routinely confused with.

**THE LINE IS APPENDED AS THE SEVENTH AND LAST**, below `C1l`:

```
Ah <n>  k <n>
```

**The existing final draw gains a trailing `y += 18` in the same edit.** Omitting
it would overprint the new line on the old one.

**THE LABEL IS `Ah` AND NOT `AH`, DELIBERATELY.** `AH` is taken, is on the
adjacent screen, and denotes the other anchor. **A reader who confuses them
draws exactly the wrong conclusion**, which is a mistake this project has
already made once in a working session and caught before it reached a file.

## 5. Capture

**DIAG 2 RENDERS FROM RAM STATICS AND DOES NOT SURVIVE APP EXIT.** `Ah` and `k`
are lost with it. They carry the same capture discipline as the clause counters
and the same consequence: **if capture 4 is missed, the values are gone and no
re-read recovers them.**

Unlike `AwO`, `Ah` does NOT depend on a value that resets on app launch - it is
lost only with the screen, not before it.

## 6. What this CANNOT establish

**WHETHER c2 IS WRONG.** The readout reports the reference and its population
size. **A reference is not thereby mistaken**, and nothing here carries ground
truth.

**WHETHER THE THRESHOLD IS TOO LOW.** That is a question about what the
reference MEANS, not about its value, and no single rendered number answers it.

**WHAT THE DISTRIBUTION LOOKS LIKE.** `Ah` is one order statistic and `k` is a
count. **The per-minute HF series is still not rendered anywhere** and remains
the instrument that would settle the early-versus-late question directly. This
spec does not build it and does not substitute for it.

**ANYTHING ABOUT A PAST NIGHT.** No prior night's `Ah` is recoverable.

**ANYTHING FROM ONE NIGHT.**

## 7. Rule 2 check

**NO CONSTANT IS INTRODUCED.** `Ah` and `k` are a measured value and a count,
both with no band, no threshold and no expected value. **The 103 is not moved
and is not read differently.** `A_MIN_MINUTES` is not moved. `AWC_EARLY_MIN`
is not moved and nothing new branches on it.

**NO BRANCH READS EITHER NEW STATIC.** They are written once and read once, in
a draw call.

## 8. Rule 3 check

**THIS SPEC REGISTERS NO CRITERION AND MAKES NO PREDICTION.** It is an
instrument. **A pre-registration may register a criterion against `Ah` once it
has read at least once**, and doing so before any reading exists would be
inventing a band from nothing.

**THE RENDER-SITE CHECK, WHICH IS NOW MANDATORY:** both values are rendered by
DIAG 2, a screen every night captures. **Neither side of any future criterion
built on them is invisible.** This spec exists precisely because that check
failed for the quantity it exposes.

## 9. Status

**Frozen on commit. Implemented in a SEPARATE commit. No frozen spec is edited
by this file and this file supersedes nothing.**
