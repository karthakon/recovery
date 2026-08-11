# NIGHTS render spec v1 - NightSummary v3 and the v_* display

**Dated 2026-08-11. Frozen BEFORE the night that tests it (N18).**

## 1. Why this exists

All seven DIAG values are already persisted in `NightSummary` v2 (`storage.h` 37-43), but
`prv_draw_history` stops at the battery line - source-verified: the render path covers
date/start/end, awake+rem, light+OSrest, rmssd+BASE, batt, then the navigation hint.

DIAG's on-screen values are RAM statics lost when the app process ends, so the only route
to a past night's diagnostics is re-entering DIAG before exit. Rendering the persisted
fields on NIGHTS retires that problem at source **for summary fields only**.

**It does NOT retire the critical ordering constraint for RUNS.** RUNS reads
`EpochRecord`, not `NightSummary`, and only one night of epochs exists at a time. RUNS
must still be read at stop, the same session.

## 2. The version byte

`NIGHT_SUMMARY_VERSION` moves **2 -> 3**. The v3 record appends **one `uint8_t
classifier_series`** at the tail. **Append, never insert.**

- `NIGHT_SUMMARY_V3_TAIL_BYTES 1`.
- `storage_night_read` accepts **three** sizes: v1, v2, v3. The existing both-sizes path
  (`cc305dd`) is **extended, never replaced or undone**.
- On a v<3 record `classifier_series` is **absent, not zero**.
- The value written is a compile-time constant bumped by hand at each classifier change,
  in the same commit as the spec that changes it.

## 3. The two non-negotiable display constraints

**3.1 Absent fields render `--`, NEVER `0`.** A rendered 0 is indistinguishable from
N16's genuine `Gate 15` or N17's genuine `Gate 77`. The seven `v_*` fields are absent on
v<2 records; `classifier_series` is absent on v<3 records. Absent prints `--`.

**3.2 The `v_*` fields CHANGED MEANING at `classifier_series 7`** (`443f499`). N16's
stored values describe raw per-minute variance against 2 x BASE. N17's describe `D(m)` and
`F(m)` against 2 x A. N18+ describe the same statistics under the v2 conjunction. **The
rendered `classifier_series` is what makes a displayed number interpretable.** Without it
the screen shows numbers whose meaning cannot be recovered from the screen.

## 4. Render

Three `GOTHIC_14` lines at 18px, appended after the battery line and before the
navigation hint. Vertical budget confirmed by source read: the existing path ends at
y=144 and the hint occupies 18px, leaving room on a 228px emery display.

```
Vmax 13225  P90 2401
Vmed 529  Vn 521  Gate 77
Bmin 342  Bmax 12415  cs7
```

On a v2 record the series reads `cs--`. On a v1 record all three lines read `--`.

## 5. Scope

**In scope:** `NIGHT_SUMMARY_VERSION`; the one appended byte; the read path's third
accepted size; the three added lines in `prv_draw_history`.

**NOT in scope:** DIAG itself; RUNS; any `v_*` computation; `EpochRecord`; the
classifier; anything in `ed12fd7`; the font pass (next-action 17); night labeling.

## 6. Comparability

`classifier_series` is **8** for N18. **N1-N17's stored records are NOT rewritten** and
will render `cs--`. That is correct: their series is recorded in the night files and in
`nights.csv`, which remain the source of record.

**This spec does not move `classifier_series` by itself.** `ed12fd7` moves it 7 -> 8.
This spec only stores and displays it.

## 7. Aggregation contract - binding on all future rolling statistics

Kristopher's stated intent is a rolling window of up to 21 stored nights, computed **on
the watch from `NightSummary` alone**, to produce averages and medians and to compare a
current night against a trend. The night files are not available to that code.

**Any rolling statistic over the `v_*` fields MUST group by `classifier_series` and MUST
NOT aggregate across a boundary.** A 21-night window at the current cadence will routinely
straddle a classifier change. Averaging raw variance against 2 x BASE together with `D(m)`
against 2 x A produces a number about nothing.

**Records reading `cs--` (v<3) are outside every group and are excluded from `v_*`
aggregation.** They remain valid for BASE, Awake, battery, RMSSD, rejection and
`mean_hr` - none of which the classifier change touches. This mirrors exactly what
`nights/C00-N17.md` records for the CSV pipeline.

**Next-action 21's Phase 2 HR baseline is UNAFFECTED.** A rolling mean of stored
`mean_hr` is not classifier-derived and is not subject to this contract. The two rolling
windows are not to be conflated.

## 8. Status

**Frozen on commit. Not retunable. A change is a v2 or a dated correction, never an
edit.**
