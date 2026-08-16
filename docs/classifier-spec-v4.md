# classifier-spec-v4 - the T1 admission threshold

**Frozen on commit. Never edited. A change is a v5 or a dated correction.**

**SUPERSEDES `classifier-spec-v1` section 3.4 AND the T1 term of
`classifier-spec-v2` section 4. Everything else in both stands unchanged** -
`D(m)`, `F(m)`, the anchor definitions, T2, T3, and the multipliers on the
Light band's LOWER edge.

## 1. What is wrong

**The REM admission threshold is a fixed multiple of the anchor, and that
multiple has NO RECORDED SOURCE.** It is stated in `classifier-spec-v1`
section 3.4, declared out of scope by that spec's own section 2, and carried
forward unexamined by v2 and v3. **Every other constant in this classifier
cites a within-night statistic, a published result or an SDK header. This one
cites nothing.**

**ITS ORIGIN IS INFERABLE FROM `classifier-spec-v1` SECTION 1 AND IS A RULE 2
DEFECT.** Before v1 the per-minute feature was compared against an anchor
computed over a 400-beat window - a documented SCALE MISMATCH in which the
median minute sat well below the anchor. **A 2x multiple is a plausible
working threshold against a systematically inflated anchor.** v1 then replaced
BOTH the feature and the anchor, making the anchor the within-night MEDIAN of
the same feature - so the median minute now sits at approximately 1.0x BY
CONSTRUCTION. **The multiplier was never re-derived after the thing it was
calibrated against ceased to exist.**

**THE DECISION IS A BAND, NOT A SINGLE GATE, AND THAT IS THE LARGER PROBLEM.**
The Light branch admits everything from half the anchor to double it - a
FOURFOLD window centred on the median - and it is tested FIRST. **The bulk of
the night's distribution is decided Light before REM is ever considered.**

**THE CONSEQUENCE, OBSERVED AND NOT PREDICTED:** the admitted population has
been a small single-digit percentage of the night while published adult REM is
20-25 percent of total sleep. **T2 and T3 then subtract from an already tiny
pool.** No downstream change can create a minute the admission gate never
passed - **not the smoother, not the episode floor, not the latency prior.**

## 2. The change

**T1 ADMITS A MINUTE WHEN `F(m)` EXCEEDS `A_D`.** The upper edge of the Light
band moves from `2 * A_D` to `A_D`.

**THE LOWER EDGE IS UNCHANGED.** `0.5 * A_D` keeps its present role and its
present value.

**THE DECISION CHAIN KEEPS ITS PRESENT SHAPE AND ORDER.** Light is still
tested first, REM still requires the conjunction of all three terms, and the
final else still falls to Light. **Only the band's upper edge moves.**

## 3. Why `A_D` and not another value

**`A_D` IS THE WITHIN-NIGHT MEDIAN of `F(m)` over the non-Awake population.**
Admitting `F(m) > A_D` therefore admits approximately the upper HALF of that
population as REM CANDIDATES - not as REM.

**IT IS A PERCENTILE OF THE NIGHT'S OWN DISTRIBUTION, NOT A FITTED CONSTANT.**
It requires no cross-night history, works on the first night ever recorded,
and survives data loss. **This is the same idiom as `A_D` and `A_H`
themselves.**

**IT IS SYMMETRIC WITH T2**, which is already a median split on the night's
own HF distribution. **The two admission terms now have the same form.**

**THE FUNNEL IS THE JUSTIFICATION, NOT THE ADMISSION RATE.** T1 admits about
half the population; T2 removes about half of what T1 admits, by construction;
T3 removes more. **REM therefore lands well below the admitted fraction and in
the neighbourhood of published adult REM proportion.** **NO STEP OF THIS IS
FITTED TO ANY NIGHT'S OUTCOME.**

**PUBLISHED ADULT REM IS 20-25 PERCENT OF TOTAL SLEEP TIME.** That is
population physiology and is the ONLY external number this spec relies on.
**It is NOT a target, no parameter is moved toward it, and no statistic is
adjusted to approach it.**

## 4. Registered in advance - THIS IS EXPECTED TO OVER-REPORT

**REM IS EXPECTED TO READ HIGHER THAN BEFORE AND MAY READ TOO HIGH. THAT IS
THE INTENDED DIRECTION OF THIS CHANGE AND IS NOT A DEFECT.**

**THE REASON IS REGISTERED SO IT IS NOT RE-LITIGATED:** a zero reading carries
no information about where the threshold should sit. **An over-call is
measurable and can be scaled back with evidence; an under-call at zero cannot
be.** Scaling back is a v5 or a dated correction and is decided against a
recorded distribution, **never against agreement with another device.**

**LIGHT WILL FALL SUBSTANTIALLY.** The Light band collapses from a fourfold
window to a twofold one. **This is arithmetic, not a finding.**

**WHAT THIS SPEC CANNOT ESTABLISH:** whether the admitted minutes are
correctly REM - there is no ground truth. Whether `A_D` is the right
percentile - only a recorded distribution can inform that. Whether T2's median
split is the right second term - review finding 4 remains instrumented and NOT
acted on. **`t2` is NOT changed by this spec.**

## 5. The distribution readout

**A percentile summary of `F(m)` over the T1 population is rendered on DIAG:
p50, p75, p90 and p95, plus the population count.** RAM-only, session-scoped,
**recorded and NOT scored.**

**IT CHANGES NO DECISION.** Its purpose is to make any future adjustment of
the admission threshold a choice against a MEASURED distribution rather than
an invented one. **The existing per-minute readouts are computed over a
DIFFERENT population and a DIFFERENT statistic and cannot serve this purpose -
verified by source read, not assumed.**

## 6. Status

**Frozen on commit. `CLASSIFIER_SERIES` bumps. No section may be edited
retroactively. A change is a superseding dated file or a v5, never an edit.**
