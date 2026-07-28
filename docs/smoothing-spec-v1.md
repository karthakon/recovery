# Recovery — Temporal Smoothing Specification v1

**FROZEN 2026-07-28, BEFORE N7 EXISTS.**

Every numeric value in this document was chosen from published sleep
physiology and from structural properties of the classifier. **No value in
this document was chosen by looking at N1–N6 output, and no value may be
changed by looking at N7.** If smoothed output looks wrong after N7, the
diagnosis is the classifier (`sleep_stage_classify()`), not these
parameters. This is next-action 3's own rule and it is the reason this
document is dated before the data.

Target: next-action 3. Prerequisite for next-action 4 (native Deep).

---

## 0. Scope and non-scope

**In scope:** relabelling the per-minute stage array at stop time.

**Explicitly NOT in scope, and untouched by this spec:**

- **Onset marking.** `s_onset_marked` is set live and stays live.
- **BASE sampling.** `s_base_samples` fills live, gated on `s_onset_marked`,
  every 400 accepted beats. Unaffected.
- **The wake rule** in `sleep_stage_classify()`. The Awake over-call
  (N6: 17.4% vs Garmin 0.2%) is a standing divergence needing a trend, and
  this spec does not correct it. See §5.
- **The 2.0× REM multiplier.** Not touched, per standing instruction.
- **`BASE_SAMPLE_MAX 24`.** Separate question, deferred past N7.

**Why the separation matters:** BASE sampling gates on onset; onset depends
on Awake. If the smoother could move onset, it would move BASE, and no
future BASE-stability comparison would be interpretable. Running the
smoother as a stop-time post-pass makes the two mechanically independent.

**Consequence for the UI:** the live SESSION screen continues to show raw
per-minute classification. RESULTS totals and the hypnogram show smoothed
output. These will disagree, by design. If both are ever displayed
together, label them.

---

## 1. Model

Two stages, applied in order at stop time:

1. **Viterbi decoding** over a first-order HMM with a stage-transition
   prior. Global-optimal path over the whole night.
2. **Minimum episode duration pass**, Webster-style rescoring, applied to
   the Viterbi output.

Reference practice: Webster et al. rescoring rules (post-Cole-Kripke),
HMM/Viterbi decoding with a transition prior, minimum-episode-duration
filtering. This is the standard shape; nothing here is novel.

**Epoch = 1 minute**, matching the classifier's existing granularity.

**States, current (3):** `StageAwake`, `StageREM`, `StageLight`.
`sleep_stage_classify()` has no `return StageDeep`, so Deep is not a state
yet. §6 gives the 4-state extension to apply **at the same time as**
next-action 4, not before.

---

## 2. Transition matrix

Self-transition probability is derived from mean episode duration `d` as
`p_self = 1 - 1/d`. Durations are population means for adult sleep.

| Mean episode duration used | Value | Source of the number |
|---|---|---|
| Awake (intra-sleep) | 6 min | mixes brief arousals with long consolidated wake blocks |
| REM | 18 min | REM episodes run 10–25 min |
| Light (N1+N2) | 25 min | Light is the dominant state and the hub of transitions |

**Matrix — rows are FROM, columns are TO. Rows sum to 1.000.**

| | → Awake | → REM | → Light |
|---|---|---|---|
| **Awake →** | 0.833 | 0.017 | 0.150 |
| **REM →** | 0.014 | 0.944 | 0.042 |
| **Light →** | 0.018 | 0.022 | 0.960 |

Off-diagonal mass is distributed on three physiological grounds:

- **Light is the hub.** Nearly all stage transitions pass through Light.
  Both Awake and REM send the bulk of their exit probability there.
- **Awake → REM is penalised**, not forbidden (0.017). A sleep-onset REM
  period is a genuine clinical abnormality, but forbidding it entirely
  would make the state unreachable after any arousal, which is wrong.
- **REM → Awake stays live** (0.014). Dream-wakes are real and Kristopher
  has reported them on prior nights.

**Implementation note:** work in fixed-point log-probability, not floats.
Store as `int16_t` log2-probability scaled by 256. No FPU dependency, no
underflow over a 600-epoch path.

---

## 3. Emission model

The classifier emits **hard labels**, not probabilities. Convert with a
single flat confusion parameter:

```
P(observed = true state)        = 0.75
P(observed = each other state)  = 0.125    (3-state: 0.25 / 2)
```

**Flat, deliberately.** No per-state trust weighting. A per-state weight
would be the place where knowledge of N1–N6 output could leak into the
parameters — in particular, down-weighting Awake would be a backdoor wake
rule change. Flat emissions mean **the transition prior does the work**,
which is the whole point of the model.

`0.75` is chosen as "the classifier is usually right but not authoritative."
It is not calibrated against any night, because no ground truth exists to
calibrate against — Garmin is not ground truth (see the handoff's "Framing
the claim"), and polysomnography is not available.

### REM latency prior

REM latency is **~90 min from onset**. It shortens with alcohol and
fragmented sleep, so this suppresses early REM rather than forbidding it:

| Minutes after onset | Multiplier on REM emission likelihood |
|---|---|
| 0 – 45 | 0.25 |
| 45 – 90 | 0.50 |
| 90+ | 1.00 |

Applied to the REM emission term only, before Viterbi. **Never zero.**

**Deliberately NOT implemented as priors, despite being real:**

- *SWS concentrated in the first third of the night* — needs a Deep state
  (§6). Add with next-action 4.
- *REM episodes lengthen toward morning* — would require a time-varying
  transition matrix. Real, but more machinery than the confetti problem
  needs. Revisit only if consolidated episodes come out uniformly sized.

---

## 4. Minimum episode duration pass

Applied **after** Viterbi, on its output.

| State | Minimum episode | Justification |
|---|---|---|
| REM | 5 min | shortest physiological REM period; the first is often 5–10 min |
| Light | 3 min | Light legitimately appears as short bridges between stages |
| Awake | 2 min | brief arousals are real and clinically meaningful |

**Absorption rule for a sub-minimum episode:**

1. Relabel it to whichever adjacent episode is **longer**.
2. On a tie, relabel to the **following** episode.
3. At the start or end of the night, where only one neighbour exists, use
   that neighbour.

**Iterate until stable, maximum 3 passes.** Absorption can create a new
sub-minimum episode by merging; three passes is ample for a 600-epoch night
and the cap guarantees termination.

**Order matters and is fixed here: shortest-episode-first.** Process all
sub-minimum episodes in ascending length order within each pass, so the
most obviously spurious minutes are absorbed before longer marginal ones.

---

## 5. What this spec does NOT fix, stated so it is not misread later

**The Awake over-call.** N5: 25.1% vs Garmin 1.9%. N6: 17.4% vs 0.2%. The
minimum-episode rule will absorb isolated single Awake minutes and the
transition prior will discourage Awake→REM, so the Awake fraction will
probably fall somewhat. **That is a side effect, not a fix, and it must not
be reported as one.** The divergence is an order of magnitude and its cause
is in the wake rule, which this spec does not touch. Judge the wake rule on
its own trend, separately.

**Total-vs-structure.** N6 got REM *total* nearly right (17.4 vs Garmin
18.1) while getting REM *structure* completely wrong (~2.5-min mean episode
against a real 10–25 min). This spec targets structure. **A post-smoothing
total that moves away from Garmin is not necessarily a regression** — the
pre-smoothing agreement may have been coincidence, as v27 concluded for N5.

---

## 6. Four-state extension — apply WITH next-action 4, not before

When `sleep_stage_classify()` gains `return StageDeep`, extend as follows.
These values are frozen now for the same reason as the rest.

Deep mean episode duration **22 min** → `p_self = 0.955`.

| | → Awake | → REM | → Light | → Deep |
|---|---|---|---|---|
| **Awake →** | 0.833 | 0.017 | 0.140 | 0.010 |
| **REM →** | 0.014 | 0.944 | 0.040 | 0.002 |
| **Light →** | 0.018 | 0.022 | 0.930 | 0.030 |
| **Deep →** | 0.005 | 0.002 | 0.038 | 0.955 |

- **Deep ↔ REM is near-forbidden in both directions** (0.002). Direct
  transitions between them are vanishingly rare; both route through Light.
- **Deep → Awake is very low** (0.005). Waking directly from slow-wave
  sleep happens but is uncommon and disorienting.

Emission confusion becomes `0.75` correct, `0.0833` for each of the three
others.

Deep minimum episode: **5 min**.

**SWS-first-third prior**, to add at the same time — the mirror of REM
latency:

| Fraction of night elapsed since onset | Multiplier on Deep emission |
|---|---|
| 0 – 33% | 1.00 |
| 33 – 66% | 0.60 |
| 66 – 100% | 0.30 |

Never zero. Late-night SWS is reduced, not absent.

---

## 7. Implementation constraints

- **Runs once, at stop time**, inside or immediately before
  `prv_stop_recording`, over the stored per-minute stage array that already
  backs the hypnogram.
- **Memory:** Viterbi over ~600 epochs × 4 states needs a 600×4 backpointer
  array = 2400 bytes, plus two 4-wide cost rows. Current app RAM footprint
  is 11115 bytes of 128 KB with 119957 bytes free heap, so this fits with
  large margin. **Cap the epoch array at 960 (16 hours)** and degrade
  gracefully past it rather than overrun.
- **Fixed-point only.** No floats.
- **Keep the raw array.** Smooth into a second array so raw and smoothed
  are both available for comparison during testing. Drop the raw copy only
  when the smoother is trusted.
- **Separate commit**, on `~/recovery` `main`, after N7 reads. `git commit
  -s` plus the `Co-Authored-By: Claude` trailer, verified before push.

---

## 8. Validation

**N8 is the first night on the smoother** (N7 runs `d4eaa6b` unchanged).

Judge on **episode structure**, which is what the spec targets:

- **Mean REM episode length.** Currently ≈2.5–3 min. Target 10–25 min.
  This is the primary success criterion.
- **REM episode count.** N6 drew ~35–40 spikes for 99 minutes. Expect
  4–6 consolidated episodes on a 9-hour night.
- **Visual: does the hypnogram read like Garmin's block structure** rather
  than confetti.

Do **not** judge on stage totals moving closer to Garmin. See §5.

**If structure is still confetti after smoothing**, the classifier is
producing near-random per-minute output and no smoother can fix it —
diagnose `sleep_stage_classify()` and its BASE-relative thresholds. **Do
not retune these parameters.**
