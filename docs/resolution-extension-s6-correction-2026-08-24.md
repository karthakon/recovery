# resolution-extension-s6-correction-2026-08-24

**Corrects §6 of `hrv-resolution-divisor-extension-2026-08-23`, frozen and implemented at
`4cb72d1`. THAT FILE IS NOT EDITED.** A change is a superseding dated file, never an edit.

**IT CORRECTS §6 BRANCH 1, AND IT REGISTERS ONE STANDING PROCESS RULE IN §8.** §1 through §5 and
§7 through §9 of the extension stand exactly as frozen. The counters, the identities, the render
and the comparability statement are untouched. `hrv-resolution-readout-spec-v1` and its two
2026-08-23 corrections are unaffected.

**RULE 7 — THIS FILE RECORDS NO SUBJECT PHYSIOLOGICAL VALUE.** It states the decision rule as
arithmetic. **The night that exposed the defect is N30 and its readings are in
`~/recovery-nights/nights/C00-N30.md`.** The outcome of applying this rule to N30 is owed as a
separate dated file in `~/recovery-nights/docs/`, named in §7.

**NOTHING IN THE CODE CHANGES. NO DIVISOR IS ADDED, MOVED OR REMOVED. `DM_SENTINEL` IS NOT
TOUCHED.** This is a correction to a reading rule, not to an instrument.

---

## §1 — The contradiction, inside one frozen file

**§3 AND §6 OF `hrv-resolution-divisor-extension-2026-08-23` DESCRIBE THE SAME HYPOTHESIS TWO
WAYS AND THE TWO DESCRIPTIONS DISAGREE.**

**§3 states the quantitative form.** Under a 10 ms coarse component, `D20/Dn` sits near HALF the
coarse fraction plus its own null; under a 20 ms coarse component it sits at the coarse fraction
itself plus its null. §3 calls this roughly a two-fold separation and rests the whole design of
`D20` on it.

**§6 branch 1 states the same hypothesis as a position.** It reads `D20/Dn` NEAR THE 1 ms NULL
and well below `D10/Dn`.

**THOSE ARE THE SAME SENTENCE ONLY WHEN THE COARSE FRACTION IS NEGLIGIBLE.** Write `f` for the
coarse fraction and `u_d` for divisor `d`'s 1 ms null. §3's 10 ms prediction is

```
P10  =  f/2  +  ( 1 - f ) * u20
```

and §6 branch 1 asserts that quantity is near `u20`. The two agree only as `f` approaches zero.
**At any appreciable `f` the §3 prediction sits well ABOVE the null — at `f` near a quarter it is
several times the null — and a reading that matches §3's 10 ms prediction exactly will fail §6
branch 1's description of that same prediction.**

**BRANCH 2 IS NOT AFFECTED.** Under a 20 ms component `D20/Dn` is `f + (1 - f) * u20` and
`D10/Dn` is `f + (1 - f) * u10`, which differ by `(1 - f) * (u10 - u20)` — a small quantity at
any `f`. **"Close to `D10/Dn`" remains a correct description of the 20 ms case.** The defect is
in branch 1 alone, and it is therefore ASYMMETRIC — the frozen wording can send a clean 10 ms
reading to NOT DECIDED while a 20 ms reading is described correctly.

**THIS IS RULE 13'S EXTENSION IN THE SAME FORM FOUND ON 2026-08-23 IN §5 AND §10 OF
`hrv-resolution-readout-spec-v1`:** a contradiction between two sentences inside ONE file,
serving different readers, where no pairwise check BETWEEN files reaches it. §3 serves the
person deciding whether `D20` is worth adding. §6 serves the person holding the night's reading.
**Each was checked against what it was written for, and they were never checked against each
other.** **TWO INSTANCES IN TWO CONSECUTIVE SPEC FAMILIES IS A PATTERN, AND §8 TURNS IT INTO A
STANDING CHECK RATHER THAN LEAVING IT AS AN OBSERVATION.**

## §2 — Which side is right, and how that is established

**§3 IS RIGHT. §6 BRANCH 1 IS THE ERROR.**

Established three ways, none of them a reading of the others:

1. **ARITHMETIC.** Under a mixture in which a fraction `f` of intervals falls on a grid of step
   `q` and the remainder is unquantised at 1 ms, the expected fraction of intervals divisible by
   `d` is `f * [d divides q] + ( 1 - f ) * u_d`. Substituting `q = 10, d = 20` gives
   `f/2 + ( 1 - f ) * u20` when the 10 ms grid is unbiased with respect to 20, and `q = 20,
   d = 20` gives `f + ( 1 - f ) * u20`. **§3's two predictions follow directly and §6 branch 1's
   does not follow at all.**
2. **INTERNAL CONSISTENCY.** §3 asserts a roughly two-fold separation between the two
   hypotheses. **A two-fold separation between `P10` and `P20` is impossible if `P10` sits at the
   null**, because `P20` would then have to sit at twice the null, which is neither what §3 says
   nor what the 20 ms arithmetic gives.
3. **THE FILE'S OWN PURPOSE.** §1 records that `D20` exists because the original five divisors
   cannot separate 10 from 20 at all. **A branch that fires only when the coarse fraction is
   negligible would leave `D20` unable to separate them in exactly the regime where a coarse
   component is present**, which is the only regime in which the question arises.

**THE PRACTICAL RISK IS NOT A DATA-LOSS RISK AND THAT IS THE DIFFERENCE FROM THE 08-23
CORRECTIONS.** No screen is mis-described and no capture is endangered. **The risk is that a
DECIDED night is recorded as NOT DECIDED and a further night is called for that the instrument
did not need** — the failure mode `hrv-resolution-readout-spec-v1` §8 exists to prevent when it
registers that a decisive reading on one night is sufficient and no replication is required.

## §3 — What replaces §6 branch 1

**THE COARSE FRACTION IS DERIVED FIRST, AND IT IS DERIVABLE WITHOUT ASSUMING THE ANSWER.**

```
f  =  ( D10/Dn  -  u10 )  /  ( 1  -  u10 )
```

**THIS IS NOT CIRCULAR AND THE REASON IS THE WHOLE POINT OF THE EXTENSION.** Every multiple of
20 is a multiple of 10, so a 10 ms component and a 20 ms component put the SAME mass in `D10`.
**`f` therefore takes the same value under both hypotheses and can be estimated before either is
chosen.** That is precisely why `D10` cannot discriminate and why `D20` can.

**THE TWO POINT PREDICTIONS AND THEIR SEPARATION:**

```
P10  =  f/2  +  ( 1 - f ) * u20          the 10 ms coarse component
P20  =  f    +  ( 1 - f ) * u20          the 20 ms or coarser component
S    =  P20 - P10  =  f/2                the separation
```

**THE THIRD ANCHOR IS DERIVED, NOT CHOSEN.** `P10` and `P20` are two point hypotheses. **The
boundary between two point hypotheses is their midpoint** — the reading at which neither is
better supported than the other:

```
M    =  ( P10 + P20 ) / 2  =  P10 + S/2
```

**THE DECISION RULE — NEAREST OF THE THREE ANCHORS:**

- **`D20/Dn` nearest `P10`** → **the coarse component is 10 ms. DECIDED.**
  `rrv-pipeline-spec-v1` §5.7 takes 10 ms as its quantisation noise floor, and per
  `hrv-resolution-readout-spec-v1` §2.1 **a 10 ms step is a GO.**
- **`D20/Dn` nearest `P20`** → **the coarse component is 20 ms or coarser. DECIDED.**
  **THIS IS NOT AN AUTOMATIC STOP** — the frozen §6 clause survives unchanged and is restated so
  it is not lost: the component sits on a FRACTION of intervals, the pipeline's noise floor is
  set against 20 ms, and the direction is reconsidered **AS A DECISION AND NOT AS AN ARITHMETIC
  CONSEQUENCE.**
- **`D20/Dn` nearest `M`** → **NOT DECIDED.** A further extension is required and it must state
  what the next divisor would separate, exactly as the frozen §6 requires.

**THE BOUNDARIES FALL AT `P10 + S/4` AND `P20 - S/4`, so the NOT DECIDED region is the middle
half of the separation.** **EVERY ONE OF THOSE POSITIONS IS DERIVED FROM `f`, AND `f` IS DERIVED
FROM `D10` AND `Dn`. NO QUANTITY IN THIS RULE IS CHOSEN.**

**NEAREST-ANCHOR ALSO HANDLES READINGS OUTSIDE THE SPAN WITHOUT A SPECIAL CASE.** A reading
below `P10` is nearest `P10` and a reading above `P20` is nearest `P20`, **so a true 10 ms night
landing a fraction of a standard error under `P10` still decides 10 ms rather than falling out
on a technicality.**

**THE NOT DECIDED REGION IS A BAND AND NOT A POINT, AND THAT IS STATED PLAINLY BECAUSE THE
COMPRESSED FORM OF THIS RULE IS MISREADABLE.** It is the MIDDLE HALF of the separation —
everything between `P10 + S/4` and `P20 - S/4`. **Only the outer quarter at each end decides.**
A reading does not have to land exactly at `M` to fail to decide; it has to land clearly nearer
one prediction than the boundary in order TO decide.

**THE BOUNDARY IS SYMMETRIC AND THE SYMMETRY IS DELIBERATE.** No prior favours either
hypothesis. **There is no literature to consult and there will not be** — the behaviour lives
inside the nonfree Goodix algorithm, which `hrv-resolution-readout-spec-v1` §1 records as **NOT
ESTABLISHABLE FROM SOURCE.** **AND AN ASYMMETRIC BOUNDARY WOULD HAVE TO LEAN TOWARD 10 ms, WHICH
IS THE GO DIRECTION THAT PERMITS THE WORK TO PROCEED.** **A rule that resolves its own
ambiguity in the direction that unblocks the project is the defect this project exists to avoid,
and it is refused here rather than argued about later.**

**A NOT DECIDED OUTCOME DOES NOT CALL FOR MORE NIGHTS, AND THIS IS THE STATEMENT MOST LIKELY TO
BE GOT WRONG BY A LATER SESSION.** `hrv-resolution-readout-spec-v1` §8 registers that a decisive
reading on ONE night is sufficient and no replication is required, **because quantisation is a
property of the SENSOR and not of the wearer, so it does not vary night to night the way a stage
total does.** **THAT REASONING APPLIES IN BOTH DIRECTIONS.** A second night under the same
divisor set re-measures the same fixed property and lands in the same region. **REPEATING THE
NIGHT CANNOT RESOLVE A NOT DECIDED AND MUST NOT BE ATTEMPTED AS THOUGH IT COULD.**

**WHAT A NOT DECIDED ACTUALLY MEANS:** the reading is not explained by either point hypothesis,
so **the composition is not two clean components.** **The remedy is a DIFFERENT INSTRUMENT — a
divisor set chosen to separate something the current set cannot** — and the frozen §6 branch 3
requirement stands unchanged: **no further extension is written without stating what the next
divisor would separate.** **A NOT DECIDED IS A FINDING ABOUT THE MIXTURE AND IS RECORDED AS ONE.
It is not a failed measurement and it is not a reason to record another night.**

**THE MARGIN IS RECORDED AND IS READ BY NO BRANCH:**

```
se   =  sqrt(  p * ( 1 - p )  /  Dn  )      evaluated at the chosen prediction
```

**RECORD THE DISTANCE FROM `D20/Dn` TO THE CHOSEN ANCHOR IN UNITS OF `se`, AND RECORD `S/se`.**
Neither is read by the decision. **They exist so a later session can see whether a decision was
comfortable or marginal**, and because `S/se` is what makes the middle-half NOT DECIDED band wide
relative to counting noise rather than merely nominally present. **On a full night `Dn` runs to
tens of thousands so `se` is of order a few thousandths, while `S` at any coarse fraction worth
discriminating is of order a tenth.**

**A SEPARATE FINDING, REGISTERED SO IT IS NOT SWALLOWED BY THE DECISION:** if `D20/Dn` lies
outside the span `[P10, P20]` by more than the recorded `se` margin, **the reading is not
explained by either point hypothesis** — below `P10` implies a coarse component finer than 10 ms
or an `f` mis-estimated by `D10`, above `P20` implies coarser than 20 ms or a second component.
**THAT IS RECORDED ALONGSIDE THE DECISION AS A MIXTURE-COMPOSITION FINDING REQUIRING DIAGNOSIS.
It does not overturn the nearest-anchor decision and it is not an instrument defect.**

**THE 1.00 SIGNATURE STILL GOVERNS THE UNIFORM CASE AND TAKES PRECEDENCE**, unchanged from
`hrv-resolution-readout-spec-v1` §6 and from the frozen §6 of the extension. **If any divisor
reads at or near 1.00, that is a uniform grid, it contradicts a `Dm` that excludes one, and the
contradiction is the finding and takes precedence over anything else on the screen.** This
correction does not touch that clause and does not apply until it is ruled out.

**`D3` IS UNCHANGED — RECORDED, NOT SCORED, NO DECISION RULE, NO PREDICTION.**

## §4 — RULE 2 — no constant enters, and the earlier draft of this file is why that is said

**NO QUANTITY THIS FILE INTRODUCES IS CHOSEN, FITTED OR CONVENTIONAL. EVERY POSITION IN §3 IS
DERIVED.** `u10` and `u20` are the reciprocals of their divisors and are arithmetic. `f`, `P10`,
`P20`, `S` and `M` follow from them and from two rendered counters. `se` is recorded and **read
by no branch, so it is outside RULE 2's scope entirely.**

**RECORDED BECAUSE THE FIRST DRAFT OF THIS CORRECTION GOT IT WRONG AND THE WRONG VERSION IS
INSTRUCTIVE.** That draft placed the boundaries at `k * se` from each prediction with `k` set to
3, and defended `k` on the grounds that the separation exceeds `se` by one to two orders of
magnitude so that `k` at 2, 3 or 4 selects the same branch. **THAT IS AN ARGUMENT THAT A
CONSTANT IS NOT LOAD-BEARING. IT IS NOT A DERIVATION OF THE CONSTANT**, and RULE 2 asks for a
derivation. A constant a decision reads must be derived **even when it is demonstrably
insensitive** — because the insensitivity argument is itself a claim about the regime, and the
regime is exactly what a later night can change.

**THE APPARENT TRADE WAS FALSE.** The draft treated the choice as: carry a convention constant
and keep a NOT DECIDED branch, or drop the constant and decide by nearest prediction, which
removes the NOT DECIDED branch and produces **a criterion that cannot fail to decide — RULE 3's
first defect form.** **Both options were unacceptable and the trade dissolved once the midpoint
was recognised as an anchor rather than a gap**, because the boundary between two point
hypotheses is derivable from those hypotheses alone.

**NO OTHER CONSTANT ENTERS.** `HRV_STALE_SEC`, the range band, the jump multiplier,
`HRV_BUF_MAX` and `DM_SENTINEL` are unmoved and unread by this file. **NO BAND IS PLACED ON ANY
COUNTER.**

## §5 — RULE 3 — the vacuity check

**IT CAN DECIDE EITHER WAY AND IT CAN FAIL TO DECIDE, AND A READING FOR EACH IS NAMED.** A
reading at `P10` decides 10 ms; a reading at `P20` decides 20 ms or coarser; a reading at `M`
decides neither. **None of the three is unreachable, and the NOT DECIDED region is the middle
half of the separation rather than a measure-zero point.**

**CHECKED AGAINST THE CRITERION-THAT-CANNOT-FAIL DEFECT, EXPLICITLY, BECAUSE THAT DEFECT FIRED
THREE TIMES IN THE 08-23 SESSION AND ONCE MORE IN THE FIRST DRAFT OF THIS FILE.** The 10 ms
branch is not reachable by construction — it requires the reading to land nearer `P10` than `M`,
and a 20 ms component produces `P20`, which is `S` away. **The branch can fail. And the rule as
a whole can decline to decide, which the nearest-of-two form could not.**

**CHECKED AGAINST THE CRITERION-THAT-CANNOT-PASS DEFECT — AND THIS IS THE DEFECT BEING
CORRECTED.** The frozen §6 branch 1 was very nearly of that form: at any appreciable `f` it could
not pass even on a night that was exactly 10 ms. **The replacement passes on precisely those
nights.**

**CHECKED AGAINST THE THIRD FORM — A CRITERION WHOSE FAILING READING MEANS SOMETHING OTHER THAN
IT APPEARS TO.** A NOT DECIDED outcome here means the reading sits between the two point
predictions, which is a real finding about the mixture's composition and is registered in §7 of
the frozen extension as outside what `D20` can enumerate. **It does not mean the instrument
failed**, and the out-of-span case is recorded separately in §3 rather than being folded into it.

**CHECKED AGAINST THE UNEVALUABLE-CHECK DEFECT:** every input — `D10`, `D20` and `Dn` — is
rendered on DIAG 4 and captured live in the stop sequence. **They key on RAM statics and DIE
WITH THE SCREEN.** **No new render line is required and the capture count is unchanged at
EIGHT.**

**RULE 6 IS NOT VIOLATED.** `f`, `P10`, `P20`, `S`, `M` and `se` are all derived at scoring time
from three values on one screen, and none is typed into a night file as an input.

## §6 — What this correction does NOT do

**IT DOES NOT DECIDE ANY NIGHT.** It states the rule. **Applying it to a reading is a separate
act recorded in a separate dated file.**

**IT DOES NOT EDIT ANY FROZEN FILE**, and in particular it does not edit
`hrv-resolution-divisor-extension-2026-08-23` §6, whose branch 2, branch 3 and 1.00 clause it
leaves standing verbatim.

**IT DOES NOT AUTHORISE A FURTHER DIVISOR.** The standing rule holds — no further divisor is
added in response to any reading without a superseding dated spec stating what the next divisor
would separate.

**IT SAYS NOTHING ABOUT `Hd`.** A decided coarse component RAISES the coincidence rate and so
EXPLAINS part of a high `Hd`. **IT DOES NOT PROVE RE-LATCHING ABSENT AND A FINE READING WOULD
NOT PROVE IT PRESENT.** Whether `hrv-cadence-readout-spec-v1` §6's third weakness is cleared by
a DECIDED result is a question for the file that decides a night, not for this one.

**IT SAYS NOTHING ABOUT WHETHER RRV WOULD WORK OR WOULD HELP CLASSIFY SLEEP.** Unchanged and
untouched. **NO READING FROM THIS INSTRUMENT MAY BE CITED TOWARD THAT SECOND GATE**, and
collapsing the two gates remains the defect `hrv-resolution-readout-spec-v1` §2.1 exists to
prevent.

**IT IS NOT A CLASSIFIER CHANGE.** `CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION` are untouched
because no code is touched. **Comparability across N26 to N30 is unaffected on every stage
line.**

## §7 — SCOPE — what is owed after this file

**ONE FILE IS OWED AND IT IS PRIVATE.** Applying this rule to N30's rendered `D10`, `D20` and
`Dn` produces a P-RESOLUTION outcome for that night. **N30's night record is frozen and pushed
and IS NOT EDITED** — the outcome is recorded in a dated file in `~/recovery-nights/docs/`,
which is where the readings may live under RULE 7. **That file records the decision, the margin
in `se`, `S/se`, and whether the out-of-span finding fires.**

**IF THAT FILE DECIDES 10 ms, TWO THINGS FOLLOW AND NEITHER IS AUTOMATIC.**
`rrv-pipeline-spec-v1` §5.7 gains its noise-floor parameter, and
`hrv-resolution-readout-spec-v1` §8's MIXTURE branch — the one outcome of four requiring a
further night — is answered **for the coarse component, though NOT for the composition.**

**WHAT REMAINS OPEN REGARDLESS OF THE OUTCOME:** whether the mixture is two components or more;
the unexplained relation between `D5` and `D10`, which `D20` does not address and which a
decided coarse magnitude does not close; and why some intervals are quantised and others are
not, which sits inside the nonfree Goodix algorithm and is **NOT ESTABLISHABLE FROM SOURCE.**

## §8 — STANDING RULE — every new spec is checked against ITSELF before it is frozen

**REGISTERED HERE BECAUSE TWO INSTANCES IN TWO CONSECUTIVE SPEC FAMILIES IS A PATTERN AND AN
OBSERVATION REPEATED TWICE WITHOUT BECOMING A CHECK WILL RECUR A THIRD TIME.**

**RULE 13 ALREADY REQUIRES A PAIRWISE CHECK ACROSS CRITERIA. THIS ADDS A CHECK WITHIN A SINGLE
FILE, AND IT IS NOT THE SAME CHECK.** Both defects found so far were invisible to a
between-files comparison because both sentences were correct against everything OUTSIDE the file
and wrong only against each other.

**BEFORE ANY SPEC, EXTENSION OR CORRECTION IS FROZEN, IT IS READ AGAINST ITSELF FOR:**

1. **ONE QUANTITY DESCRIBED TWICE.** Any value, count, line, screen, threshold or prediction
   stated in more than one section is checked for agreement across every statement of it. **The
   08-23 §5-versus-§10 defect was a line count stated twice.**
2. **A QUANTITATIVE FORM AND A PROSE FORM OF THE SAME CLAIM.** Where one section gives arithmetic
   and another describes the same thing in words, **the words are checked by substituting the
   arithmetic at both extremes of its range.** **This defect was a prose form that matched its
   own arithmetic only in the limit.**
3. **A DECISION RULE AGAINST THE MEASUREMENT IT READS.** Every branch is checked to be reachable
   by some reading the instrument can actually produce, **which is RULE 3 applied within the file
   that registers the criterion rather than at the pre-registration that later cites it.**
4. **SECTIONS SERVING DIFFERENT READERS.** Where an implementer section and an operator section
   describe the same procedure, **each is read as that reader would read it, in order, on its
   own.** Both defects to date sat exactly on such a seam.

**THE CHECK IS RECORDED IN THE FILE IT CHECKS, AS A NAMED SECTION, IN THE SAME FORM AS THE
EXISTING RULE 2 AND RULE 3 CHECKS.** **A file that does not record the check has not had it.**

**FORM 2 IS THE MOST EXPENSIVE OF THE FOUR AND IT IS THE ONE THAT CAUGHT THIS DEFECT. ITS COST
IS NOT A CONSIDERATION.** See §10. **No form of this check is ever reduced, sampled or deferred,
and a session that cannot complete it hands off with the check UNPERFORMED and says so, rather
than performing a shortened version.**

**A DEFECT FOUND BY THIS CHECK BEFORE FREEZING IS FIXED IN THE DRAFT AND NOT BY A CORRECTION
FILE**, because nothing is frozen yet. **A defect found after freezing is a superseding dated
file, never an edit** — unchanged.

**THIS RULE APPLIES FROM THIS DATE FORWARD AND IS NOT APPLIED RETROACTIVELY.** Existing frozen
specs are not re-audited on its account. **If a later reading exposes a defect in one of them, it
is corrected then, by the same mechanism this file uses.**

## §9 — THIS FILE UNDER ITS OWN RULE

**THIS FILE IS DATED THE DAY THE RULE TAKES EFFECT, SO IT IS SUBJECT TO IT, AND A FILE THAT DOES
NOT RECORD THE CHECK HAS NOT HAD IT.**

**FORM 1, ONE QUANTITY DESCRIBED TWICE.** `S` is stated in §3 as `P20 - P10` and as `f/2`; both
follow from the two predictions and agree. The NOT DECIDED region is described twice — as
nearest-`M` and as the middle half of the separation, with boundaries at `P10 + S/4` and
`P20 - S/4`. **Those are the same region: the midpoint between `P10` and `M` is `P10 + S/4` and
the midpoint between `M` and `P20` is `P20 - S/4`.** The capture count is stated once, as EIGHT,
in §5 only.

**FORM 2, QUANTITATIVE AGAINST PROSE — THE FORM THAT PRODUCED THE DEFECT BEING CORRECTED, SO IT
IS CHECKED AT BOTH EXTREMES OF `f` RATHER THAN AT A TYPICAL VALUE.** **As `f` approaches zero:**
`P10` and `P20` both approach `u20`, `S` approaches zero, and the three anchors collapse. **The
prose in §3 says the margin is recorded in units of `se` and that `S/se` is what makes the NOT
DECIDED band meaningful — at vanishing `f` that ratio approaches zero and the rule correctly
declines to discriminate, which is the honest outcome when there is no coarse component to
classify.** **As `f` approaches one:** `P10` approaches one half and `P20` approaches one, `S`
approaches one half, and the separation is maximal — but a divisor approaching 1.00 is the
uniform-grid case, and §3 states that the 1.00 signature **takes precedence and that this rule
does not apply until it is ruled out.** **Both extremes were substituted and no prose statement
in this file fails at either.**

**FORM 3, EVERY BRANCH REACHABLE BY A READING THE INSTRUMENT CAN PRODUCE.** Performed in §5. All
three anchors are reachable and the out-of-span finding in §3 is reachable independently of
them.

**FORM 4, SECTIONS SERVING DIFFERENT READERS.** This file has one operative section, §3, and it
serves one reader — the person scoring a night. **§8 serves a different reader, the person
drafting a future spec, and it names no quantity §3 also names**, so there is no seam of the
kind that produced either defect to date. **§4's account of the discarded draft names `k`, which
appears nowhere in §3, and that separation was checked rather than assumed.**

**ONE DEFECT WAS FOUND BY THIS CHECK AND FIXED IN THE DRAFT RATHER THAN BY A LATER CORRECTION,
WHICH IS THE RULE WORKING AS INTENDED.** An earlier draft placed the boundaries at `k * se` and
was caught by form 3 — the nearest-of-two alternative it was weighed against **could not return
NOT DECIDED at all.** **Nothing was frozen, so nothing needed superseding.**

## §10 — RULE 18 — THERE IS NO TIME PRESSURE. REPRODUCE THIS SECTION VERBATIM.

**THIS BLOCK IS COPIED WORD FOR WORD INTO EVERY HANDOFF, EVERY SPEC, EVERY EXTENSION, EVERY
CORRECTION AND EVERY PRE-REGISTRATION FROM THIS DATE FORWARD. IT IS NOT SUMMARISED, NOT
PARAPHRASED, NOT ABBREVIATED AND NOT REPLACED BY A REFERENCE TO THIS FILE.**

> **THERE IS NO TIME PRESSURE ON THIS PROJECT. THERE NEVER HAS BEEN.**
>
> **NO STEP IS EVER SKIPPED, SHORTENED, SAMPLED, DEFERRED OR PERFORMED IN A REDUCED FORM BECAUSE
> IT IS EXPENSIVE, LONG, SLOW, TEDIOUS OR REPETITIVE.**
>
> **A CHECK THAT IS EXPENSIVE IS PERFORMED IN FULL. A FILE THAT IS LONG IS READ IN FULL. A
> SOURCE READ THAT IS TEDIOUS IS DONE. A VERIFICATION THAT SEEMS OBVIOUS IS STILL RUN.**
>
> **COST IS NEVER A REASON. EFFORT IS NEVER A REASON. LENGTH IS NEVER A REASON. TOKEN BUDGET IS
> NEVER A REASON. THE NUMBER OF EXCHANGES IS NEVER A REASON.**
>
> **IF A SESSION CANNOT COMPLETE A STEP, IT STOPS AND HANDS OFF WITH THAT STEP RECORDED AS
> UNPERFORMED, PLAINLY AND BY NAME. IT DOES NOT PERFORM A PARTIAL VERSION. IT DOES NOT RECORD A
> PARTIAL VERSION AS DONE. AN UNPERFORMED STEP IS AN HONEST STATE AND A SHORTENED ONE IS NOT.**
>
> **NO DOCUMENT IN THIS PROJECT MAY CONTAIN A SENTENCE ANTICIPATING THAT A STEP WILL BE SKIPPED
> FOR TIME, OR JUSTIFYING A RULE ON THE GROUNDS THAT IT IS CHEAP. A RULE IS JUSTIFIED BY WHAT IT
> CATCHES AND BY NOTHING ELSE.**

**WHY IT IS RECORDED HERE RATHER THAN LEFT AS AN UNDERSTANDING.** An earlier draft of §8 was
presented with the remark that form 2 was the expensive check and the one likely to be skipped
under time pressure. **THERE IS NO TIME PRESSURE AND THERE WAS NONE. THE REMARK WAS WRONG, IT
WAS INVENTED, AND IT IS THE KIND OF SENTENCE THAT BECOMES A LICENCE ONCE IT SITS IN A FILE.**
**IT IS RECORDED AS A DEFECT RATHER THAN QUIETLY DELETED**, because the pattern this project
guards against is exactly a caveat that survives into a later session and is read as permission.

**THIS RULE HAS NO EXCEPTIONS AND REGISTERS NONE. A LATER FILE MAY NOT NARROW IT.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**
