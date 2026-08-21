# hrv-cadence-readout-spec-v1

**AN INSTRUMENT ONLY.** No classifier input, no stage, no total, no stored field. Adds four session-scoped counters and two render lines. **RECORDED, NOT SCORED on N27.**

## §1 — Why this instrument exists

The RSA-derived respiration work is blocked on the actual overnight HRV delivery cadence, which has never been measured.

`s_hrv_events` already exists but cannot answer the question. It is **app-lifetime**: declared `= 0` at file scope, incremented in `prv_health_handler`, rendered on IDLE's `ev` line, and reset by **neither** `prv_init` **nor** `prv_start_recording` — both function bodies were read in full at `042a98a`. A session-scoped counter is a DIFFERENT QUANTITY and must not reuse the `ev` label (RULE 9).

A raw event count is also insufficient on its own. `health_service_peek_hrv_ppi_ms()` is documented in the stock 4.33.1 SDK (emery `pebble.h`, doc comment immediately above the declaration) as returning the MOST RECENT interval, or 0 if no reading is available. It is a **LATCH, NOT A QUEUE** — an event arriving before the sensor advances returns the previous value again. **Duplicate delivery is expected API behaviour, not an anomaly**, and a cadence figure that does not account for it is nominal rather than real.

## §2 — The four counters and the registered identity

All session-scoped, all `uint32_t`.

| Label | Meaning |
|---|---|
| `He` | HRV events delivered this session |
| `Hx` | events rejected at the `ppi > 0` guard (no reading available) |
| `Hn` | events accepted |
| `Hd` | accepted samples equal to the immediately preceding accepted sample |

**REGISTERED IDENTITY: `He == Hn + Hx`.** It holds because the guard is a two-way branch on a single event with no third path — read from source in `prv_health_handler`. **A violation means a counter is misplaced**, and is a defect in the instrument, not a finding about the sensor.

## §3 — Sites

Declarations beside the existing statics. **Zeroed in `prv_start_recording`** with a `hrv-cadence-spec-v1 s3` trailing comment, matching the local convention every other session-scoped counter in that function follows.

- `He` increments at the top of the HRV branch, **OUTSIDE** the `ppi > 0` guard, beside the existing `s_hrv_events++`. It counts events, not accepted samples.
- `Hx` increments on the guard's FALSE path.
- `Hn` increments and the `Hd` comparison runs on the TRUE path, **BEFORE** the assignment to `s_last_ppi`.
- **The `Hd` comparison uses a NEW SESSION-SCOPED REFERENCE, NEVER `s_last_ppi`.** `s_last_ppi` is app-lifetime and is NOT reset in `prv_start_recording` — read from source — so using it would compare the night's first sample against a value carried in from before recording began.

## §4 — Render

**Two `GOTHIC_14` lines appended to `prv_draw_diag3` after the `C2s` / `C2n` line.** `y` advances 118 → 154 at the established 18px per line.

DIAG 3's header is `GOTHIC_24_BOLD` at 26px from `y = 2`; every value line is `GOTHIC_14` at 18px. **DIAG 2 renders NINE lines by the same font, advance and offset — `y = 190` — and was verified on the watch 2026-08-20 with nothing clipped at either end.** 154 is 36px inside a height already proven on this panel. **This is an empirical read of the hardware, not an inference from a display constant.**

`He` / `Hx` / `Hd` / `Hn` reuse no existing label on any screen.

## §5 — The undefined guard

**Keyed on `s_session_start`** (RAM-only, written in `prv_start_recording`). Zero → print `--`, **never 0**, per `measurement-spec-v1` §3.6.

`prv_deinit` calls `prv_stop_recording()` if `s_recording` is set — read from source — so an app exit ENDS the session and a relaunch cannot rejoin one. The guard therefore distinguishes "this process ran no session" from a real measurement.

**A DEFINED `He` OF 0 IS A REAL ZERO AND IS A FINDING** — events did not arrive — exactly as the `Sr5` comment already records for a defined `SrN` with `Sr5 0`.

## §6 — What this instrument cannot say, and how `Hd` is to be read

**`Hd` BOUNDS DUPLICATION FROM ABOVE. IT DOES NOT MEASURE IT.**

1. **`Hd` sees ACCEPTED SAMPLES ONLY.** Events rejected at the guard never reach the comparison, so the duplicate rate is over accepted samples and **NOT over events**.
2. **Two genuinely identical consecutive intervals are indistinguishable from a re-delivery.** At millisecond resolution that is uncommon but not rare.

**THE COINCIDENCE NULL IS DERIVED AND FILED PRIVATELY.** Some fraction of `Hd` is coincidence rather than re-latching. The derivation, its inputs and its numeric result are in `~/recovery-nights/docs/hrv-duplicate-coincidence-null-2026-08-20.md` — **filed there and not here because it contains subject physiological values (RULE 7).** **READ THAT FILE BEFORE INTERPRETING `Hd`.**

**Four weaknesses of that derivation, stated here without the values:**

- The source RMSSD column is **last-400-beat scope, not night-wide** — the open defect in next-action 13. The null is order-of-magnitude, not a precise threshold.
- The form assumes successive differences are roughly normal and centred on zero. Real PPI series are heavier-tailed with more near-zero mass, so the true coincidence rate is **HIGHER** than derived. **The null is a FLOOR** — the conservative direction for detecting excess.
- It assumes millisecond integers. The API returns `uint16_t` ms, confirmed from the declaration. **If the sensor’s true resolution is coarser than 1 ms, quantisation raises coincidence sharply and this null collapses — a high `Hd` must trigger a resolution check BEFORE it is read as re-latching.**
- One subject. Consistency, not correctness.

**INTERPRETIVE GUIDANCE, EXPLICITLY NOT A CRITERION:** below the derived null is consistent with coincidence and rules duplication out; above ~5% exceeds any coincidence explanation and indicates re-latching; between, inconclusive.

**NO BAND IS PLACED ON ANY OF THESE FOUR VALUES FOR N27.** They have not read once. This follows `awake-runs-readout-spec-v1` §9 and the same clause in `stillness-run-readout-spec-v1`. **The guidance above is for scoring-time reading only and scores nothing.** A banded criterion may be registered for N28 after the values have read.

## §6.1 — THE PRIMARY TEST: EFFECTIVE UNIQUE SAMPLE RATE AGAINST NYQUIST

**The RSA constraint is Nyquist on the beat-sampling rate, NOT a beats-per-breath count.** The RR series is sampled by the heartbeat itself, so its effective sampling frequency is the heart rate and the highest recoverable respiratory frequency is **HR / 2**. This was established by literature search 2026-08-20 — including a study that deliberately paced breathing above half the heart rate in order to observe aliasing develop, and a standard reference noting that resampling-based methods severely attenuate components above ~0.4 Hz while the Lomb method escapes the limit only because a non-uniform series has no defined Nyquist frequency.

**AN EARLIER FRAMING IN THIS PROJECT — "RSA needs about four beats per breath" — WAS AN ARITHMETIC PLAUSIBILITY ESTIMATE, NOT A FINDING, AND IS SUPERSEDED BY THIS SECTION.**

At sleeping heart rate, Nyquist sits near **0.45–0.55 Hz (27–33 breaths/min)** against sleep respiration of **12–20/min (0.2–0.33 Hz)** — a margin of roughly **1.5–2×**, not an order of magnitude.

**THIS IS WHY `Hd` MATTERS. A LATCHED DUPLICATE IS DECIMATION.** The UNIQUE sample rate sets Nyquist, not the event rate. A 50% duplicate rate halves the effective frequency and drops Nyquist to ~0.25 Hz, inside the respiratory band — where the signal does not degrade gracefully, it **ALIASES INTO A PLAUSIBLE-LOOKING WRONG ANSWER.**

**THE DERIVED QUANTITY, computed at scoring time and NEVER typed (RULE 6):**

```
effective unique sample rate = (Hn - Hd) / session seconds
```

compared against **2× the respiratory band**. Both inputs are on the screen; deriving the quotient at scoring time is not a RULE 6 violation, the same reasoning the anchor readout gives for not rendering its fraction. Session duration comes from `rec_duration_min`.

**WHY AN UPPER BOUND IS SUFFICIENT HERE.** The decision is a THRESHOLD COMPARISON, not an estimate. An upper bound on duplication gives a LOWER bound on unique sample rate, and a lower bound is the correct form of evidence for a "does it clear the threshold" question. **`Hd` does not need to be a measurement for this decision to be sound.**

**Secondary, and unambiguous in every direction:** `He` against expected beat count (`mean_bpm` from `derived.csv` × session duration). `He` far below expected → the stream is decimated at delivery and no duplicate analysis rescues it. `He` ABOVE expected → re-delivery is proven outright without `Hd`.

## §7 — Comparability

**INSTRUMENT ONLY. NO OUTPUT CHANGE.** `CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION` are to be **VERIFIED UNCHANGED AFTER IMPLEMENTATION**, not asserted here. N27 remains comparable to N26 on every stage line.

## §8 — Registered for the respiration spec, not for this one

Standard RSA pipelines **interpolate RR values onto a regularly sampled time vector** before spectral analysis. **Recovery has no resampling step.** Any on-device RSA work inherits that requirement. Named here so it is not discovered late; it is scope for the respiration spec.
