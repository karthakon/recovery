# Measurement spec v1 - feature correction

**Dated 2026-08-10. Supersedes `0670f72` section 3.1 and the `v_over_gate_count`
definition in section 3.2. `0670f72` is NOT edited.**

## 1. What changed and why

`docs/classifier-spec-v1.md` (`c5edaee`, dated 2026-08-10) replaces the classifier's
per-minute feature. Two consequences fall on the measurement spec:

1. **Section 3.1's population changes contents.** `s_epoch_var[0 .. s_epoch_var_count-1]`
   held `hrv_ppi_variance(&s_minute_buf)`, a plain per-minute variance. From
   `classifier_series 7` it holds `D(m)`, the squared median absolute deviation of the
   minute's accepted PPI about its median (classifier-spec section 3.1).
2. **Section 3.2's `v_over_gate_count` loses its comparand.** It was defined as the count
   of qualifying entries where `v > base_final * 2`. Classifier-spec section 3.4 anchors
   the REM decision on `A`, the night median of the windowed feature `F`, not on BASE. A
   field comparing against `base_final * 2` would no longer audit any decision the
   classifier makes.

**Why this is a correction and not a retune:** no threshold moves. The 2.0x multiplier is
unchanged in both documents. What changes is *which statistic* is compared and *which
anchor* it is compared to - both consequences of a classifier change already frozen, not
independent parameter choices.

## 2. Replacement definitions

**Section 3.1 replacement.** Statistics are computed over the entries of
`s_epoch_var[0 .. s_epoch_var_count-1]` for which the value is greater than 0. The
entries hold `D(m)` per classifier-spec section 3.1. **Everything else in section 3.1
stands unamended**: the zero-exclusion rationale, `v_count` as the recorded qualifying
count, and `v_count`'s role in detecting `EPOCH_VAR_MAX 960` truncation.

**`v_over_gate_count` replacement.** The number of minutes for which `F(m) > 2 * A`, per
classifier-spec section 3.4. This is the count of minutes the classifier decided REM
before the smoother. **It remains recorded so that zero is a measured value rather than
an inference from REM being 0** - the original purpose in `0670f72` section 3.2,
preserved.

**Registered relation: `RemN <= Gate` STANDS for now.** The relation frozen by `465f93e`
is carried unchanged into this correction. **The equality `RemN == Gate` is EXPECTED by
construction under the new feature** - both sides will run the same test over the same
population - **but it is NOT registered here, because the implementation that would make
it true does not yet exist and an identity must be verified against the code computing
both sides before it is frozen** (`465f93e` process lesson 1).

The two counts are computed **independently and from different sources**, which is what
makes the comparison worth registering at all:

- `RemN` comes from `prv_compute_runs`, reading **persist storage** via
  `storage_epoch_read`, counting epochs whose stored `stage` is REM.
- `Gate` comes from `prv_measure`, reading the **RAM array** `s_epoch_var`, counting
  values passing the threshold test.

**Action required at implementation, before N17 exists:** read both call sites together
and confirm whether the Awake-skip and `beat_count < 20` rules land on the same side of
both counts. Then either register `RemN == Gate` in a dated addendum to this correction,
or record why it cannot hold. **Either way the relation is pre-registered before the
night that tests it.**

**This relation is an internal consistency check on the instrument, NOT an acceptance
criterion for the night's results.** A failure means one counter is wrong or a skip rule
fires asymmetrically. The night's REM is judged by the pre-registered lines and the RUNS
distribution, never by this line.

## 3. What stands unamended

`0670f72` sections 0, 1, 2, 3.3 (sorting, in place, only after `prv_base_redecide`), 3.4
(forced ordering), 3.5 (persistence), 3.6 (display layout), 4, 5 and 6 all stand.
Classifier-spec section 4.3 places the new pass after `prv_base_redecide`, so section
3.3's precondition is satisfied unchanged.

`v_max`, `v_median`, `v_p90`, `base_min` and `base_max` keep their definitions exactly:
index arithmetic, integer only, upper-middle median, no averaging, no floats. **Only the
contents of the population change, not how the statistics are taken over it.**

`NIGHT_SUMMARY_VERSION 2` unchanged. No storage layout change. No new column: count stays
**81**.

## 4. What these numbers are, and are not

`v_max`, `v_p90`, `v_median` and `v_over_gate_count` measure the HRV signal delivered by
the API added in PR #1670, as processed by `hrv_buf_add`'s quality, range and jump gates.
**They are not a Garmin-referenced quantity and are not calibrated against any external
device.** Pebble Health does not carry PPI at all - it reports heart rate, and its
Light/Deep classification is accelerometer-only - so it is a comparison line, never a
source for these figures.

**No statistic in this spec may be adjusted to improve agreement with Garmin or with
Pebble Health.** Divergence is evidence about the instruments; a tuned statistic is
evidence about nothing.

## 5. Comparability

`rec_v_max`, `rec_v_p90`, `rec_v_median`, `rec_v_count`, `rec_v_over_gate`,
`rec_base_min` and `rec_base_max` keep their names and CSV positions. **From N17 forward
they measure `D(m)`; N1-N16 measured raw per-minute variance. The two are not comparable
and no cross-group trend may be drawn on them.** The `classifier_series 6 -> 7` boundary
already marks this; no night N1-N16 is edited or recomputed.

`rec_base_min` / `rec_base_max` are unaffected in contents - they are taken over
`s_base_samples`, which classifier-spec section 2 leaves out of scope.

## 6. Status

**Frozen on commit. `0670f72` remains the governing spec except where superseded here. A
further change is a v2 or another dated correction, never an edit.**
