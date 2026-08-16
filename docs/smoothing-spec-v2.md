# smoothing-spec-v2 - the REM latency prior

**Frozen on commit. Never edited. A change is a v3 or a dated correction.**

**SUPERSEDES the REM-latency table of `smoothing-spec-v1` ONLY. Everything
else in `7f6c250` stands unchanged** - the Viterbi decode, the transition
matrix, the emission costs, the minimum-episode pass, the episode minimums for
every stage, and section 8's rule that a structure result diagnoses the
CLASSIFIER.

## 1. What is wrong

**THE LATENCY PRIOR ONLY EVER SUPPRESSES. IT NEVER PROMOTES.** The table runs
0.25x, 0.50x, 1.00x and stops at neutral. **Late-night REM receives no
assistance at any point in the night.**

**THAT IS ASYMMETRIC WITH THE PHYSIOLOGY THE PRIOR CLAIMS TO ENCODE.** REM
pressure builds across a night: later cycles carry longer REM episodes and a
higher REM proportion. **A prior that encodes "REM is unlikely early" and
stops there models half of a two-sided fact.**

**THE EARLY PENALTY IS CORRECT AND IS NOT RELAXED.** Sleep-onset REM is
genuinely rare in healthy adults. **The first two rows keep their present
values exactly.**

## 2. The change

**The table is EXTENDED past neutral. The existing rows are UNCHANGED.**

| Minutes since onset | Multiplier | Value | Status |
|---|---|---|---|
| pre-onset | 0.25 | -512 | UNCHANGED |
| 0 to 45 | 0.25 | -512 | UNCHANGED |
| 45 to 90 | 0.50 | -256 | UNCHANGED |
| 90 to 180 | 1.00 | 0 | UNCHANGED |
| 180 to 300 | 2.00 | **+256** | NEW |
| 300 or more | 4.00 | **+512** | NEW |

**THE VALUES CONTINUE THE EXISTING SEQUENCE AND ARE NOT NEW CONSTANTS.** Every
row is `log2(multiplier) * 256` and the multipliers are the existing doubling
ladder continued in the direction it already ran. **Rule 2: no row is a
measurement promoted to a rule.**

**THE BOUNDARIES ARE STANDARD SLEEP ARCHITECTURE.** At roughly 90-minute
cycles, 180 minutes is about the third cycle and 300 minutes about the fifth.
**They are not drawn from any recorded night.**

## 3. What is NOT changed, with reasons

**`MIN_EP_REM` STAYS AT 5.** Raising it was considered and REJECTED FOR NOW.
**A higher episode minimum DELETES short REM episodes by absorbing them into
their neighbours, and while REM is scarce that makes the reported total
smaller, not more honest.** **A minimum-block rule is correct for a classifier
that is already producing REM and is premature for one that is not.**

**REGISTERED AS PENDING, NOT ABANDONED:** a REM episode minimum of 10 minutes
is the intended end state. **It is revisited once the classifier produces REM
episodes at all, and it is a v3 or a dated correction when it happens.**

**`MIN_EP_PASSES` STAYS AT 3.** It was to be raised alongside a higher episode
minimum; with the minimum unchanged, the reason is gone. **Raising it in
isolation would only absorb MORE short REM episodes.**

**THE ABSORPTION DIRECTION IS RECORDED AS A FINDING:** `min_episode_pass`
absorbs a sub-minimum episode into its LONGER NEIGHBOUR. On a Light-dominated
night that neighbour is almost always Light, **so the pass systematically
converts short REM into Light and never the reverse.** **Not changed here.
Recorded so it is not rediscovered.**

## 4. Registered in advance

**THIS SPEC CANNOT CREATE A REM MINUTE.** The smoother decodes over minutes
the classifier already labelled. **If admission is the binding constraint, no
latency table changes the outcome** - `smoothing-spec-v1` section 8 already
directs a structure result to the classifier and that rule is unchanged.

**WHAT IT CAN DO:** let REM-observed minutes in the later part of a night form
runs that survive the decode, where previously they were merely unpenalised.

**WHAT IT CANNOT ESTABLISH:** whether any surviving episode is correctly REM;
whether the boundaries are correct; whether the early penalty is correctly
sized. **No band is registered on any of these.**

## 5. Status

**Frozen on commit. No section may be edited retroactively. A change is a
superseding dated file or a v3, never an edit.**
