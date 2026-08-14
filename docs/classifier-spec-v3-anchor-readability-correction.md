# Classifier spec v3 - anchor readability correction

**Dated 2026-08-14. Supersedes the READABILITY of section 6.1 limb 1 and of section 6.3's
`A_D` sentence, and registers WHEN section 6.1's discriminating branch becomes scoreable.
`2ae829a` is NOT edited and remains governing in every other part. NO band moves, NO branch
is removed, NO prediction is withdrawn and NO constant is retuned.**

## 1. What was wrong, and what was not

**Section 6.1 registers that `anchor_hr` FALLS on series 9. Nothing rendered `anchor_hr`.**
Grepped at `60c378e`: **nine sites, all in `main.c`** - the out-parameter and its zeroing in
`prv_compute_anchor`, the guarded fill and the `A_H` median assignment, the
`prv_base_redecide` parameter and the `t2` test, and the locals in `prv_stop_recording`.
**No persist site. No render site.** A stack local that dies when `prv_stop_recording`
returns. **The same is true of `A_D`**, which section 6.3 records as UNKNOWN in sign.

**The prediction was not defective. The instrument was missing.** This is the `acf88e4`
shape; `acf88e4` section 4 states expressly that section 6.1 and BOTH its branches stand
unchanged, and this correction does not disturb that.

## 2. What the first series-9 night could and could not do

**It scored limb 2 and could not score limb 1.** `RemN` FELL relative to the two series-8
nights - the direction limb 2 does not register. Values are in the night record; they are
not reproduced in this repository.

**The discriminating branch was NOT scored, and that night's record registers it as a
STOP**, for two reasons. Limb 1 was unreadable, so the registered qualifier of no change to
the 2.0x multiplier and no change to T2's definition was satisfiable while the anchor VALUE
itself was unobserved. And **`Gate` moved substantially**, so the T1 admission population
changed - **a FALL is not a NON-MOVE**, and the branch reads on a non-move.

**No night is re-scored and no night record is edited.**

## 3. The instruments now exist

**`b706e2b`** adds DIAG `AH <value>  n <count>` and `AD <value>`. `AH` prints `--` when 0,
because `prv_compute_anchor` zeroes it when no minute qualifies, so 0 is UNDEFINED and not
measured - the convention `measurement-spec-v1` section 3.6 already established. **`AD` is
NOT `BASE`**; the two are different medians over different populations.

**`2a2f625`** fixes T3 and adds the T1-admitted veto partition, DIAG `xT2 / xT3 / xB` and
`pass`. **T3 read `s_epoch_still` alone**, a bitmap set whenever movement is false, which
includes minutes with NO accel samples - so **a sensor gap passed the atonia veto.** Section
3.1 states UNKNOWN is NEVER treated as STILL with no scope qualifier, and `main.c` 188-189
states the contract; the Awake path honoured it and T3 was the only site that did not.
**Implementation defect against `2ae829a`, corrected in code, NOT a spec change.**

**Consequence recorded: this makes T3 STRICTER and `RemN` mechanically LOWER**, for a reason
unrelated to anchor bias.

**The partition is NOT guaranteed to sum to `Gate`** - `prv_measure` re-reads `rec.stage`
after `prv_base_redecide` rewrote it, so the two Awake skips filter different sets. **Record
the difference; do not assume zero.**

## 4. The replacement readability statement

> **Section 6.1 limb 1 and section 6.3's `A_D` sign were NOT checkable under `60c378e`
> and ARE OBSERVABLE from `b706e2b` onward**, via the DIAG readout. Both are
> same-session-only, captured at stop before the app reaches IDLE, and neither is persisted.

## 5. OBSERVABLE IS NOT SCOREABLE - limb 1 needs a baseline that does not exist

**Limb 1 is COMPARATIVE: `anchor_hr` FALLS relative to series 8. No series-8 `A_H` reading
exists and none ever will.** Series 8 ran under a build with no readout; the values were
computed and discarded. **The first night after `b706e2b` produces the FIRST `A_H` value in
the project's history, and a first reading cannot fall.**

**Registered plainly, because the sequence was proposed before this was noticed: building
the instrument made `A_H` observable and did NOT make limb 1 scoreable on the first night
that reads it.**

**THE SEQUENCE:**

- **The first night after `b706e2b` ESTABLISHES THE BASELINE.** `A_H`, `n`, `A_D` and the
  four partition counters are **recorded RAW with NO BAND.** Registering a band on a
  quantity nobody has ever read would be fitting a threshold to an unseen number.
- **The night after it IS THE FIRST THAT CAN TEST MOVEMENT**, against that baseline in the
  same T3 regime.
- **Section 6.1's discriminating branch is NOT SCORED on the baseline night and becomes
  scoreable from the one after.** Work-gated on a prior reading existing, never time-gated.

**No `RemN` band is registered for the baseline night, and the reason is stated:** T3 got
stricter at `2a2f625`, which lowers `RemN` mechanically, so a fall would look like evidence
about anchor bias and would not be. **A criterion that produces a reading meaning something
other than what it appears to mean is the defect Rule 3 exists to catch.**

## 6. What the baseline night CAN discriminate - the partition, not the limbs

**The veto partition answers the question BEHIND section 6.1 without needing either limb**,
and it needs no band to do it:

- **`xT2` dominant** - T2 is the subtractor. **Consistent with the anchor-bias mechanism**,
  and the `A_H` / `n` values say over what population the median was taken.
- **`xT3` dominant** - T3 is the subtractor. **The anchor-bias mechanism is NOT the
  operative cause**, whatever `A_H` reads.
- **`xB` dominant** - both terms fail together, which points at minutes where movement state
  and HF are both absent.
- **`pass` healthy with REM still 0** - the classifier produced REM-like structure and the
  smoother's 5/7/18 floor deleted it. Read `Off` and `OnsL` per `5d7628d`.

**One night with no band is a DIRECTION, not a magnitude.** It cannot establish a
proportion, a spread or an expected value, and **no constant may be fitted to it.**

## 7. Comparability - stated because two commits moved a group key

**`b706e2b` and `2a2f625` both move `recovery_commit` off `60c378e`.** ANY commit moves that
key; a byte-identical build with a different hash still records as a group change.

**One night stands ALONE under `60c378e` in `classifier_series 9`, and the next opens
under `2a2f625` with NO comparable predecessor.** They are NOT a mutually comparable pair
on lines keying on `recovery_commit`.

**Additionally and independently: `2a2f625` changes T3, so the two are incomparable on
every REM line regardless of the pin.** Two separate reasons, both recorded.

**This was chosen explicitly.** The alternative - defer both instruments and spend another
night as blind as the last - was available and was rejected. **Same shape and same reasoning as
`9ec062d`.**

## 8. What is NOT changed

**Section 6.1's prediction and BOTH its branches stand. Section 6.2 stands. Section 6.3's
first three sentences stand** - the sign of the effect on `A_D` remains UNKNOWN and is NOT
guessed, and building a readout is not a prediction about what it will show.

**`acf88e4` stands** and its supersession of section 6.3's last sentence is untouched;
`s_epoch_hf` remains RAM-only, never persisted and never rendered. **This correction does
NOT expose per-minute HF** - diagnostic-queue item 14 is unaffected and still open.

**`HR_STALE_SEC`, `A_MIN_MINUTES`, `AW_MOVED_MIN` and the 103 are untouched. `t2` is NOT
changed** - review finding 4 is instrumented, not acted on. **`classifier-spec-v2` section
4.3's intact-atonia standing assumption is unchanged**; a user-declared RBD relaxation is
out of scope until normal staging works. **No constant moves. No decision moves.
`CLASSIFIER_SERIES` is NOT bumped and remains 9.**

## 9. Status

**Frozen on commit. Registered BEFORE the night that reads these instruments runs. `2ae829a` remains governing except for the
readability statements superseded here. A further change is a v4 or another dated
correction, never an edit.**
