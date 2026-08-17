# epoch-readout-spec-v1 — transition counters and HYPNO hour ticks

**Dated addition, 2026-08-16. SUPERSEDES NOTHING.** `epoch-readout-spec-v1.md`
(`413e850`), its nav correction (`dff61ec`) and its s2 correction (`465f93e`)
remain authoritative in full. This file ADDS one RUNS line and one HYPNO
overlay. No frozen section is edited.

## 1. Why

The Viterbi transition matrix in `smoother.c` permits Awake -> REM at 0.017.
It is DISCOURAGED, not forbidden. An Awake epoch followed directly by a REM
epoch is physiologically implausible: REM is not entered from wakefulness in
normal sleep architecture. Whether the classifier or the smoother emits such a
transition has never been measured, because no rendering of the per-minute
sequence exists.

`prv_draw_hypno` cannot answer it. It draws one rect per epoch at
`x0 = 4 + i * plot_w / n`. When `n` exceeds the plot width in pixels, several
epochs map to the same column and the last drawn OVERPAINTS the others. The
hypnogram is a SAMPLE of the sequence, not the sequence. Adjacency is not
recoverable from it at any night length past the plot width.

## 2. What is added — RUNS

Four `uint16_t` counters in `RunStats`, accumulated in the existing
`prv_compute_runs` epoch loop, rendered as one line above the `Ons`/`OnsL`
line:

A>R <pre>/<post> R>A <pre>/<post>

- `tr_ar_pre`  — count of `t` where `reserved[t-1] == StageAwake` and `reserved[t] == StageREM`
- `tr_ar_post` — the same over `stage[t-1]`, `stage[t]`
- `tr_ra_pre`  — count of `t` where `reserved[t-1] == StageREM` and `reserved[t] == StageAwake`
- `tr_ra_post` — the same over `stage[t]`

`reserved` holds the PRE-smoother stage (`smoother.c` writes it before
overwriting `rec.stage`); `stage` holds the final. Counting both separates a
classifier-emitted transition from a smoother-emitted one. An epoch that
`storage_epoch_read` fails on breaks the chain — the pair spanning it is not
counted and no epoch is fabricated.

`R>A` is the physiologically ORDINARY direction and is counted as the control.
A reading of `A>R 0` alongside `R>A 0` means the instrument saw no transitions
of either kind, which is a different fact from Awake -> REM being absent.

## 3. What is added — HYPNO

Black vertical lines at `i = 60, 120, ...`, drawn AFTER every bar so a tick is
never overpainted. One epoch is one minute, so these are hour boundaries.
Cosmetic. Reads no stage.

## 4. What this does NOT do

**No decision changes. No threshold moves. No stored field changes. No
`EpochRecord` change. `CLASSIFIER_SERIES` is NOT bumped and does not need to
be — identical epoch input produces identical stage output.** Nights recorded
before and after this commit remain comparable on every stage line.

This is READ-ONLY over persisted epochs: `storage_epoch_read` only, never
`storage_epoch_update`.

## 5. Whether to forbid the transition — NOT DECIDED HERE

Forbidding Awake -> REM would mean setting that TRANS entry to an effectively
unreachable cost, or adding a re-decide rule. **That is a decision for after a
count exists, and it is a decision that WOULD change output and WOULD bump
`CLASSIFIER_SERIES` and WOULD open a comparability group.** Nothing in this
file authorises it.

## 6. Retroactive read

Persist survives install. The counters are computed at render time from stored
epochs, so installing this build and opening RUNS from IDLE reads the counts
for the LAST RECORDED NIGHT without recording a new one. **A new recording
zeroes `epoch_count` and destroys that opportunity permanently.**

## 7. Rule 3 vacuity check

`A>R` can read zero and can read non-zero; both are informative and both are
reachable. The `pre`/`post` split can disagree in either direction. No clause
here is a prediction and none is scored — this file freezes an INSTRUMENT, not
a criterion.
