# Recovery - REM Collapse Measurement Specification v1

**FROZEN 2026-08-09, BEFORE N16 EXISTS.**

This document defines a MEASUREMENT-ONLY change made after the N13-N15
comparability group closed. It is dated before the first night that runs it,
for the same reason docs/smoothing-spec-v1.md (7f6c250) and
docs/base-spec-v1.md (876a6d6) were: so that no parameter in it can have been
chosen by looking at the data it will be judged against.

**Nothing in this document amends 7f6c250 or 876a6d6.** Both remain frozen and
untouched. No threshold moves. No stage decision changes. See section 1.

Target: the top item of the post-group-close diagnostic queue - prv_base_redecide
and the 2.0x REM threshold measured against a whole-night BASE.

---

## 0. Why this change exists

The N13-N15 group closed with the REM collapse three-for-three:

| Night | BASE | rem_gate (2.0x BASE) | REM min | Rendered bands | Context |
|---|---|---|---|---|---|
| N13 | 2821 | 5642 | 5 | 1 | alcohol 2 units, sugary snack |
| N14 | 4542 | 9084 | 0 | 0 | alcohol-free, late salty snack |
| N15 | 4846 | 9692 | 0 | 0 | fully clean, no flags |

For reference, the last night of the OLD code: N12 BASE 641, rem_gate 1282,
REM 179 min / 7 bands.

Three identical-code nights across three different context profiles produced
the same result. **Alcohol does not survive as an explanation.** The collapse
tracks the code change, not the nights.

### The two candidate mechanisms

**Both are stated as hypotheses. Neither is established. This spec does not
choose between them - it measures which one is true.**

**H1 - WINDOW SCALE MISMATCH.** Each BASE sample is
hrv_ppi_variance(&s_night_buf), a **400-beat** window (HRV_BUF_MAX 400,
hrv_math.h line 4). Each per-minute value is
hrv_ppi_variance(&s_minute_buf), a **one-minute** window - about 63 beats at
N15's 63.2 mean bpm (main.c line 134). The BASE window therefore spans roughly
6.3x more time than the values it gates, and contains between-minute variation
that a single-minute window structurally cannot. If the two populations barely
overlap, v greater than 2x BASE is close to unreachable by construction.

**Known weakness of H1, recorded so it is not quietly dropped:** this mismatch
existed identically under the old code, which produced 179 REM minutes on N12.
H1 alone therefore cannot explain why REM worked before af318da and stopped
after. It can only be a contributing structural factor.

**H2 - THE ANCHOR IS INFLATED BY THE MINUTES IT SHOULD DETECT.** The old BASE
medianed the first ~143 minutes past onset only (BASE_SAMPLE_MAX 24). The new
BASE medians the whole night. Observed BASE rose roughly sevenfold across the
series break. If high-variance late-night minutes are REM, including them in
the median raises the very anchor meant to gate them, and the anchor defeats
itself. **H2 tracks the change; H1 does not.**

### Why this is measured rather than fixed

**The distribution of per-minute variance has never been recorded on any
night.** base-spec-v1 section 4 makes s_epoch_var RAM-only and it does not
survive app exit. Every night to date records only that ZERO minutes cleared
the gate - not how far short they fell, nor whether they fell short of the
anchor or only of the multiplier.

Changing the 2.0x multiplier now would be tuning blind against a mechanism that
has not been measured. **The multiplier is untouched by this spec.** One night
of distribution data separates H1 from H2; see section 5.

---

## 1. Relationship to the frozen specs

docs/smoothing-spec-v1.md (7f6c250) and docs/base-spec-v1.md (876a6d6) are

**not modified, superseded, or renegotiated by this document.**

- **No numeric parameter in either spec moves.** Not the 2.0x REM multiplier,
  not the 0.5x Light multiplier, not BASE_SAMPLE_MAX 160, not a transition
  probability, not the 0.75 emission confusion, not a REM latency multiplier,
  not a minimum episode duration.
- **No stage decision changes.** prv_base_redecide's arithmetic, its four skip
  rules, and its s_mins bookkeeping are untouched.
- **The ordering in base-spec-v1 section 3.5 is preserved exactly.** The
  measurement pass is appended after it and cannot affect it - see section 3.4.
- **sleep_stage_classify() is untouched.** The live classification path,
  onset marking, and the Awake branch are all unchanged.

**The output of N16's staging is bit-identical to what the same night would
have produced on af318da.** This is the property that makes the change safe to
run mid-diagnosis, and section 5 registers a check that it held.

---

## 2. Scope and non-scope

**In scope:** computing summary statistics over two arrays that already exist
in RAM at stop time, persisting them in NightSummary, and displaying them.

**Explicitly NOT in scope, and untouched:**

- **Every threshold and multiplier.** See section 1.
- **EpochRecord.** It has **no version field** (storage.h lines 8-15), so
  growing it pulls in a silent-data-loss blast radius. **This spec does not
  touch it.** Per-minute variance stays RAM-only per base-spec-v1 section 4.
- **The Awake branch, onset marking, and the wake rule.** The standing Awake
  over-call is judged on its own trend, separately.
- **Deep.** Still not natively classified. Next-action 4 remains HELD.
- **OSrest.** health_service_sum's proration defect is queued separately and is
  NOT fixed here. One change at a time.
- **The smoother.** Runs unchanged, in its existing position.

---

## 3. Definition

### 3.1 The measured population

Statistics are computed over the entries of s_epoch_var[0 .. s_epoch_var_count-1]

**for which v is greater than 0**. Zero entries are minutes with too few beats
to produce a variance; including them would drag the median toward zero and
misrepresent the distribution. The count of qualifying entries is itself
recorded (v_count), so the exclusion is visible rather than hidden.

**v_count also detects EPOCH_VAR_MAX truncation.** s_epoch_var caps at 960
entries (16 h). If s_epoch_var_count reads exactly 960 the cap was reached,
and the statistics describe a truncated night. Recording the count makes that
detectable rather than silent - the same failure discipline base-spec-v1
section 4 and the OSrest findings both require.

### 3.2 The statistics

Over the qualifying population, sorted ascending, with n = v_count:

- **v_max** - the largest per-minute variance of the night.
- **v_median** - element at index n/2 using integer division. For even n this
  takes the upper-middle element rather than averaging the two central values.
  **Stated explicitly so the figure is reproducible; no averaging, no floats.**
- **v_p90** - element at index (n * 9) / 10, clamped to n-1. Integer only.
- **v_over_gate_count** - the number of qualifying entries for which
  v is greater than base_final * 2. **Expected to be 0 on a night that
  reproduces the collapse; it is recorded so that "zero" is a measured value
  rather than an inference from REM being 0.**

Over s_base_samples[0 .. s_base_sample_count-1]:

- **base_min** and **base_max** - the extremes of the BASE sample
  distribution. BASE itself is already recorded as their median. These two
  values show whether the median sits inside a tight cluster or a wide spread,
  which bears directly on H2.

### 3.3 Sorting

Sorting is permitted **in place on s_epoch_var**, and only after
prv_base_redecide has completed. At that point no code reads the array by index
again, so the index-to-epoch correspondence is no longer needed. This avoids a
960-entry (3840-byte) scratch copy.

s_base_samples is sorted by the existing prv_base_median() into its own static
scratch and is NOT re-sorted; base_min and base_max are taken by a single
linear scan, which needs no sort at all.

**Fixed-point / integer only. No floats.** Unchanged from existing practice.

### 3.4 Ordering - forced, not chosen

```
prv_close_minute()
-> prv_base_median() (876a6d6 s3.3, unchanged)
-> prv_base_redecide(base_final) (876a6d6 s3.4, unchanged)
-> MEASUREMENT PASS (this spec)
-> smoother_run(s_mins) (7f6c250, unchanged)
-> s_mins[StageDeep] overwrite from OSrest
-> NightSummary assembly
```

**The measurement pass MUST run after prv_base_redecide**, because it sorts
s_epoch_var in place and redecide reads that array by index.

**It reads only.** It writes no EpochRecord, touches no s_mins element, and
changes no stage. Its position relative to smoother_run is therefore immaterial
to output; it is placed before for readability. **This is the mechanical
guarantee behind section 1's bit-identical claim.**

### 3.5 Persistence

NIGHT_SUMMARY_VERSION goes from **1 to 2**. Seven fields are appended to the
END of the struct, after batt_end_pct:

```c
uint32_t v_max;
uint32_t v_p90;
uint32_t v_median;
uint32_t base_min;
uint32_t base_max;
uint16_t v_count;
uint16_t v_over_gate_count;
```

Twenty-four bytes appended. **Appended at the end, never inserted**, so every
prior field keeps its offset.

**The read path must accept BOTH sizes.** storage.h already documents version 0
as pre-versioning, so the mechanism exists. A record whose version is below 2
has its seven new fields treated as absent and **displayed as `--`, never as
0** - zero is a meaningful measured value here and must not be confused with
"not recorded". **This is the requirement next-action 8 exists for, and it is
met rather than deferred.**

### 3.6 Display

The seven values are shown on a **new DIAG screen**, not crowded onto RESULTS.
RESULTS is unchanged, so every existing recorded-value convention and every
prior night's screen-reading procedure still applies.

Suggested layout, one screen:

```
Diag
Vmax <v_max>
P90 <v_p90> Vmed <v_median>
Vn <v_count> Gate <v_over_gate_count>
Bmin <base_min> Bmax <base_max>
```

**The button wiring is an implementation decision, not a measurement one**, and
is recorded in the commit message rather than fixed here. The IDLE screen is
already flagged as diagnostic-only and due for release polish (queued item 7);
DIAG joins it under that same item and is likewise not a release screen.

---

## 4. Implementation constraints

- **No EpochRecord change.** See section 2.
- **No new large allocation.** In-place sort per section 3.3. Added static
  footprint is the 24 struct bytes only.
- **Sort cost.** Up to 960 elements once, at stop, on a screen the user is not
  waiting on. An insertion sort is acceptable; qsort from the standard library
  is preferred if available. **If the sort measurably delays the stop
  transition, report it - do not silently switch algorithms mid-night.**
- **Integer only. No floats.**
- **Single commit** on ~/recovery main: `git commit -s` plus the
  `Co-Authored-By: Claude <noreply@anthropic.com>` trailer, both counts
  verified = 1, karthakon identity, before push.

---

## 5. What N16 can and cannot establish

### N16 CAN separate the two hypotheses

Let BASE be the whole-night median and GATE be 2 x BASE.

- **H1 SUPPORTED** if **v_max is far below BASE** - the minute-scale and
  400-beat-scale populations barely overlap, and no minute could clear the gate
  regardless of what the multiplier were set to. The scale mismatch is then the
  operative constraint.
- **H2 SUPPORTED** if the distribution **straddles BASE normally** (v_median
  within a small factor of BASE, v_max above BASE) **but v_p90 and v_max both
  fall below GATE.** The anchor sits in a defensible place and the 2.0x
  multiplier is what is unreachable.
- **BOTH WRONG** if **v_over_gate_count is greater than 0** while rec_rem_min
  is 0 - minutes cleared the gate and something downstream of the re-decision
  discarded them, which would point at the smoother or the band renderer rather
  than the anchor. **Registered in advance as a live outcome.**
- **NEITHER PATTERN** - if the numbers match none of the above, both hypotheses
  are wrong and that must be reported as the result it is, not forced into the
  nearest branch.

### N16 CANNOT establish these

1. **What the multiplier should be.** Measuring where the distribution sits
   does not license fitting a threshold to it. Any threshold change is a
   separate dated spec, frozen before the night that tests it.
2. **Anything about spread.** One night has no spread. The closed group's 1.72x
   result stands as reported on three nights and is **not** revised by N16.
3. **Whether REM is truly absent.** There is no ground truth. Garmin is not
   ground truth. The measurement describes the instrument, not the sleep.

### The check that the change was inert

**Registered in advance:** N16's staging output must be explicable by the same
mechanism as N13-N15. The measurement pass reads only (section 3.4), so any
gross discontinuity in Awake percentage, rejection rate, or the P8 identity
would indicate the change was NOT inert and must be diagnosed before any number
on the night is interpreted. **P8 in particular is the tripwire: it has held
six-for-six and is pure s_mins bookkeeping.**

---

## 6. Comparability

**recovery_commit changes, so N16 opens a NEW comparability group** by the
group definition already in force. classifier_series stays **6** and
smoother_version stays **v1**, because neither the classifier nor the smoother
changes. firmware stays **v4.32.0** with no flash - an app install is not a
firmware flash, so watch_slot stays **1**.

**Registered honestly, in advance:** because this change is provably inert with
respect to staging (section 3.4), N16's staging numbers are mechanically
comparable to N13-N15 even though the group key differs. **That is an argument
for reading them together, NOT a licence to merge the groups or to extend the
closed group's spread result.** The 1.72x result was reported on three nights
and stays reported on three nights.
