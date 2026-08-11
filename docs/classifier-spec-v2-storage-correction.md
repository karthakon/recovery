# Classifier spec v2 - storage correction

**Dated 2026-08-11. Supersedes section 3.5 and one line of section 5.5 of `8793680`.
`8793680` is NOT edited.**

## 1. The defect

`8793680` section 3.5 states that `EpochRecord.reserved` "is read by nothing."

**This is false.** Source read establishes:

- `smoother.c` line 215 writes `rec.reserved = rec.stage`, preserving the **pre-smoother
  raw classifier stage**.
- `main.c` line 801 reads it as `st_pre` and compares it against `StageAwake` and
  `StageREM` for the onset and REM-run detection of `epoch-readout-spec-v1`.
- `smoother.h` line 7 documents the field's purpose.

The field is fully allocated and load-bearing. Writing `S(m)` into it would corrupt the
RUNS screen - the instrument that produced N17's `L5p 7` and the instrument N18 must be
scored with.

The claim was written from the single write site at `main.c` line 104 (`rec.reserved = 0`)
without grepping for readers. **Recorded as a process failure: an assertion about the
source was made without reading the source.**

## 2. A second, independent defect

`smoother_run` executes **after** `prv_base_redecide` (`main.c` `prv_stop_recording`).
A stillness bit written at minute close would therefore be overwritten by the smoother
before RUNS ever read it. **The section 3.5 design failed on ordering as well as on
allocation.** Either defect alone is disqualifying.

## 3. The replacement for section 3.5

**3.5 Stillness.** `S(m)` = true when minute `m` recorded no movement. **RAM-only, never
persisted**, following the pattern `base-spec-v1` section 4 established for
`s_epoch_var`.

> Held in a static bitmap `s_epoch_still`, `uint8_t[(EPOCH_VAR_MAX + 7) / 8]` - 120 bytes.
> Bit `m` is set at minute close when the minute recorded no movement, written from the
> existing `movement` boolean at `main.c` 105-107, index-parallel to `s_epoch_var`, and
> cleared alongside `s_epoch_var_count` at session start. Read only by the stop-time
> re-decision pass, in the same process that wrote it.

**`EpochRecord.reserved` is NOT touched by this spec in any way.**

## 4. The replacement for section 5.5

The section 5.5 entry is replaced by:

> `EpochRecord` layout, size, version **and field semantics are entirely unchanged.**
> `reserved` retains its existing meaning as the pre-smoother raw stage.

Section 5.6 (`NightSummary`, storage layout, persist keys unchanged) **stands as written.**

## 5. Consequences

- BSS for the stillness term falls from a persisted-field approach to **120 bytes**.
- **No storage format implication.** Nothing about this spec now touches persistence.
- `S(m)` does **not** survive app exit. **Acceptable:** it is consumed at stop, before
  exit, and is never displayed.
- Section 5.4's `s_epoch_hf` (1920 bytes) is unaffected and stands.

## 6. What is NOT changed

Section 4's three-term conjunction, `A_D`, `A_H`, `HF(m)`, section 4.1's term roles,
section 4.2, **section 4.3's intact-atonia assumption and its README obligation**,
section 6's registered failure modes, section 7's DIAG consequence and section 8's
comparability all stand unchanged. **Only where `S(m)` lives has changed, not what it is
or how it is used.**

## 7. Status

**Frozen on commit. `8793680` remains governing except for section 3.5 and the single
section 5.5 line superseded here. A further change is a v3 or another dated correction,
never an edit.**
