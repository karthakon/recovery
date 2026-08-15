# movement-spec-v1

**Frozen on commit. Never edited. A change is a v2 or a dated correction.**

## 1. Scope

Replaces the peek-based movement sampling path with an accelerometer data
subscription. **This spec governs HOW SAMPLES ARRIVE, not what is done with
them.**

**SUPERSEDES NOTHING IN classifier-spec-v3.** The three-valued MovementState,
the per-minute percentage test, MV_MOVED_PCT, the magnitude band, the
known-bit bitmaps and A's two filter sites are ALL UNCHANGED.

## 2. Trigger

The swap is authorised by ONE pre-registered branch, frozen in advance of any
reading. **NO OTHER TRIGGER AUTHORISES IT.** Firing it on any other basis
would spend a baseline for nothing.

## 3. The swap

- **prv_accel_peek is DELETED.** accel_service_peek CANNOT be used while
  subscribed - pebble.h lines 769 and 789.
- **accel_service_set_sampling_rate(ACCEL_SAMPLING_10HZ)** - pebble.h 743, 761.
- **accel_data_service_subscribe(25, prv_accel_data_handler)** - pebble.h 772.
- **accel_data_service_unsubscribe() in prv_deinit.**
- **TWO CALLS, NOT ONE.** accel_data_service_subscribe carries NO rate
  parameter - the rate is a separate setter. Read from the header, not
  recalled.
- The handler iterates num_samples and applies the EXISTING magnitude test to
  each sample, incrementing the EXISTING counters.

**THE SUBSCRIPTION IS APP-LIFETIME, NOT RECORDING-SCOPED, AND THE REASON IS
REGISTERED:** prv_tick_handler calls prv_close_minute UNCONDITIONALLY, and the
peek path it replaces also ran outside recording. A recording-scoped
subscription would change idle behaviour. This matches the health
subscription exactly.

## 4. Rate and batch - CITED, NOT FITTED

**25 is the SDK's own documented cap on samples_per_update** - pebble.h 774,
stated as between 0 and 25. **10 Hz is the LOWEST rate the AccelSamplingRate
enum offers** - pebble.h 743.

**A 25-SAMPLE BATCH CANNOT COVER A MINUTE AT ANY OFFERED RATE.** At 10 Hz the
wake cadence is 24 per minute, delivering roughly 600 samples per minute
against roughly 60 on the peek path.

**RULE 2: both constants are read from an authoritative header. Neither is a
measurement promoted to a rule and neither is an unmeasured guess.**

## 5. The vibration counter

**s_vibe_samples** - RAM-only, session-scoped, zeroed at session start beside
the veto counters, incremented when a sample carries did_vibrate - pebble.h
694. Rendered on DIAG as **Vib**.

**VIBRATED SAMPLES ARE COUNTED, NOT FILTERED.** They continue to feed the
per-minute moved and sample counters exactly as before. **THIS CHANGES NO
DECISION.**

**FINDING, RECORDED SO IT IS NOT REDISCOVERED AS NEW: did_vibrate has ALWAYS
been available.** AccelData is the same struct accel_service_peek filled. The
peek path simply never read the field. **NOTHING NEW ARRIVES WITH THE
SUBSCRIPTION.**

**WHY THE SWAP MAKES THE COUNTER MEANINGFUL:** at roughly 60 samples per
minute a short vibration can fall entirely between samples; at roughly 600 a
one-second vibration spans about ten samples.

**THE REGISTERED EXPECTED RESOLUTION IS DOCUMENTATION, NOT CODE:** a README
warning that Quiet Time should cover the sleep window, plus a per-night flag
recording that notifications fired. **IF Vib READS ZERO ACROSS THREE
CONSECUTIVE NIGHTS THIS ITEM CLOSES** and is not carried further.

## 6. Quiet Time readout

**quiet_time_is_active()** - pebble.h 9003 - sampled at minute close into
**s_qt_min**, the count of minutes with Quiet Time active. RAM-only,
session-scoped, rendered on DIAG as **QT**. **RECORDED, NOT ACTED ON. IT
CHANGES NO DECISION.**

**REGISTERED SO IT IS NOT RE-INVESTIGATED: THE QUIET TIME API IS READ-ONLY.**
The 4.33.1 header exposes exactly one symbol and it returns a bool. **There is
no setter.** **AN APP CANNOT ENABLE QUIET TIME AND CANNOT PREVENT ITS BEING
DISABLED.** The header's own doc comment states the design intent - apps
RESPECT the user's choice rather than setting it.

**PURPOSE: a scheduled Quiet Time window does not cover an unusually early
sleep or an unusually late wake.** QT identifies those minutes.

## 7. The unknown-minute counter is RETAINED PERMANENTLY

**The instrument that fired this swap is the instrument that verifies it.** A
zero reading on the first night after the swap is the confirmation that the
sampling gap closed. **Removing it would spend the finding.**

## 8. Named open items - NOT addressed by this spec

- **did_vibrate FILTERING.** Removing vibrated samples from the movement
  counters is a MOVEMENT-INPUT CHANGE and needs its own night and its own
  pre-registration. **It becomes live ONLY IF Vib is non-trivial AND the
  README warning proves insufficient.** Not now.
- **THE MAGNITUDE GATE REMAINS BAND-ONLY.** A slow roll preserving 1 g still
  reads as still. **600 samples per minute through a band-only gate is more
  samples of the same blind spot.** External review finding 6's residual.
  **NOT FIXED BY THIS SPEC.**
- **AccelData.timestamp** - pebble.h 696 - makes a true per-minute sample
  cadence measurable for the first time. **Recorded as existing. Nothing is
  built on it.**

## 9. Group consequence

**CLASSIFIER_SERIES BUMPS.** The prior baseline is SPENT and the first night
on this path opens a new series as a new baseline.

**A filters on accelerometer evidence, so the anchor scalars are spent too**,
not only the atonia veto counters. Registered in advance, not discovered
afterward.

## 10. Battery - A REAL FAILURE CONDITION, NOT A RECORDED READOUT

**THE SUBSCRIPTION MUST NOT INCREASE DRAIN BY 1 PCT PER HOUR OR MORE against
the immediately preceding night on the peek path.**

**WHY THIS LINE ESCAPES THE RESOLUTION CAVEAT that limits every other battery
question:** battery reads as integer percent, and the standing objection is
that small effects sit inside that resolution. **A 1 pct per hour increase is
roughly a DOUBLING and is worth several whole points across a full night.**
That is far above the noise floor. **This criterion is answerable on a SINGLE
night.**

**CONSEQUENCE AT OR ABOVE THE THRESHOLD, REGISTERED IN ADVANCE: the
subscription is REVERTED and the hardware FIFO route is reconsidered as
FIRMWARE work.**

**WHY APP-LEVEL BATCHING CANNOT REACH THE FIFO MODEL:** the accelerometer has
a hardware FIFO and the efficient design is to let the CPU sleep and drain a
whole batch periodically. **samples_per_update is capped at 25 by the SDK** -
pebble.h 774 - **so at any offered rate a batch cannot span a minute.** **THE
DRAIN-ONCE-PER-MINUTE MODEL IS NOT REACHABLE FROM AN APP.** It lives in
firmware. Upstream LSM6DSO FIFO burst-read work exists at 8dd50aaa and
23e9a9d8. **Reaching it would require a custom fork and a new PR.**

**A HYPOTHESIS, STATED AS ONE:** at 10 Hz the accelerometer wakes 24 times per
minute while the health handler already wakes the processor far more often, so
the added wakes land on a processor that is not sleeping through the night
anyway. **This predicts an increase well below the threshold. It is NOT a
measurement and the night decides it.**

## 11. Night-file field

A per-night notification flag is added to the night-file front matter.
**IT CHANGES THE COLUMN COUNT AND THEREFORE REQUIRES A GROUP BOUNDARY** - the
first night on this path is one.

## 12. Status

**Frozen on commit. No section may be edited retroactively. A change is a
superseding dated file or a v2, never an edit.**
