# hrv-resolution-readout-spec-v1

**INSTRUMENT ONLY. NO CLASSIFIER INPUT, NO STAGE DECISION, NO TOTAL, NO STORED FIELD, NO
PERSISTED STRUCTURE.** `CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION` do NOT change and MUST
be VERIFIED UNCHANGED by reading `src/c/storage.h` after implementation, not asserted.

**THIS SPEC DOES NOT SPECIFY RSA AND IT DOES NOT SPECIFY RRV.** It specifies the measurement
that establishes the true resolution of the inter-beat interval series, which is an INPUT
ASSUMPTION of any spectral work and is currently UNVERIFIED.

**WHY IT EXISTS AND WHAT IT BLOCKS, recorded because it is the reason this file precedes the
RRV spec:** `hrv-cadence-readout-spec-v1` §6 registers as its THIRD WEAKNESS that the
coincidence null assumes millisecond integers, and that **if the sensor's true resolution is
coarser than 1 ms, quantisation raises coincidence sharply and the null collapses.** **N28's
`Hd/Hn` is THE FIRST READING ABOVE THAT SPEC'S OWN RE-LATCHING LINE, which triggers the
weakness for the first time.** The measured value is in the N28 night file in
`~/recovery-nights` and is NOT reproduced here (RULE 7). **The full-pipeline RRV spec is BLOCKED ON THIS ONE READING**, by
decision recorded 2026-08-22, because a coarse result changes the interval series the pipeline
would analyse and would require the spec to be rewritten rather than amended.

## §1 — What the source read established, and where it stopped

**Read 2026-08-22. All line numbers are STALE BY CONSTRUCTION. RE-GREP BEFORE RELYING ON ONE.**

**THE FIRMWARE IS A PASS-THROUGH AND CANNOT BE THE SOURCE OF QUANTISATION.** Read at tag
`v4.33.2` in `~/PebbleOS`, which is the firmware the watch runs:

1. `gh3x2x_hrv_result_report()` in `src/fw/drivers/hrm/gh3x2x.c` receives `rri` as
   `const int32_t *` — **already-formed intervals delivered by the Goodix library.**
2. Everything the firmware does to them is a plausibility bound (`rri[i] <= 0` or
   `rri[i] > UINT16_MAX`), a `(uint16_t)` cast, and a confidence-to-quality mapping.
   **No conversion factor, no sample-count arithmetic, no rounding, no scaling.**
3. **THE HRM DRIVER IS BYTE-IDENTICAL BETWEEN `v4.33.1` AND `v4.33.2`** — verified by an empty
   `git diff --stat` over `src/fw/drivers/hrm/`. The SDK version and the firmware version are
   DIFFERENT THINGS and PebbleOS does not ship an SDK with every firmware release.
4. **THEREFORE THE RESOLUTION IS DETERMINED INSIDE THE NONFREE GOODIX ALGORITHM AND IS NOT
   ESTABLISHABLE FROM SOURCE.** That is a FINDING, not a gap in the reading, and it holds on
   current stock as much as on any branch.

**`GH3X2X_HRV_SAMPLING_RATE` IS 100 AND IS THE PPG SAMPLING RATE FED TO THE HRV FUNCTION. IT
IS NOT THE INTERVAL QUANTISATION AND MUST NOT BE READ AS IMPLYING 10 ms RESOLUTION.** A 100 Hz
raw rate is CONSISTENT WITH 10 ms granularity if the algorithm reports peak positions in whole
samples, **and is equally consistent with sub-sample interpolation, which is standard
practice.** Nothing in open source says which. **A later session must not resolve this by
arithmetic on the sampling rate.**

**THE SDK DECLARATION GIVES THE CONTAINER AND THE UNIT, NOT THE RESOLUTION.**
`uint16_t health_service_peek_hrv_ppi_ms(void)` at
`~/.local/share/pebble-sdk/SDKs/4.33.1/sdk-core/pebble/emery/include/pebble.h` documents PPI in
ms and 0 when unavailable. **A value delivered as 750 is equally consistent with 1 ms
resolution and with a sensor quantising to 5 or 10 ms and reporting in ms.** The `uint16_t`
type is NOT evidence of 1 ms resolution and the existing coincidence null's citation of it
establishes the unit only.

**SDK PATH, RECORDED SO IT IS NEVER RE-DERIVED:** the SDK is at
`~/.local/share/pebble-sdk/SDKs/` and NOT at `~/.pebble-sdk`. App headers are in the single
per-platform `pebble.h` above. **There is no standalone `health_service.h`.**

## §2 — The quantity this instrument measures

**THE GRANULARITY ON WHICH ACCEPTED INTER-BEAT INTERVALS FALL.**

It measures nothing about respiration, nothing about the classifier, and nothing about whether
the discarded beats were artefact or signal. **It answers one question: are the integers the
sensor delivers spaced at 1 ms, or at some coarser step.**

## §2.1 — What the FIRST NIGHT under this instrument DOES and DOES NOT DECIDE

**REGISTERED BECAUSE THE COMPRESSED FORM OF THIS QUESTION IS WRONG IN A SPECIFIC WAY AND A
LATER SESSION WILL INHERIT IT OTHERWISE.**

**THE WRONG FORM, NAMED SO IT IS RECOGNISED:** *"the first night tells us whether the interval
series is good enough for RRV to be useful for staging, and if it is not we skip RRV."*
**THAT IS WRONG ON THREE COUNTS.**

**FIRST — THERE IS NO SWEET SPOT. FINER IS ALWAYS BETTER OR NEUTRAL.** Coarse is the failure
direction and fine is the success direction. **No reading is "too fine to be useful."**

**SECOND — A COARSE READING VERY PROBABLY DOES NOT KILL THE DIRECTION.** Quantisation noise on
a step `q` has SD `q/sqrt(12)`, so a 10 ms step contributes under 3 ms **against RSA modulation
of inter-beat intervals that is an order of magnitude larger in a healthy adult at rest.**
**A STEP OF 10 ms IS A GO, NOT A STOP** — it is recorded as a known noise floor in the RRV spec
and the work proceeds. **THE STOPPING REGIME IS TENS OF MILLISECONDS** — 20 ms or coarser —
**and `Dm` exists precisely because the divisor set alone cannot distinguish 10 from 20 or 50.**

**THIRD AND MOST IMPORTANT — THIS INSTRUMENT IS NOT THE GATE ON WHETHER RRV HELPS CLASSIFY
SLEEP, AND IT IS NOT CLOSE.** It settles ONE input assumption. **RRV COULD READ A PERFECT 1 ms
AND STILL BE USELESS FOR STAGING.** Everything that actually decides that is untouched here:
whether the respiratory modulation is strong enough to extract from wrist PPG at all; whether
the spectral computation fits on the watch; **whether RRV DISCRIMINATES SLEEP STAGES, which
nothing in this project has yet tested;** and the standing ceiling that the cited RSA literature
used dedicated airflow or respiratory-effort channels while wrist-derived respiration is
reported around 0.5 to 1.5 breaths per minute error with WATCH FIT as the largest error source.

**THE TWO GATES ARE DIFFERENT AND COLLAPSING THEM IS THE DEFECT THIS SECTION EXISTS TO
PREVENT:** a pipeline that COMPUTES CLEANLY is not a pipeline that HELPS. **Drafting the RRV
pipeline spec is gated on this instrument and on the on-watch computation question. RRV
ENTERING `classifier-spec-v6` IS GATED ON A DEMONSTRATION THAT IT DISCRIMINATES STAGES**, which
cannot be performed until the pipeline exists and produces numbers. **NO READING FROM THIS
INSTRUMENT MAY BE CITED AS EVIDENCE TOWARD THAT SECOND GATE.**

## §3 — Which buffer, and why the choice is not free

**READ FROM SOURCE — `main.c` calls `hrv_buf_add` on THREE buffers with the same interval, and
each maintains its OWN gates and references.** The reasoning is identical to
`rsa-feasibility-readout-spec-v1` §3 and is not restated: `s_live_buf` is UNGATED and includes
intervals belonging to no session; `s_minute_buf` resets every minute; **`s_night_buf` is gated
on `s_recording` and accumulates across the session, and IS THE ONE.**

**ALL COUNTERS IN §4 ARE DEFINED OVER `s_night_buf` AND OVER NOTHING ELSE.** A counter that
does not name its buffer is meaningless. A later session changing the buffer changes the
quantity and must supersede this section by a dated file.

## §4 — The counters

All session-scoped, all `uint32_t`, all gated on `s_recording`, all reset in
`prv_start_recording`.

| Label | Meaning |
|---|---|
| `D2` | accepted intervals divisible by 2 |
| `D4` | accepted intervals divisible by 4 |
| `D5` | accepted intervals divisible by 5 |
| `D8` | accepted intervals divisible by 8 |
| `D10` | accepted intervals divisible by 10 |
| `Dm` | the SMALLEST NON-ZERO absolute difference between CONSECUTIVE accepted intervals, in ms |
| `Dn` | the denominator — accepted intervals counted |

**`Dm` IS THE COUNTER THAT DOES NOT ENUMERATE.** The five divisor counters can only detect the
steps they name. **`Dm` bounds the step from above WITHOUT naming it** — if the true resolution
is `q` ms, every interval is a multiple of `q`, so every difference between two intervals is
also a multiple of `q`, and **the smallest non-zero difference observed over a night of tens of
thousands of intervals IS `q`** provided the series ever produces two intervals one step apart.
**It therefore detects 3 ms, 7 ms, 20 ms, 50 ms or any other step the divisor set cannot.**

**`Dm` IS COMPUTED OVER CONSECUTIVE ACCEPTED PAIRS AND ZERO DIFFERENCES ARE EXCLUDED.** A zero
difference is two identical consecutive intervals, which is the quantity `Hd` already counts
and is NOT a resolution signal. **`Dm` is seeded to a sentinel above any possible interval and
prints `--` if it is never lowered.**

**`Dm` AND THE DIVISOR COUNTERS CHECK EACH OTHER AND NEITHER IS SUFFICIENT ALONE.** A SINGLE
anomalous pair could drag `Dm` to a spuriously low value, so **`Dm` alone is a bound and not a
measurement.** The divisor ratios describe the BULK of the distribution and cannot be moved by
one pair. **`Dm` bounds the step; the divisors confirm it. A later session must not read either
one on its own.**

**THEY ARE ACCUMULATED PER BEAT AS INTERVALS ARE ACCEPTED, NOT COMPUTED AT RENDER TIME.**
**READ FROM SOURCE AND THIS IS THE REASON:** `hrv_math.c` appends while `count < HRV_BUF_MAX`
and otherwise `memmove`s down one and writes the newest at the end, so **`ppi[]` holds only the
last 400 intervals.** A histogram computed at stop would describe the last 400 beats and NOT
the night — **the same last-400-beat scope error that next-action 13 records against
`ns.rmssd`, `ns.sdnn` and `ns.mean_ppi`, and it must not be reintroduced in a new instrument.**

**`Dn` IS NOT `count` AND IS NOT `total_accepted`.** `count` pins at `HRV_BUF_MAX`.
`total_accepted` is a running session total that does not pin (`hrv_math.c`, incremented on
every accepted interval) and **is the correct scope** — but it is maintained by the buffer for
its own purposes, and this spec does NOT read it. **`Dn` is incremented by this instrument at
the same site as the five divisor counters, so all six share one increment path and one
scope.** **`Dm` IS UPDATED AT THAT SAME SITE BUT IS NOT A COUNT — it is a running minimum and
is EXCLUDED FROM EVERY IDENTITY IN THIS SECTION.** A separate `Dn` is what makes the identity in this section checkable from the rendered
values alone without trusting a field this spec did not maintain.

**REGISTERED IDENTITIES — NESTED SUBSET RELATIONS, NOT BANDS:**

- **`D10 <= D5 <= D2 <= Dn`** — every multiple of 10 is a multiple of 5 and of 2.
- **`D8 <= D4 <= D2 <= Dn`** — every multiple of 8 is a multiple of 4 and of 2.
- **`D10 <= D2`** — follows from the first chain and is stated so it is checked directly.

**A VIOLATION MEANS A COUNTER IS MISPLACED AND IS A DEFECT IN THE INSTRUMENT, NOT A FINDING
ABOUT THE SENSOR.** Identical in status to P-GIDENT.

**NOTE ON `D4` AND `D8` AGAINST `D5` AND `D10`:** no ordering relation holds between the
binary chain and the decimal chain, and none is registered. **Only the two chains above are
identities.**

**`Dm` PARTICIPATES IN NO IDENTITY.** It is a minimum over differences, not a subset count, and
no nesting relation connects it to the divisor counters. **A CONSISTENCY EXPECTATION IS
REGISTERED INSTEAD AND IT IS EXPLICITLY NOT AN IDENTITY: if a divisor `d` reads at or near
1.00, `Dm` SHOULD be a multiple of `d`.** A disagreement between the two is a FINDING requiring
diagnosis, **not an instrument defect and not a void reading** — it is the mixture signature
seen from a second direction.

**NO OTHER COUNTER IS ADDED.** In particular nothing is added to `s_live_buf` or
`s_minute_buf`, and `rej`, `rng` and `jmp` on RESULTS are NOT touched, NOT relabelled and NOT
reinterpreted.

## §5 — Render, and the screen split

**DIAG 3 IS FULL AT NINE LINES AND NINE IS THE CEILING.** The nine-line fit was verified on the
watch 2026-08-22; **ten and eleven have NOT been verified and MUST NOT be assumed.**

**A NEW SCREEN, DIAG 4, IS ADDED. FOUR LINES, WELL INSIDE THE NINE-LINE CEILING:**

```
D2 <n>   D4 <n>
D5 <n>   D8 <n>
D10 <n>  Dn <n>
Dm <n>
```

**HOW IT ATTACHES, READ FROM SOURCE 2026-08-22:** `ScreenMode` in `main.c` runs
`MODE_IDLE, MODE_RECORDING, MODE_RESULTS, MODE_HYPNO, MODE_HISTORY, MODE_DIAG, MODE_DIAG2,
MODE_DIAG3, MODE_RUNS`. **`MODE_DIAG3` currently subscribes UP and BACK and has NO DOWN
handler — it is the terminal screen of the diagnostic chain.** DIAG 4 therefore attaches
EXACTLY as DIAG 3 attached to DIAG 2: **a `MODE_DIAG4` enum member, a DOWN handler on
`MODE_DIAG3`, and a new `case MODE_DIAG4` subscribing UP and BACK back to `MODE_DIAG3`.** **NO
EXISTING NAVIGATION IS RESTRUCTURED and no other screen's handlers change.**

**THE UNDEFINED GUARD KEYS ON `s_session_start`**, identical to `rsa-feasibility-readout-spec-v1`
§5 and `hrv-cadence-readout-spec-v1` §5. Zero prints `--` and NEVER `0`, per
`measurement-spec-v1` §3.6. **A DEFINED `D8` OF 0 IS A REAL ZERO AND IS A FINDING.** **`Dm` PRINTS `--` WHEN IT WAS NEVER
LOWERED FROM ITS SENTINEL**, which on a real session means no two consecutive accepted
intervals ever differed — itself a finding, and NOT the same thing as an undefined session.
**`Dm` CAN NEVER LEGITIMATELY PRINT 0** because zero differences are excluded, so a rendered
`Dm 0` IS AN INSTRUMENT DEFECT.

**NO FRACTION AND NO PERCENTAGE IS RENDERED.** All seven values are on one screen, so deriving
every ratio at scoring time is not a RULE 6 violation — the same reasoning the anchor,
stillness, cadence and RSA readouts give.

**RULE 11 APPLIES: the new format strings are verified by running `strings` on
`pebble-app.elf` and finding all seven labels in both their defined and undefined variants. A
clean build does not prove a string reached the binary.**

## §6 — The reading, registered BEFORE the first night

**NO BAND IS PLACED ON ANY COUNTER.** What follows is ARITHMETIC FROM THE DIVISORS, not a
constant fitted to a measurement, and it takes RULE 2's narrow exemption because **no decision
reads any of these numbers.** The moment any decision reads one, the exemption stops applying.

**THE EXPECTED RATIOS UNDER EACH HYPOTHESIS:**

| Hypothesis | `D2/Dn` | `D4/Dn` | `D5/Dn` | `D8/Dn` | `D10/Dn` |
|---|---|---|---|---|---|
| True 1 ms | ~0.50 | ~0.25 | ~0.20 | ~0.125 | ~0.10 |
| 2 ms step | 1.00 | ~0.50 | ~0.20 | ~0.25 | ~0.10 |
| 4 ms step | 1.00 | 1.00 | ~0.20 | ~0.50 | ~0.10 |
| 5 ms step | ~0.50 | ~0.25 | 1.00 | ~0.125 | ~0.50 |
| 8 ms step | 1.00 | 1.00 | ~0.20 | 1.00 | ~0.10 |
| 10 ms step | 1.00 | ~0.50 | 1.00 | ~0.25 | 1.00 |

**UNDER EVERY HYPOTHESIS ABOVE, `Dm` EQUALS THE STEP: 1 for true 1 ms, 2 for a 2 ms step, 4, 5,
8, 10 respectively — and `Dm` EQUALS A VALUE IN NO ROW OF THIS TABLE if the step is one the
divisor set does not test.** That is the case the divisors alone cannot reach.

**THE DECISIVE SIGNATURE IS A RATIO AT OR NEAR 1.00, NOT A RATIO NEAR ITS NULL.** A counter
sitting at its null value is the ABSENCE of quantisation at that step. **A counter at 1.00 is
quantisation at that step, and the LARGEST divisor reading 1.00 is the step size.**

**THE `HRV_STALE_SEC` DISTINCTION APPLIES HERE TOO: `HRV_STALE_SEC` IS 10 AND IS A DIFFERENT
CONSTANT FROM `HR_STALE_SEC` 180.** RULE 9. This spec always writes them in full. **The
coincidence in this file between `HRV_STALE_SEC` 10 and the `D10` divisor is EXACTLY THAT — a
coincidence of the numeral 10 — and the two have NOTHING to do with each other.**

## §7 — What this instrument CANNOT say

**WHETHER RSA OR RRV WOULD WORK.** Resolution is one input assumption among several. The
modulation must still be strong enough to detect, the PPG noise floor must permit it, and the
computation must fit on the watch. **NONE of those is measured here.**

**WHETHER A COARSE RESULT MEANS THE SENSOR IS BAD.** Quantisation is a design choice in the
Goodix algorithm and this project has no standing to call it a defect.

**WHETHER `Hd` IS RE-LATCHING.** A coarse resolution would EXPLAIN part of a high `Hd` by
raising the coincidence rate. **It would not prove that re-latching is absent**, and a fine
resolution would not prove it is present. **The coincidence null's OTHER THREE weaknesses are
untouched by this instrument** — last-400-beat source scope, the normality assumption, and one
subject.

**WHAT THE TRUE UNDERLYING INTERVALS WERE.** Quantisation destroys information and no readout
recovers it.

**ANYTHING FROM THE THREE COUNTERS THAT READ NEAR THEIR NULL ON ONE NIGHT.** The 1.00 signature
is decisive on one night because it is arithmetic; **a reading NEAR a null is consistent with
several hypotheses at once and is not.**

## §8 — Exit conditions, registered in advance

**WHAT WOULD ESTABLISH FINE RESOLUTION:** every ratio at or near its 1 ms null AND `Dm` 1, on
ONE night.

**WHAT WOULD ESTABLISH COARSE RESOLUTION:** a divisor at or near 1.00, with `Dm` a multiple of
it. **Also answerable on one night.**

**WHAT WOULD ESTABLISH A STEP THE DIVISOR SET DOES NOT TEST:** `Dm` reading a value that is not
1 and not one of 2, 4, 5, 8, 10 — **3, 7, 20, 50 or anything else** — with the divisor ratios
consistent with it. **THE DIVISOR SET ALONE COULD NOT REACH THIS AND `Dm` IS WHY IT IS IN THE
SPEC.**

**WHY THE ENUMERATION GAP IS CLOSED IN BOTH DIRECTIONS, and this CORRECTS AN EARLIER DRAFT OF
THIS SECTION:** an earlier form registered only the FINER-than-tested gap (3 ms, 7 ms) as the
limitation. **That was the harmless direction and it was the wrong one to worry about.**
Quantisation noise on a step `q` has SD `q/sqrt(12)`, so a 3 ms step contributes under 1 ms and
a 7 ms step around 2 ms, **both far below the RSA modulation amplitude in a healthy adult at
rest.** **THE HARMFUL DIRECTION IS COARSER, NOT FINER** — and `D10` at 1.00 says the step is a
MULTIPLE of 10 without distinguishing 10 from 20 or 50, which is precisely the regime where
quantisation noise approaches the modulation and the direction fails. **`Dm` distinguishes
them. A later session must not reinstate the finer-step caveat as though it were the live
risk.**

**WHAT WOULD BE INCONCLUSIVE:** ratios elevated above their nulls but well below 1.00, with
`Dm` disagreeing with the ratios. **That pattern is what a MIXTURE would produce** — some
intervals interpolated and some not. **It is registered here in advance as a REAL possible
outcome and NOT as an instrument failure.**

**THIS INSTRUMENT IS UNUSUAL IN THIS PROJECT AND IT IS SAID PLAINLY: A DECISIVE READING ON ONE
NIGHT IS SUFFICIENT AND NO REPLICATION IS REQUIRED.** The reason is that the signature is
arithmetic rather than physiological — **quantisation is a property of the sensor, not of the
wearer, so it does not vary night to night the way a stage total does.** A second night would
re-measure the same fixed property and could not disconfirm the first unless the first were
miscounted, **which the §4 identities already check.**

**THE DECISION RULE, REGISTERED IN ADVANCE SO IT IS NOT ARGUED EITHER WAY AFTERWARD:**

- **The §4 identities hold AND some divisor reads at or near 1.00 AND `Dm` is a multiple of the
  largest such divisor** → the step is `Dm`. **DECIDED. The RRV spec is written against that
  step and no repeat night is required.**
- **The §4 identities hold AND every ratio sits at or near its 1 ms null AND `Dm` 1** →
  resolution is 1 ms. **DECIDED. The RRV spec is written against 1 ms and no repeat night is
  required.**
- **The §4 identities hold AND `Dm` reads a step the divisor set does not test, with the ratios
  consistent with it** → the step is `Dm`. **DECIDED — this outcome needed a further night
  under the divisor-only form and does NOT need one now.**
- **The §4 identities hold AND `Dm` DISAGREES with the divisor ratios** → **NOT DECIDED.** The
  mixture case. **A further night with a divisor set chosen against THIS night's `Dm` is
  required, and this is the ONLY outcome that requires one.**
- **An identity FAILS** → a counter is misplaced. **The reading is VOID, not inconclusive**, and
  the instrument is fixed before any night is read against it.

**A DECIDED READING IS NOT A LICENCE TO SKIP ANYTHING ELSE.** It settles the resolution input
assumption and NOTHING MORE — every other RRV precondition in §7 stands untouched.

## §9 — Rule 2 and Rule 3 checks

**RULE 2 — NO NEW CONSTANT ENTERS THE CODE.** The five divisors are the definition of the
measurement, not thresholds, and no decision reads them. `HRV_STALE_SEC`, the range band, the
jump multiplier and `HRV_BUF_MAX` are read and UNMOVED. **The §6 table is arithmetic in the
guidance and is not in the code.**

**RULE 3 — THIS SPEC REGISTERS NO SCORED CRITERION**, so there is nothing to check for
vacuity, with ONE exception: **the §4 identities ARE checkable and a night MUST check them.**
A reading that would make them PASS is any correctly-counted night, including one where every
counter reads 0 with `Dn` 0 on a session that produced no accepted intervals. A reading that
would make them FAIL is any nesting violation. **Neither is unreachable.**

**CHECKED AGAINST THE UNEVALUABLE-CHECK DEFECT: all seven values are on DIAG 4, which is captured
live in the stop sequence.** They key on RAM statics and **DIE WITH THE SCREEN** — captured
live or lost, exactly as the cadence, stillness and RSA values are.

**CHECKED AGAINST THE THIRD VACUITY FORM:** the identities can pass and can fail, and §6 names
a reading that establishes each hypothesis. **No line here is a criterion that cannot fail or a
criterion whose failing reading means something other than what it appears to mean.**

## §10 — Comparability, and the capture sequence

**INSTRUMENT ONLY.** No classifier input, no stage decision, no total, no stored field, no
persisted structure. **`CLASSIFIER_SERIES` STAYS 14 AND `NIGHT_SUMMARY_VERSION` STAYS 3, TO BE
VERIFIED BY READING `src/c/storage.h` AFTER IMPLEMENTATION AND NOT ASSERTED.**

**THE FIRST NIGHT UNDER THIS INSTRUMENT REMAINS COMPARABLE TO N26, N27 AND N28 ON EVERY STAGE
LINE.** The three comparability keys are unaffected.

**THE CAPTURE SEQUENCE CHANGES AND THIS IS THE PART THAT LOSES DATA IF IT IS WRONG.** The
capture count goes from SEVEN to EIGHT, and **the walk back up the chain is now FIVE presses,
not four.** Read from source 2026-08-22: BACK on DIAG 3, 2 and 1 walks back up; HYPNO's BACK
returns to RESULTS; **RESULTS' BACK goes to IDLE and does NOT exit the app**; BACK on IDLE is
what pops the window.

1. **RESULTS** — capture.
2. **DOWN to HYPNO** — capture.
3. **DOWN to DIAG 1** — capture. Nine lines.
4. **DOWN to DIAG 2** — capture. Nine lines.
5. **DOWN to DIAG 3** — capture. Nine lines.
6. **DOWN to DIAG 4** — capture. **THREE LINES. NEW.**
7. **UP five times back to RESULTS, then BACK to IDLE** — one screen per press.
8. **DOWN to RUNS** — capture. Eleven lines.
9. **UP to IDLE. Stop.**

**ONE COST IS REGISTERED HONESTLY: this adds five divisibility tests, six increments and one
absolute-difference comparison in the HRV event handler, which runs once per beat.** **NO PREDICTION IS MADE about its power cost and
NONE MAY BE INFERRED FROM ONE NIGHT** — P4's band and battery's integer-percent resolution are
unchanged by this spec.

## §11 — Recorded for a separate spec, NOT specified here

**`rej_quality` IS A THIRD REJECTION PATH AND NO READOUT SPEC HAS EVER NAMED IT.** Read from
source 2026-08-22: `hrv_buf_add` rejects on `quality == 0` BEFORE the range gate, incrementing
both `rejected` and `rej_quality`. **`rsa-feasibility-readout-spec-v1` §1.5 names only the
range and jump gates as the discard paths, so its account of what CAUSES a discard is
INCOMPLETE.** `Gp` counts discard-adjacent positions regardless of which gate fired, so **`Gp`
itself is not wrong** — but a session's `rej_quality` has never been rendered or read on any
night.

**THIS IS DELIBERATELY NOT FOLDED INTO THIS SPEC.** It is a real discovery and a cheap one —
one counter already maintained, one render line — **but it is not the resolution question, and
a spec written to answer one thing stays answerable by not absorbing a second.** It is entered
in the diagnostic queue and gets its own dated spec.

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**

**NOTHING IS IMPLEMENTED FROM THIS FILE UNTIL IT IS FROZEN AND PUSHED.**
