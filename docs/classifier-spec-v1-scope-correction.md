# Classifier spec v1 - scope correction

**Dated 2026-08-10. Supersedes ONE LINE of `c5edaee` section 2. `c5edaee` is NOT edited.**

## 1. The defect

`c5edaee` section 2 lists under **NOT in scope**:

> `876a6d6`'s BASE definition, `BASE_SAMPLE_MAX 160`, and the existing re-decision pass.

**The re-decision pass must not be in that list.** The line was written believing
`prv_base_redecide` was BASE-internal bookkeeping. Source read of `main.c` 280-305
establishes otherwise: the function **is** the REM decision. Lines 292-297 are the stage
rule - `v * 2 >= base_final && v <= base_final * 2` gives Light, `v > base_final * 2`
gives REM, else Light - which is `c5edaee` section 3.4's rule, anchored on `base_final`
rather than on `A`.

A spec that changes the REM decision cannot exclude the function that makes it. **The
exclusion was self-contradictory as written.**

## 2. The replacement

The `NOT in scope` entry is replaced by:

> `876a6d6`'s BASE definition, `BASE_SAMPLE_MAX 160`, `prv_base_median()`, the
> `s_base_samples` collection path, and the BASE value itself as computed, displayed and
> recorded.

**`prv_base_redecide` is IN scope.** What changes in it is the comparand only: it takes
`A` (section 3.3) in place of `base_final`, and tests `F(m)` (section 3.2) in place of the
raw `s_epoch_var[i]`.

**What does NOT change in it:** the Awake skip, the `beat_count < 20` skip, the zero skip,
the `s_mins` decrement/increment bookkeeping, the `storage_epoch_update` write, the loop
clamp, and its position before `prv_measure` and `smoother_run`. **The function is not
rewritten. One comparand moves.**

## 3. Why this does not touch `876a6d6`

`876a6d6` section 3.4 governs the re-decision pass **as a mechanism**: a stop-time pass
over stored epochs that overwrites Light/REM, leaves Awake alone, and precedes the
smoother. **That mechanism is unchanged and this correction relies on it.** What
`876a6d6` does not govern is which statistic the pass compares - that is a classifier
question, resolved by D1 (`c5edaee` section 5): the REM anchor moves from BASE to `A`,
and BASE is otherwise untouched.

**No BASE parameter moves. `BASE_SAMPLE_MAX 160` unchanged. `prv_base_median()`
unchanged. The 2.0x multiplier unchanged.**

**Naming:** the function keeps the name `prv_base_redecide` despite no longer anchoring
on BASE. Renaming it would produce a diff that obscures the one-line change under a
rename, and `876a6d6` section 3.4 refers to it by name. **Recorded so the name is
understood as historical, not descriptive.**

## 4. Implementation consequence - no second pass

`c5edaee` section 4.1 describes the REM/Light decision as "a pass over stored per-minute
`D`", which read alone could be built as a **new** pass alongside `prv_base_redecide`.
**It must not be.** Two functions overwriting the same stored stages is the ordering
hazard `0670f72` section 3.4 exists to prevent. There is **one** re-decision pass, and it
is the existing one with a moved comparand.

`F(m)` requires `s_epoch_var` **un-sorted and epoch-indexed**. `prv_measure` compacts and
sorts it in place. `0670f72` section 3.3 already forces `prv_measure` to run after
`prv_base_redecide`, so this is satisfied - but **`F` must be computed inside or before
the re-decision pass, never after `prv_measure`.**

## 5. `RemN <= Gate` - resolved, equality NOT registered

`443f499` section 2 required both call sites to be read before registering
`RemN == Gate`. **Read, and the equality does not hold.** `Gate` counts entries of the
compacted, sorted array in `prv_measure`; `RemN` counts stored epochs surviving the
Awake, `beat_count < 20` and zero skips in `prv_base_redecide`. **Different populations,
as before the change.**

**`RemN <= Gate` STANDS as the registered relation for N17.** It remains an internal
consistency check on the instrument, never an acceptance criterion for a night's results.
`RemN > Gate` remains a genuine failure requiring diagnosis.

## 6. Status

**Frozen on commit. `c5edaee` remains governing except for the single section 2 line
superseded here. A further change is a v2 or another dated correction, never an edit.**
