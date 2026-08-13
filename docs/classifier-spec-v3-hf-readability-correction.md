# Classifier spec v3 - HF readability correction

**Dated 2026-08-13. Supersedes the last sentence of `2ae829a` section 6.3 ONLY. `2ae829a`
is NOT edited and remains governing in every other part.**

## 1. The defect

`2ae829a` section 6.3 ends, at line 318 of `classifier-spec-v3.md`:

> It is checkable from `s_epoch_hf` and has not been checked.

**That sentence asserts a readout that does not exist.** It was written without grepping
`s_epoch_hf` for persist or render sites - the same class of failure as `2401455` section
1, and the reason PERMANENT RULES Rule 1 exists.

## 2. What the source says

Grepped 2026-08-13 across `src/c/*.c` and `src/c/*.h`. **Five sites, all internal:**

- `main.c` 254 - declaration, `static uint16_t s_epoch_hf[EPOCH_VAR_MAX]`
- `main.c` 366 - filled in `prv_compute_anchor` from `prv_window_median_hr`
- `main.c` 375-376 - read by the `A_H` median
- `main.c` 431 - read by the T2 test

**No persist site. No render site. No screen exposes per-minute HF** - DIAG shows the
dispersion aggregates (`Vmax`, `P90`, `Vmed`, `Vn`, `Gate`, `Bmin`, `Bmax`), not HR.

## 3. The replacement sentence

> **Checkable in principle but NOT checkable today.** `s_epoch_hf` is a RAM-only array
> (`main.c` 254), filled in `prv_compute_anchor` (366) and read only by the `A_H` median
> (375-376) and the T2 test (431). **It is never persisted and never rendered**, so no
> screen or stored field exposes it, and it would be same-session-only in any case. HF
> derives from stored `mean_ppi` per `8793680` section 3.3, and `EpochRecord.mean_ppi` IS
> persisted, so **the check needs an INSTRUMENT that does not exist, not a new
> measurement.** Registered as a diagnostic-queue item, not as something a later session
> can simply look up.

## 4. What is NOT changed

**Section 6.1's anchor-bias prediction and BOTH its branches, section 6.2, and the first
three sentences of section 6.3 stand unchanged.** The sign of the effect on `A_D` (T1)
remains UNKNOWN and is not guessed. The direction argument for `A_H` remains a reasoned
physiological expectation, NOT established on Kristopher's own data.

**No constant moves. No decision moves. `CLASSIFIER_SERIES` is NOT bumped by this
correction.**

## 5. Consequence

**The direction of the anchor bias CANNOT be confirmed before N20 runs**, and no session
should plan on looking it up. Confirming it requires a readout built from persisted
`mean_ppi` - an instrument, sequenced behind the v3 implementation.

## 6. Status

**Frozen on commit. `2ae829a` remains governing except for the single section 6.3 sentence
superseded here. A further change is a v4 or another dated correction, never an edit.**
