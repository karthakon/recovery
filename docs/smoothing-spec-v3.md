# smoothing-spec-v3 — REM resumption rule

**Frozen 2026-08-16.** SUPERSEDES NOTHING in `smoothing-spec-v1.md`
(`7f6c250`) or `smoothing-spec-v2` (`469a8c7`). Both remain authoritative in
full: the transition matrix, the emission model, the REM-latency table with
its two promoting rows, `MIN_EP_REM 5`, `MIN_EP_LIGHT 3`, `MIN_EP_AWAKE 2` and
`MIN_EP_PASSES 3` are ALL UNCHANGED. This file ADDS one repair pass.

## 1. The finding that prompted it

The transition counters frozen in
`epoch-readout-spec-v1-transition-counters-2026-08-16.md` measured, on the
last recorded night, Awake -> REM transitions in BOTH the pre-smoother and the
final series, and measured MORE of them than transitions in the
physiologically ordinary REM -> Awake direction. Per RULE 7 the counts are
not reproduced here; they are recorded in the night file.

## 2. Why the classifier cannot fix this itself

`prv_base_redecide` decides each minute on `v > A_D && t2 && t3`. No term in
that decision reads the previous minute. Awake is decided live and is never
overwritten, and the re-decide pass skips Awake minutes entirely. **The
classifier is memoryless by construction.** The smoother is the only component
in which sequence is represented at all, so a sequence rule belongs here. This
is not a workaround for a classifier defect.

## 3. Why the rule is CONDITIONAL and not a blanket prohibition

Awake -> REM is implausible as an ENTRY into REM from sustained wakefulness.
It is entirely normal as a RESUMPTION: a brief arousal in the middle of a REM
period, after which REM continues. Conventional scoring treats the latter as
one REM period, not two. **A blanket prohibition would suppress real
physiology.** The rule must separate the two cases.

## 4. The rule

`REM_RESUME_LOOKBACK = 15` epochs. One epoch is one minute.

For every `t` where `s_path[t] == REM` and `s_path[t-1] == Awake`:

1. Walk back to `aw_start`, the first epoch of that Awake episode.
2. Examine `s_path[aw_start - 15 .. aw_start - 1]`, clamped at 0.
3. **Any REM in that window** -> RESUMPTION. The transition stands untouched.
4. **No REM in that window** -> ENTRY FROM WAKE. The entire REM episode
   beginning at `t` is rewritten to Light.

The lookback window ends at `aw_start`, NOT at `t`, so the length of the
Awake episode itself does not consume the window.

## 5. Where 15 comes from — Rule 2 statement

**15 minutes is the conventional separation below which two REM bouts belong
to the SAME REM period.** A wake gap shorter than that leaves the sleeper
within that period, so REM returning after it is a continuation. The constant
is taken from a scoring DEFINITION and from no measurement on any night. It
would be the same number on a night of any length and after any struct change.
**No reading from any night was consulted in setting it, and it may not be
tuned by one.**

## 6. Ordering, and a consequence accepted deliberately

The pass runs AFTER `min_episode_pass` and therefore has the last word:
Viterbi -> `MIN_EP_PASSES` min-episode passes -> resumption repair ->
write-back.

**This can leave a Light episode shorter than `MIN_EP_LIGHT`.** That is
accepted, not overlooked. The min-episode rule is a smoothing heuristic; the
prohibition on entering REM from wake is physiology, and physiology outranks
smoothing. Re-running min-episode afterwards would let a demoted-to-Light
episode be absorbed back toward REM and defeat the repair.

## 7. What this does NOT do

- **`TRANS` is NOT edited.** The matrix is first-order: `cost[i] + TRANS[i][j]`
  sees exactly one step back and cannot express a 15-minute lookback. The
  0.017 Awake->REM entry stays exactly as frozen in v1.
- No threshold, anchor, veto or emission value moves.
- The classifier is not touched. `t2`, `MIN_EP_REM` and `MIN_EP_PASSES` stay
  put.
- Deep remains HELD.

## 8. Comparability

**OUTPUT CHANGES. `CLASSIFIER_SERIES` 12 -> 13.** A new comparability group
opens. Every stage line reopens with no comparison set. The series that do not
depend on staging output are unaffected.

## 9. Rule 3 vacuity check

Both branches of the rule are reachable: a lookback window containing REM and
one not containing REM are both ordinary readings, and which one fires is
decided by the night.

**The registered falsifier:** after this pass, a final-series Awake -> REM
transition may ONLY be a resumption. **A non-zero post-smoother `A>R` count
whose lookback window contains no REM would mean this pass is broken.** That
reading is available on every night from the RUNS transition counters.

A second observable: the pre-smoother `A>R` count is untouched by this change
by construction, since the pass rewrites `s_path` and never `rec.reserved`.
**A pre-smoother count that moves would indicate something other than this
change is acting.**
