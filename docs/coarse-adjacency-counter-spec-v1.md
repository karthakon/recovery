# coarse-adjacency-counter-spec-v1

**Dated 2026-08-25. Frozen BEFORE the counter is built and BEFORE any reading exists.**

**FILED IN `~/recovery/docs/` — THE PUBLIC REPO. RULE 7 CHECK PERFORMED IN §12: NO SUBJECT
PHYSIOLOGICAL VALUE APPEARS IN THIS FILE, AND THE NUMERIC PREDICTIONS DERIVED FROM THE MEASURED
`f` ARE DELIBERATELY LEFT IN THE PRIVATE FILE THAT DERIVED THEM.**

**IT SPECIFIES ONE COUNTER AND ONE RENDER LINE. IT CHANGES NO CLASSIFIER PATH, NO PARAMETER, NO
BAND AND NO STAGE DECISION. IT DECIDES NO NIGHT.**

**IT IS OWED BY `hrv-coincidence-null-mixture-rederivation-2026-08-24` (`f53ea21`) §8, WHICH NAMES
THE INSTRUMENT PRECISELY AND DELIBERATELY DOES NOT SPECIFY IT.** This file is that specification
and nothing more. **IT DOES NOT ABSORB `D10r`**, the rejected-interval divisor counter owed by the
same file's weakness 6, **which remains owed as its own dated spec** — the reasoning
`hrv-resolution-readout-spec-v1` §11 gives for not folding in `rej_quality`.

**THAT §11 IS ITSELF CORRECTED BY `resolution-spec-s11-correction-2026-08-23`, AND THE CORRECTION
IS NOTED SO §11 IS NOT TAKEN WHOLESALE.** What the correction withdraws is §11's PREMISE that the
`rej_quality` path can fire — every call site passes the literal `1` and the SDK exposes no
quality accessor, so the path is dead by API. **WHAT SURVIVES UNTOUCHED IS THE REASONING THIS FILE
BORROWS: A SPEC WRITTEN TO ANSWER ONE THING STAYS ANSWERABLE BY NOT ABSORBING A SECOND.** **THE
CORRECTION STRENGTHENS THAT REASONING RATHER THAN WEAKENING IT** — §11's owed instrument turned
out to be one whose only possible reading was a criterion that cannot fail, **which is exactly
what folding it in would have buried.**

---

## §1 — THERE IS NO TIME PRESSURE. REPRODUCED VERBATIM PER RULE 18.

**Frozen in `resolution-extension-s6-correction-2026-08-24.md` §10 (`6f7ccb3`). It is copied word
for word into every handoff, spec, extension, correction and pre-registration from 2026-08-24
forward. IT IS NOT SUMMARISED, NOT PARAPHRASED, NOT ABBREVIATED AND NOT REPLACED BY A
REFERENCE.**

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

**IT HAS NO EXCEPTIONS AND REGISTERS NONE. A LATER FILE MAY NOT NARROW IT.**

## §2 — RULE 21 — IF A READ CAN SETTLE A QUESTION, THE READ IS PERFORMED. REPRODUCED VERBATIM.

**Frozen in `construction-sweep-2026-08-24.md` §RULE 21 (`608fd85`). It is copied word for word
into every handoff, spec, extension, correction and pre-registration from 2026-08-24 forward,
alongside RULE 18. IT IS NOT SUMMARISED, NOT PARAPHRASED, NOT ABBREVIATED AND NOT REPLACED BY A
REFERENCE.**

> **RULE 21 — IF A READ CAN SETTLE A QUESTION, THE READ IS PERFORMED. ALWAYS. EVERY TIME. NO
> EXCEPTIONS.**
>
> **THE MOMENT A SESSION KNOWS A READ WOULD SETTLE A QUESTION, THAT READ IS RUN BEFORE ANYTHING
> FURTHER IS WRITTEN, DECIDED, CONCLUDED, DRAFTED OR ASKED.**
>
> **IT DOES NOT MATTER WHAT THE SOURCE IS. PROJECT SOURCE, SDK HEADERS, PLATFORM DOCUMENTATION,
> SPEC FILES, NIGHT RECORDS, COMMIT MESSAGES, MAN PAGES, THIRD-PARTY SOURCE — IF IT CAN BE READ
> AND IT WOULD SETTLE THE QUESTION, IT IS READ. THE CATEGORY OF THE SOURCE IS NEVER A REASON.
> "THAT IS DOCUMENTATION RATHER THAN SOURCE" IS NEVER A REASON. "THAT IS OUTSIDE THE PROJECT
> TREE" IS NEVER A REASON.**
>
> **WHETHER TO PERFORM A READ IS NOT A DECISION AND IS NEVER PRESENTED AS ONE. A SESSION DOES NOT
> OFFER THE READ AS AN OPTION, DOES NOT ASK PERMISSION FOR IT, DOES NOT WEIGH IT AGAINST STOPPING,
> AND DOES NOT OFFER `NOT ESTABLISHED` AS AN ALTERNATIVE TO PERFORMING IT. ASKING WHETHER TO READ
> IS ITSELF A VIOLATION OF THIS RULE.**
>
> **`NOT ESTABLISHED` IS FOR WHAT CANNOT BE READ. IT IS NEVER A SUBSTITUTE FOR WHAT CAN BE.**
>
> **NO WORK CONTINUES PAST A KNOWN-NEEDED READ. NOT THE REST OF A PARAGRAPH, NOT THE REST OF A
> DOCUMENT, NOT THE REST OF A VERDICT. THE READ COMES FIRST AND THE WORK RESUMES AFTER IT.**
>
> **A READ IS NEVER QUEUED, NEVER DEFERRED, NEVER SCHEDULED FOR A LATER SESSION AND NEVER LEFT AS
> AN ITEM. PLACING A READABLE QUESTION IN A DIAGNOSTIC QUEUE IS THE SAME FAILURE AS SKIPPING IT.
> AN ITEM THAT SAYS "ONE COMMAND AND STILL NOT PERFORMED" IS A RULE 21 VIOLATION STANDING IN THE
> RECORD.**
>
> **A VERDICT REACHED WITH AN UNREAD PREMISE IS WITHDRAWN, NOT AMENDED — AND THE PREMISE IS READ
> BEFORE THE REPLACEMENT VERDICT IS WRITTEN.**
>
> **THERE ARE NO EXCEPTIONS AND NONE MAY EVER BE REGISTERED. RULE 18 SUPPLIES NO EXCUSE AND
> NEITHER DOES ANYTHING ELSE. A LATER FILE MAY NOT NARROW THIS RULE.**

## §3 — What question this instrument exists to answer

**`f53ea21` §3 DERIVES THE DUPLICATE-COINCIDENCE NULL AS `phi0 * (1 + 9c)`**, where `c` is the
probability that BOTH members of a consecutive accepted pair are coarse — that is, both multiples
of the 10 ms grid `N30-resolution-decision-2026-08-24` (`0efe368`) decided.

**`c` IS NOT `f`, AND `c` IS NOT MEASURED BY ANY INSTRUMENT THAT EXISTS.** `f53ea21` §3 bounds it
and cannot narrow it:

- **INDEPENDENT ASSIGNMENT BETWEEN CONSECUTIVE INTERVALS GIVES `c = f * f`. LOWER BOUND.**
- **FULLY CLUSTERED QUANTISATION GIVES `c` APPROACHING `f`. UPPER BOUND.**
- **NOTHING IN THIS PROJECT HAS MEASURED WHICH.** The mechanism lives inside the nonfree Goodix
  algorithm, which `hrv-resolution-readout-spec-v1` §1 records as **NOT ESTABLISHABLE FROM
  SOURCE**, so it must be measured rather than reasoned about.

**`f53ea21` §6 NAMES THIS AS WEAKNESS 5 AND AS THE DOMINANT UNCERTAINTY, "NOT REDUCIBLE BY
ARGUMENT".** The bounds differ by more than a factor of two. **THE NULL IS THEREFORE A RANGE AND
NOT A NUMBER, AND `Hd` IS UNINTERPRETABLE UNTIL IT IS A NUMBER.**

**THIS COUNTER COLLAPSES THE RANGE. THAT IS ITS ENTIRE PURPOSE AND IT HAS NO OTHER.**

## §4 — The counter, specified exactly

**ONE COUNTER, `s_dc`, RENDERED AS `Dc`.**

**IT COUNTS CONSECUTIVE ACCEPTED PAIRS IN WHICH BOTH INTERVALS ARE DIVISIBLE BY 10**, per
`f53ea21` §8's own words, which are the specification and are not widened here.

**POPULATION: ACCEPTED INTERVALS ONLY, NOT GAP-FILTERED.** Both properties are load-bearing and
both are registered rather than assumed:

- **ACCEPTED-ONLY, BECAUSE §8 SAYS ACCEPTED.** A siting at the `ppi > 0` guard, matching `Hd`'s
  own population, was proposed in an earlier session and **is recorded as wrong so it is not
  revived.**
- **NOT GAP-FILTERED.** Excluding gap-spanning pairs is standard for **physiological**
  beat-to-beat measures like rMSSD, because a delta across a gap is not a real beat-to-beat delta.
  **`c` IS QUANTISATION ARITHMETIC AND NOT A DELTA**, so the physiological framing does not
  transfer. **A GAP-EXCLUDED SITING WAS ALSO PROPOSED IN AN EARLIER SESSION AND IS ALSO RECORDED
  AS WRONG.**

**SITE, ESTABLISHED FROM A FULL READ OF `main.c:440-510` ON 2026-08-25:** inside the existing
`if (acc)` block, using `s_dm_prev` while it still holds the previous accepted interval, and
therefore **STRICTLY BEFORE `s_dm_prev = ppi;`.**

**THE INCREMENT:**

```
// coarse-adjacency-counter-spec-v1 s4: consecutive ACCEPTED pairs in which
// BOTH intervals are divisible by 10. Same site, same scope, same increment
// path as the divisors. Uses s_dm_prev BEFORE it is overwritten below.
if (s_dm_prev > 0 && (ppi % 10 == 0) && (s_dm_prev % 10 == 0)) s_dc++;
```

**NO NEW STATE IS INTRODUCED BEYOND THE COUNTER ITSELF.** `s_dm_prev` already exists, is already
maintained at this site, and is not moved, renamed or repurposed. **`hrv_buf_add` IS NOT TOUCHED.
NO DIVISOR IS ADDED OR MOVED. `DM_SENTINEL` IS NOT TOUCHED.**

**DECLARATION** alongside the divisor statics: `static uint32_t s_dc = 0;`

**RESET AT SESSION START**, in the existing reset block, on its own line with this spec named:
`s_dc = 0;`. **READ FROM SOURCE THAT THIS BLOCK IS WHERE `s_dm_prev` IS RESET TO 0** — which is
what makes the `s_dm_prev > 0` test a genuine first-interval-of-session test and not an
app-lifetime one. **THAT PROPERTY IS THE ONE P6 LACKED**, per
`handoff-v69.md` lines 185-205, and it is verified here rather than assumed.

## §5 — THE DENOMINATOR IS DERIVED AND IS DELIBERATELY NOT COUNTED

**THIS SECTION EXISTS BECAUSE THE OBVIOUS DESIGN IS WRONG.**

**A SECOND COUNTER OVER EVALUATED PAIRS IS NOT ADDED.** The pair test runs for exactly every
accepted interval after the first of the session, because `s_dm_prev` is 0 only before the first
accepted interval and `ppi > 0` is guaranteed by the enclosing guard. **SO THE NUMBER OF PAIRS
EVALUATED IS `Dn - 1` BY CONSTRUCTION.**

**WHAT RULE 20 ACTUALLY SAYS IS QUOTED HERE VERBATIM FROM ITS FROZEN FILE,
`hrv-event-per-beat-corroboration-withdrawal-2026-08-24` (`28e0452`) §10, READ IN FULL ON
2026-08-25. AN EARLIER DRAFT OF THIS FILE PARAPHRASED IT FROM A HANDOFF SUMMARY AND GOT IT WRONG,
AND THE OVERSTATEMENT IS RECORDED IN §11 RATHER THAN QUIETLY REPAIRED:**

> before any exact agreement between two counters is reported as a finding, **the increment sites
> of both are read and the agreement is shown to be breakable by some reachable state.** **AN
> AGREEMENT THAT CANNOT BREAK IS RECORDED AS A CONSTRUCTION AND NEVER AS A CORROBORATION.**

**IT GOVERNS REPORTING. IT DOES NOT FORBID BUILDING THE COUNTER.** The draft claimed it did. **A
COUNTER PAIRED WITH `Dn` WOULD AGREE WITH `Dn - 1` ON EVERY RUNTIME STATE REACHABLE AT A FIXED
PIN**, so its agreement could never be a finding — but that alone is not the reason it is
declined.

**THE REASON IT IS DECLINED IS THAT A STRICTLY STRONGER CHECK IS AVAILABLE FROM VALUES ALREADY
RENDERED, AT NO CODE COST. SEE §5.2.** A counter would verify only that the site was reached; the
check in §5.2 constrains the value itself, and does so with bounds that are the physical extremes
the instrument exists to distinguish.

**RULE 20's FIRST CLAUSE IS SATISFIED FOR `P-DCIDENT` AND IS NOT ASSUMED.** The increment sites of
all three counters it reads — `Dc`, `D10` and `Dn` — were read from `main.c:486-501` on
2026-08-25. **ALL THREE SIT AT ONE SITE INSIDE `if (acc)`**, which is what makes the bounds
arithmetic over a single well-defined sequence.

**AND THE FAULT CLASS IS REAL EVEN SO.** `handoff-v69`'s diagnostic queue records the
unguarded-increment class as **PREVENTED, NOT WATCHED**, and **P-MINS CAN NO LONGER CATCH IT AT
REALISTIC DURATIONS.** **§5.2 IS WHAT WATCHES IT FOR THIS INSTRUMENT.**

**THEREFORE: `q = Dc / (Dn - 1)`, DERIVED AT SCORING TIME FROM TWO RENDERED VALUES.** Both are on
DIAG 4, so **RULE 6 IS SATISFIED — NOTHING COMPUTED IS TYPED, AND NOTHING IS COMPUTED BY HAND
THAT IS NOT DERIVED FROM RENDERED VALUES.**

### 5.1 THE EXPOSURE THE DERIVATION CREATES, STATED BEFORE THE REMEDY

**`Dn - 1` IS EXACT ONLY IF THE PAIR TEST IS REACHED ONCE PER ACCEPTED INTERVAL.** That is true at
the specified site by construction, **and it is true only because the increment sits inside
`if (acc)` and outside no other conditional.**

**A LATER EDIT THAT MOVES EITHER INCREMENT OUT FROM UNDER THAT GUARD BREAKS THE IDENTITY WITH NO
COMPILE ERROR** — the same shape as the `SLEEP_ONSET_MINUTES` / `ONSET_RUN` / `RUNS_ONSET_RUN`
shared-literal chain that closed diagnostic queue item 44, **where nothing coupled three constants
but comments.**

**THAT EXPOSURE IS NOT ACCEPTED AS A COST. IT IS CLOSED BY §5.2.**

### 5.2 `P-DCIDENT` — THE IDENTITY CHECK, DERIVED FROM VALUES ALREADY RENDERED

**IT COSTS NO COUNTER, NO STATE AND NO CODE. IT IS SCORED AT SCORING TIME FROM `Dc`, `D10` AND
`Dn`, ALL THREE OF WHICH ARE ON DIAG 4.** It is the same family as `P-HIDENT`, `P-GIDENT` and
`P-DIDENT`, which settle their own instruments' correctness by identity rather than by trust.

**THAT FAMILY MEMBERSHIP IS READ FROM THE FILE THAT DEFINES IT AND NOT ASSUMED.**
`hrv-event-per-beat-corroboration-withdrawal-2026-08-24` §9 keeps `P-HIDENT` on the STANDS list
because **A MISPLACED COUNTER BREAKS IT**, and keeps `P-GIDENT` and `P-DIDENT` because **THE
DIVISOR CHAINS FAIL ON A MISPLACED COUNTER.** **THAT IS THE STANDARD, AND `P-DCIDENT` MEETS IT:
ITS BOUNDS HOLD FOR ANY VALID SEQUENCE, SO THEY CAN BE VIOLATED ONLY BY A MISCOUNTED `Dc`.**

**IT IS THEREFORE NOT A CONSTRUCTION IN RULE 20's SENSE.** `Dn == B` is by construction outright
and that same file puts any evidentiary use of it on the WITHDRAWN list. **`P-DCIDENT` IS NOT THAT
SHAPE**, because where `Dc` sits between its bounds is arrangement-dependent and varies with the
night.

**THE PROJECT'S OWN STANDARD FOR FREEZING AN IDENTITY IS MET AND IS NAMED RATHER THAN ASSUMED.**
`measurement-spec-v1-feature-correction` §2 carries `465f93e` process lesson 1: **AN IDENTITY MUST
BE VERIFIED AGAINST THE CODE COMPUTING BOTH SIDES BEFORE IT IS FROZEN**, and it declines to
register `RemN == Gate` for precisely that reason — the implementation did not yet exist. **THAT
FILE'S REMEDY WAS TO READ BOTH CALL SITES TOGETHER AND CONFIRM WHETHER THE SKIP RULES LAND ON THE
SAME SIDE OF BOTH COUNTS.**

**THE EQUIVALENT READ IS PERFORMED HERE AND ITS RESULT IS RECORDED:** `Dc`, `D10` and `Dn` all
increment inside the single `if (acc)` block at `main.c:486-501`, **so every gate — quality, range
and jump — lands on the same side of all three.** **THERE IS NO SKIP RULE THAT REACHES ONE AND NOT
THE OTHERS.** **THAT IS WHY THE BOUNDS ARE ARITHMETIC OVER ONE WELL-DEFINED SEQUENCE AND NOT A
COMPARISON ACROSS TWO POPULATIONS.**

**ONE DIFFERENCE FROM THAT PRECEDENT IS REGISTERED: `P-DCIDENT`'s `Dc` DOES NOT YET EXIST.** The
bounds are arithmetic and hold for any valid sequence, so they are freezable in advance — **but
the read confirming `Dc` lands inside `if (acc)` AS BUILT is owed at implementation, exactly as
§5.2's scoring-time read is owed at each pin.** **FREEZING THE BOUNDS IS NOT THE SAME AS HAVING
VERIFIED THE COUNTER, AND THIS FILE DOES NOT CLAIM THE SECOND.**

**AND `P-DCIDENT` IS WHAT MAKES §7's ONE-NIGHT SUFFICIENCY ARGUMENT AVAILABLE TO THIS INSTRUMENT
AT ALL.** `hrv-resolution-readout-spec-v1` §8 grants one-night sufficiency to the divisors because
the signature is arithmetic rather than physiological — **and it attaches a rider:** a second night
could not disconfirm the first **UNLESS THE FIRST WERE MISCOUNTED, WHICH THE §4 IDENTITIES ALREADY
CHECK.** **THE RIDER IS LOAD-BEARING AND NOT DECORATIVE.** **WITHOUT AN IDENTITY CHECK ON `Dc`,
THIS INSTRUMENT WOULD BE BORROWING A ONE-NIGHT ARGUMENT WHOSE STATED PRECONDITION IT DID NOT
MEET.** **`P-DCIDENT` IS THAT PRECONDITION.**

**THE BOUNDS ARE PURE COMBINATORICS ON A SEQUENCE OF `Dn` INTERVALS OF WHICH `D10` ARE COARSE.
THEY ARE NOT FITTED, NOT PHYSIOLOGICAL AND NOT DERIVED FROM ANY NIGHT.**

**UPPER BOUND — `Dc <= D10 - 1` WHEN `D10 >= 1`, AND `Dc = 0` WHEN `D10 = 0`.** Adjacencies are
maximised by placing every coarse interval in one contiguous run, which yields `D10 - 1` of them.
**NO ARRANGEMENT EXCEEDS IT.**

**LOWER BOUND — `Dc >= max(0, 2*D10 - Dn - 1)`.** Coarse intervals arranged into `r` runs produce
`D10 - r` adjacencies, and `r` cannot exceed the number of available slots `Dn - D10 + 1`.
**ONCE `D10` PASSES HALF THE POPULATION, ADJACENCIES ARE FORCED BY PIGEONHOLE AND CANNOT BE
AVOIDED.**

**THE TWO BOUNDS ARE THE PHYSICAL EXTREMES THE INSTRUMENT EXISTS TO DISTINGUISH.** The upper bound
IS full clustering; the lower bound IS maximal dispersion; **the independence case sits strictly
between them.** **SO `P-DCIDENT` IS NOT A BOLT-ON CHECK ON AN UNRELATED QUANTITY — IT CONSTRAINS
THE SAME QUANTITY THE READING IS ABOUT.**

**A READING OUTSIDE EITHER BOUND IS NOT A SURPRISING RESULT. IT IS A MALFORMED INSTRUMENT**, per
RULE 12, and the night is recorded **NOT PERFORMED** for `q` rather than scored. **IT MUST NOT BE
INTERPRETED AS EXTREME CLUSTERING OR EXTREME DISPERSION.**

**WHAT `P-DCIDENT` DOES NOT CATCH, NAMED SO IT IS NOT READ AS COMPLETE:** an edit that moves the
pair increment under a **NARROWER** guard reduces `Dc` and may leave it still inside the bounds.
**THE CHECK IS ONE-SIDED AGAINST THAT FAILURE AND IS NOT CLAIMED OTHERWISE.** **A `Dc` SITTING
IMPLAUSIBLY NEAR THE LOWER BOUND IS THEREFORE READ AS A REASON TO RE-READ THE SITE, NOT AS A
DISPERSION FINDING.**

**WHAT WOULD MAKE THE COVERAGE FULL, AND WHAT WOULD NOT. REGISTERED IN ADVANCE BECAUSE A PARTIAL
CHECK ACCUMULATING PASSES IS EXACTLY WHAT COMES TO LOOK LIKE A COMPLETE ONE.**

**A SECOND PASSING CHECK DOES NOT UPGRADE IT UNLESS THAT CHECK COVERS PRECISELY THE UNCOVERED
MODE AND IS INDEPENDENT OF `P-DCIDENT`'s INPUTS.** **TWO CHECKS THAT BOTH FAIL TO REACH THE
NARROWING EDIT LEAVE THE SAME HOLE, AND THEIR AGREEMENT IS A CONSTRUCTION** — the co-firing defect
`rrv-stage-discrimination-spec-v1` §5.4 registers, and the reason
`handoff-v69` forbids citing an all-four-conditioning-checks reading as evidence. **THE CLAIM MUST
REST ON THE UNION BEING DEMONSTRABLY COMPLETE, NEVER ON BOTH HAVING PASSED.**

**NO RUNTIME INSTRUMENT CAN COVER IT.** A suppressed increment leaves nothing to count, so no
counter, no bound and no night can see it. **THE MODE IS A CODE PROPERTY AND NOT A RUNTIME ONE.**

**THE ONLY THING THAT COVERS IT IS A SOURCE READ OF THE INCREMENT SITE**, per RULE 0 and RULE 21 —
the same shape as §8's requirement to read `storage.h` AFTER the edit rather than assert it
before. **TWO CONSEQUENCES FOLLOW AND NEITHER MAY BE SOFTENED:**

- **IT IS NOT EVIDENCE FROM A NIGHT AND IT NEVER ACCUMULATES.** **TEN NIGHTS OF `P-DCIDENT` PASSES
  DO NOT ADD UP TO COVERAGE OF THE MODE `P-DCIDENT` CANNOT SEE.**
- **IT MUST BE REDONE PER COMMIT THAT TOUCHES THE HRV EVENT PATH, NOT ONCE.** A read performed at
  one pin says nothing about the next.

**SO: `P-DCIDENT` PASSING, PLUS A CURRENT SOURCE READ OF THE SITE, IS FULL COVERAGE. `P-DCIDENT`
PASSING ACROSS MANY NIGHTS IS NOT, HOWEVER MANY NIGHTS THERE ARE.**

**IT ALSO SUPPLIES A FREE FLOOR ON INTERPRETATION.** If the whole interval `[max(0, 2*D10 - Dn -
1), D10 - 1]` is narrow on a given night, **THAT NIGHT CANNOT SEPARATE THE BOUNDING CASES AT ALL**,
whatever `q` reads, and it is recorded as **NOT DECIDING** rather than as a weak reading. **THAT
CONDITION IS CHECKED BEFORE `q` IS INVERTED, NOT AFTER.**

## §6 — The render

**DIAG 4 GOES FROM FIVE LINES TO SIX**, against the nine-line ceiling `f53ea21` §8 cites.

**READ FROM SOURCE: DIAG 4 CURRENTLY RENDERS `D2/D4`, `D5/D8`, `D10/Dn`, `D3/D20`, `Dm`.**

**THE NEW LINE SITS ABOVE `Dm`, SO `Dm` REMAINS THE LAST LINE**, per the divisor-extension rule
already recorded in the source at that site. It renders alone:

```
Dc %lu
```

**ZERO PRINTS `--` WHEN THE SESSION IS UNDEFINED AND NEVER `0`**, guarding on `s_session_start`
exactly as the surrounding lines do. **A DEFINED `Dc` OF 0 IS A REAL ZERO AND IS A FINDING** — no
consecutive accepted pair was ever both-coarse — and it must not be read as an undefined session.

**THE PROVENANCE OF THAT CONVENTION IS `measurement-spec-v1` §3.5, NOT §3.6 — AND THE MISCITATION
APPEARS IN TWO INDEPENDENTLY FROZEN SPECS, NOT ONE.** `hrv-resolution-readout-spec-v1` §5
(`7a2f58d`) states: *Zero prints `--` and NEVER `0`, per `measurement-spec-v1` §3.6.*
**`hrv-cadence-readout-spec-v1` §5 (`2d0cc15`) STATES THE SAME RULE WITH THE SAME WRONG SECTION
NUMBER.** **THE `main.c` COMMENT COPIED IT FAITHFULLY.** §3.6 is a display-layout section for a
different DIAG screen and contains no such rule. **§3.5 IS WHERE IT LIVES**, in these words: a
record whose version is below 2 has its new fields treated as absent and **displayed as `--`,
never as 0** — zero is a meaningful measured value and must not be confused with "not recorded".

**THIS IS RULE 4's SECOND CLAUSE — RE-DERIVE THE PREMISE, NOT ONLY THE EVIDENCE.** The convention
itself is right and has always been right; **what was never checked was the section number
attached to it**, and it propagated from spec into source and would have propagated into this file
next. **IT IS THE SAME SHAPE AS THE `rej_quality` DEFECT**, which
`resolution-spec-s11-correction-2026-08-23` records as a correct read of `hrv_math.c` whose
premise — who calls it, and with what — was never read.

**THE CHAIN STOPS AT THE SOURCE COMMENT.** `hrv-resolution-divisor-extension-2026-08-23` §5
states the convention with NO citation attached, so it did not carry the error forward.

**A DATED CORRECTION COVERING BOTH `hrv-resolution-readout-spec-v1` §5 AND
`hrv-cadence-readout-spec-v1` §5 IS OWED AND IS NOT WRITTEN HERE.** Both specs are frozen and
neither is edited. **THIS FILE IS NOT THAT CORRECTION** — a file written to specify one counter
stays answerable by not absorbing a second question, per that same spec's §11. **IT IS ENTERED IN
THE DIAGNOSTIC QUEUE.**

**WHY IT SURVIVED IS NOT A MYSTERY AND IS RECORDED RATHER THAN LEFT IMPLIED.**
`resolution-extension-s6-correction-2026-08-24` §8 registers the standing self-check **FROM THAT
DATE FORWARD AND EXPLICITLY NOT RETROACTIVELY** — existing frozen specs are not re-audited on its
account. **BOTH SPECS PREDATE IT.** That same §8 states the remedy for exactly this case: **if a
later reading exposes a defect in one of them, it is corrected then, by the same mechanism.**
**THIS READ IS THAT LATER READING.**

**AND IT WOULD NOT HAVE BEEN CAUGHT BY THE SELF-CHECK EVEN IF IT HAD APPLIED.** Both specs are
internally consistent; the wrong section number agrees with itself everywhere it appears. **IT IS
A CROSS-FILE CITATION DEFECT AND THE FOUR FORMS ARE ALL WITHIN-FILE.** **ONLY READING THE CITED
FILE REACHES IT — WHICH IS RULE 21, NOT RULE 19.**

**AND ITS SCOPE IN §3.5 IS NARROWER THAN THE USE MADE OF IT HERE, WHICH IS STATED RATHER THAN
GLOSSED.** §3.5 governs fields **ABSENT BECAUSE THE PERSISTED RECORD PREDATES THE VERSION.** The
DIAG 4 guard governs a **SESSION THAT HAS NOT RUN.** **THE UNDERLYING PRINCIPLE IS THE SAME — AN
ABSENT VALUE MUST NOT RENDER AS A MEASURED ZERO — BUT THE EXTENSION FROM ONE CASE TO THE OTHER IS
ESTABLISHED PRACTICE ON THIS SCREEN AND IS NOT LICENSED BY §3.5's OWN WORDS.** **THIS FILE FOLLOWS
THE PRACTICE AND DOES NOT CLAIM THE SPEC REQUIRES IT.**

### 6.2 THE WATCH VERIFICATION AND THE CAPTURE SEQUENCE, BOTH OWED BEFORE N32 IS RECORDED

**THE SIX-LINE FIT MUST BE VERIFIED ON THE WATCH WITH NOTHING CLIPPED AT EITHER END BEFORE N32 IS
RECORDED**, on the same standard as the four-line verification of 2026-08-22 and the five-line
verification the divisor extension required before N30. **NINE IS THE CEILING AND ONLY NINE LINES
ON DIAG 3 HAS EVER BEEN VERIFIED; SIX IS INSIDE IT AND IS STILL NOT ASSUMED.**

**THE CAPTURE SEQUENCE'S DIAG 4 LINE COUNT CHANGES AGAIN AND THAT IS THE PART THAT LOSES DATA IF
IT IS WRONG.** `hrv-resolution-readout-spec-v1` §10 step 6 read THREE, was corrected to FOUR by
`resolution-spec-s10-correction-2026-08-23`, and went to FIVE with the divisor extension.
**FROM THIS CHANGE IT READS SIX.**

**THAT CORRECTION FILE RECORDS WHY THIS MATTERS AND ITS REASONING IS CARRIED HERE RATHER THAN
CITED:** §10 is the section a session reads during the capture sequence, when the values are
RAM-only and there is no second chance. **A PERSON FOLLOWING IT AND SEEING SIX LINES AFTER BEING
TOLD TO EXPECT FIVE WILL NATURALLY READ THAT AS SOMETHING BEING WRONG WITH THE SCREEN** — at
exactly the moment when hesitating, re-navigating or exiting destroys `P-DIDENT`, `P-DCIDENT` and
`P-RESOLUTION` for the night.

**THE CAPTURE COUNT IS UNCHANGED AT EIGHT.** The new line is on a screen already captured.

**NO FRACTION AND NO PERCENTAGE IS RENDERED.** `Dc` and `Dn` are both on this screen, so `q` is
derived at scoring time.

### 6.1 A STALE COMMENT AT THE DIAG 4 HEAD IS CORRECTED BY THIS CHANGE, AND THE CORRECTION IS RECORDED HERE RATHER THAN MADE SILENTLY

**`main.c`'s DIAG 4 header comment STATES THAT ALL *SEVEN* VALUES ARE ON THIS SCREEN, AND THE
RULE 6 ARGUMENT RESTS ON THAT CLAIM.** **THERE ARE NINE**: `D2`, `D4`, `D5`, `D8`, `D10`, `Dn`,
`D3`, `D20`, `Dm`. **THE COUNT WAS NOT UPDATED WHEN THE DIVISOR EXTENSION ADDED `D3` AND `D20`.**

**THAT COMMENT BLOCK CARRIES A SECOND, INDEPENDENT ERROR, FOUND BY READING `measurement-spec-v1`
IN FULL ON 2026-08-25: IT CITES §3.6 FOR THE `--`-NEVER-`0` RULE AND THE RULE IS IN §3.5.** §6
above records the correct provenance and the scope difference. **IT IS ONE COMMENT WITH TWO
DEFECTS AND IT IS COUNTED AS ONE COMMENT.**

**IT IS WRONG IN ITS NUMBER AND CORRECT IN ITS SUBSTANCE** — every input to every derivable ratio
is in fact rendered, so the Rule 6 conclusion holds. **THAT IS THE SAME DISTINCT FAILURE MODE
`handoff-v70` §6.2 RECORDS FOR `main.c:1680-1681`: PROSE RIGHT, POINTER OR COUNT STALE.** **IT IS
THE SIXTH WRONG COMMENT IN `main.c`, NOT THE FIFTH, AND THE COUNT IS CORRECTED IN THE HANDOFF.**

**THIS CHANGE UPDATES THAT NUMBER TO TEN, BECAUSE THIS CHANGE IS WHAT MAKES IT TEN.** The edit is
comment-only, alters no behaviour, and is named here so it is not discovered later as an
unexplained diff. **NO OTHER COMMENT IN `main.c` IS EDITED BY THIS SPEC**, and the other five
wrong comments are left standing for their own dated files.

## §7 — What a reading would and would not deliver

**WHAT IT WOULD DELIVER.** `f53ea21` §8 gives the inversion:

```
q  =  0.81 * c  +  0.18 * f  +  0.01
```

**IT DOES NOT READ `c` DIRECTLY AND THAT MUST NOT BE ASSUMED** — a fine interval is divisible by
10 by chance, which is what the `0.18 f` and `0.01` terms carry. **THE EXPRESSION INVERTS FOR `c`
GIVEN `q` AND THE ALREADY-MEASURED `f`.**

**THE TWO BOUNDING CASES PREDICT SHARPLY DIFFERENT READINGS, SO ONE NIGHT SEPARATES THEM
DECISIVELY**, on the same one-night arithmetic grounds `hrv-resolution-readout-spec-v1` §8 gives
for the divisors. **THE NUMERIC PREDICTIONS, AND THE SIZE OF THE SEPARATION BETWEEN THEM, ARE
DERIVED FROM THE MEASURED `f` AND THEREFORE LIVE IN `f53ea21` §8 IN THE PRIVATE REPO. NEITHER IS
REPRODUCED HERE — NOT AS VALUES AND NOT AS A RATIO — AND BOTH MUST BE READ FROM THAT FILE AT
SCORING TIME.** **§12 RECORDS WHY THE RATIO IN PARTICULAR CANNOT APPEAR IN THIS FILE.**

**A READING COLLAPSES THE §5 RANGE OF `f53ea21` TO A NUMBER AND MAKES `Hd` INTERPRETABLE FOR THE
FIRST TIME.**

**WHAT IT WOULD NOT DELIVER, EACH NAMED SO THE READING IS NOT OVERSTATED:**

- **IT WOULD NOT ESTABLISH WHETHER RE-LATCHING OCCURS.** `f53ea21` §8's own words: if the
  resulting null still sits below the observed rate, **the excess is real but its cause is
  unexamined**; the latch behaviour is documented API and the alternative explanations have not
  been enumerated.
- **IT WOULD NOT LICENCE MOVING THE ~5 PCT `Hd` GUIDANCE LINE IN `hrv-cadence-readout-spec-v1`
  §6.** **THAT LINE DOES NOT MOVE.** `f53ea21` §7: moving a threshold in response to a measurement
  is the defect this project exists to prevent, **and a reading from this counter is a
  measurement.** **THE TEMPTATION IS AT ITS STRONGEST WHEN THE READING ARRIVES**, and it is
  registered here in advance for that reason.
- **IT WOULD NOT ADDRESS WEAKNESS 6**, the scope mismatch between `Hd/Hn` over guard-accepted
  samples and `f` over buffer-accepted intervals. **THAT IS `D10r`'s JOB AND `D10r` IS STILL
  OWED.**
- **IT WOULD NOT SETTLE WHETHER REJECTED INTERVALS ARE QUANTISED LIKE ACCEPTED ONES.** `f53ea21`
  §9: nothing has looked.
- **IT WOULD SAY NOTHING WHATEVER ABOUT RSA OR RRV.** `f53ea21` §9 is explicit and it is carried
  verbatim: `Hd` bears on the effective unique sample rate through `hrv-cadence-readout-spec-v1`
  §6.1, and **that test is computed from `Hn - Hd` directly and does not read this null at all.**
  **NO READING FROM THIS COUNTER MAY BE CITED TOWARD WHETHER RRV WOULD WORK OR WOULD HELP CLASSIFY
  SLEEP.**

## §8 — Comparability and series

**THIS IS AN INSTRUMENT-ONLY CHANGE. IT MUST BE VERIFIED AS ONE RATHER THAN ASSERTED AS ONE.**

**NO STAGE DECISION, NO STAGE MINUTE TOTAL, NO SMOOTHER INPUT, NO CLASSIFIER PATH AND NO STORED
`EpochRecord` OR `NightSummary` FIELD IS TOUCHED.** `s_dc` is written at one site, read at one
render site, and read by nothing else.

**`CLASSIFIER_SERIES` STAYS 14 AND `NIGHT_SUMMARY_VERSION` STAYS 3. BOTH ARE VERIFIED BY READING
`src/c/storage.h` AFTER THE EDIT IS APPLIED, NOT BEFORE IT AND NOT FROM MEMORY**, per
`handoff-v70` §15.

**THE PIN MOVES OFF `ebd03f2` AND NIGHTS BEFORE AND AFTER REMAIN COMPARABLE**, because nothing
read by the classifier changes. **A NIGHT RECORDED AFTER THIS COMMIT CANNOT BE A REPLICATION-RUN
NIGHT PAIRED WITH ONE BEFORE IT**, since the run rules require one pin with no code changes of any
kind, **instrument-only explicitly included.** **NO RUN IS OPEN, SO NOTHING IS BROKEN BY THAT.**

**RULE 11 — THE NEW FORMAT STRING IS VERIFIED BY RUNNING `strings` ON `pebble-app.elf` AND
FINDING THE `Dc` LABEL IN BOTH ITS DEFINED AND ITS UNDEFINED VARIANTS.** **A CLEAN BUILD DOES NOT
PROVE A STRING REACHED THE BINARY**, per `hrv-resolution-readout-spec-v1` §5, and the source is
NOT read back in place of the ELF.

## §9 — Rule 2 check

**NO CONSTANT IS FITTED AND NO BAND IS REGISTERED ON `Dc`, ON `q` OR ON `c`.**

**THE `10` IN THE DIVISIBILITY TEST IS NOT A NEW CONSTANT AND IS NOT FITTED.** It is the grid
`0efe368` decided from the divisor signature, reproduced three times, **and it is the same literal
already present at the `D10` increment at this same site.**

**THE COEFFICIENTS `0.81`, `0.18` AND `0.01` ARE NOT INTRODUCED BY THIS FILE**, are not fitted to
any night, and are not entered into any code path. **THEY LIVE IN `f53ea21` §8 AND ARE READ FROM
IT AT SCORING TIME.** **NOTHING IN THIS FILE IS READ BY THE APP EXCEPT THE INCREMENT AND THE
RENDER LINE.**

**NO CONSTANT ANYWHERE IS DERIVED FROM ONE NIGHT'S READING, BECAUSE NO READING EXISTS YET.**

## §10 — Rule 3 check

**IT CAN READ HIGH:** a `q` inverting to `c` near `f` — clustered quantisation. **PRODUCIBLE.**

**IT CAN READ LOW:** a `q` inverting to `c` near `f * f` — independent assignment. **PRODUCIBLE.**

**IT CAN READ BETWEEN THEM**, which is neither bound, **and that outcome is recorded as
INTERMEDIATE rather than forced to the nearer bound.** **THAT IS NAMED IN ADVANCE BECAUSE IT IS
THE READING MOST LIKELY TO BE ROUNDED TO A CONCLUSION.**

**FIRST VACUITY FORM — A CRITERION THAT CANNOT FAIL.** Not applicable in the usual sense, because
this is an instrument and not a criterion, **and that is itself worth stating rather than skipping
the check.** The instrument's analogue is a counter that cannot read anything but one value.
**`Dc` CAN READ ANYWHERE IN `[max(0, 2*D10 - Dn - 1), D10 - 1]` AND BOTH ENDPOINTS ARE REACHABLE
ARRANGEMENTS.** **THE RANGE IS `D10 - 1` AT THE TOP AND NOT `Dn - 1`**, per §5.2 — a draft of this
section said `Dn - 1` and was corrected by the Form 1 check in §11.

**SECOND FORM — CANNOT PASS.** `Dc` reaching values consistent with the clustered bound is
reachable, since `f53ea21` §10's own Form 2 substitution shows that at `f` approaching 1 the
whole population is coarse.

**THIRD FORM — A READING THAT MEANS SOMETHING ELSE. TWO INSTANCES ARE REGISTERED:**

1. **A `Dc` OF 0 ON A SESSION WHERE `Dn` IS ALSO NEAR 0 MEANS AN EMPTY OR NEAR-EMPTY ACCEPTED
   POPULATION, NOT AN ABSENCE OF CLUSTERING.** **`Dn` IS READ FIRST AND `q` IS NOT DERIVED WHEN
   `Dn` IS BELOW 2**, because `Dn - 1` is then 0 or the population is a single pair. **A `q`
   DERIVED FROM ONE PAIR IS RECORDED NOT PERFORMED, NOT AS A READING.**
2. **A `Dc` READING TAKEN AS EVIDENCE ABOUT `Hd`'s CAUSE RATHER THAN ABOUT THE NULL'S VALUE.**
   §7 forbids it and names why.
3. **A `Dc` OUTSIDE `P-DCIDENT`'s BOUNDS READ AS AN EXTREME CLUSTERING OR DISPERSION RESULT WHEN
   IT MEANS THE INSTRUMENT IS MALFORMED.** **§5.2 REQUIRES NOT PERFORMED, NOT A SCORE**, and
   RULE 12 governs: an unexpected result is presumed malformed until its own correctness is
   established. **THIS IS THE INSTANCE MOST LIKELY TO ARRIVE LOOKING LIKE A DISCOVERY.**

## §11 — Self-consistency check, per RULE 19 — the standing rule registered in `resolution-extension-s6-correction-2026-08-24` §8

**THE RECORD CARRIES TWO LABELS FOR THIS CHECK AND BOTH ARE NAMED HERE SO NEITHER READS AS A
DIFFERENT RULE.** `rsa-feasibility-exit-2026-08-25` calls it RULE 19;
`hrv-coincidence-null-mixture-rederivation-2026-08-24` §10 calls it *RULE 18 and §8 of the 08-24
correction*. **THEY ARE THE SAME FOUR-FORM CHECK AND ITS TEXT IS IN THAT §8.**

**PERFORMED BEFORE THE FILE IS FROZEN AND RECORDED HERE AS A NAMED SECTION. A FILE THAT DOES NOT
RECORD THE CHECK HAS NOT HAD IT.**

**FORM 1, ONE QUANTITY DESCRIBED TWICE.** The population of `Dc` is stated in §4 as accepted-only
and not gap-filtered, and again in §5 in the derivation of the denominator, and the two agree.
`Dn - 1` appears in §5 and in §10's guard and agrees in both. **The bounds on `c` are stated in §3
as `f*f` and `f` and used in §10 in that same order.**

**THIS FORM FOUND A REAL DEFECT AND IT IS RECORDED RATHER THAN SILENTLY REPAIRED.** §10 stated
that `Dc` ranges from 0 to `Dn - 1`, **while §5.2 caps it at `D10 - 1`.** `Dn - 1` is the count of
pairs EVALUATED and `D10 - 1` is the maximum that can SUCCEED, **and the draft used the first
where it meant the second.** **THAT IS RULE 9 — TWO DIFFERENT QUANTITIES CARRYING ONE LABEL — AND
§10 IS CORRECTED.**

**FORM 2, QUANTITATIVE AGAINST PROSE, SUBSTITUTED AT BOTH EXTREMES.** **At `f` approaching 0:**
almost no interval is coarse, `Dc` approaches 0, `q` approaches the `0.01` chance term, and the
inversion returns `c` near 0 — **which is the required behaviour, since no coarse component means
no adjacency to measure.** **At `f` approaching 1:** every accepted interval is coarse, every
evaluated pair is both-coarse, `Dc` approaches `Dn - 1`, `q` approaches 1, and the inversion
returns `c` near 1 — **which is the uniform-grid case, for which `f53ea21` §10 records that the
1.00 divisor signature would have fired and `Dm` would not read 1.** **THE PROSE IN §7 CLAIMS THE
TWO BOUNDING CASES ARE FAR APART AND THE SUBSTITUTION IS CONSISTENT WITH THAT.**

**FORM 3, EVERY BRANCH REACHABLE.** Checked in §10. High, low and intermediate are all producible
and the intermediate case is named rather than absorbed.

**FORM 4, SECTIONS SERVING DIFFERENT READERS — AND PAIRED BY SHARED QUANTITY RATHER THAN BY
ADJACENCY, PER THE CONSEQUENCE `rrv-stage-discrimination-spec-v1` §12 REGISTERS.** §3 serves
whoever asks why the instrument exists, §4 and §5 serve whoever builds it, §6 serves whoever reads
the screen, §7 serves whoever scores it, §8 serves whoever checks comparability.

**THE PAIRINGS ACTUALLY RUN, EACH BECAUSE THE TWO SECTIONS CONSTRAIN THE SAME QUANTITY:**

- **§4 AGAINST §5**, because both constrain what `Dc` counts: §4 fixes the site inside `if (acc)`
  and §5's `Dn - 1` identity is valid **only** at that site. **THEY AGREE, AND §5 STATES THE
  DEPENDENCE EXPLICITLY RATHER THAN RELYING ON §4 BEING READ FIRST.**
- **§5 AGAINST §6**, because both constrain Rule 6: §5 derives `q` by hand at scoring time and §6
  is what puts both inputs on the screen that makes the derivation legitimate. **THEY AGREE.**
- **§5 AGAINST §10**, because both constrain the denominator: §5 says `Dn - 1` and §10 guards
  against deriving `q` when `Dn` is below 2. **THEY AGREE, AND WITHOUT §10's GUARD §5 WOULD DIVIDE
  BY ZERO ON AN EMPTY SESSION.** **THAT PAIRING FOUND A REAL GAP IN THE DRAFT AND §10's GUARD WAS
  ADDED BY IT.**
- **§5.2 AGAINST §10**, because both constrain the reachable range of `Dc`. **THEY DID NOT AGREE
  AND §10 WAS CORRECTED** — see Form 1 above.
- **§5.2 AGAINST §7**, because both constrain what a reading is allowed to mean: §5.2 requires NOT
  DECIDING when the bounds are too narrow to separate the cases on a given night, and §7 claims
  the two bounding cases are separable on one night. **THEY AGREE ONLY BECAUSE §7's CLAIM IS ABOUT
  THE INVERSION'S PREDICTIONS AND §5.2's IS ABOUT A PARTICULAR NIGHT'S POPULATION.** **THE
  SEPARABILITY IS A PROPERTY OF THE ARITHMETIC AND THE DECIDABILITY IS A PROPERTY OF THE NIGHT,
  AND A LATER SESSION MUST NOT COLLAPSE THEM.**
- **§7 AGAINST §9**, because both constrain what may be done with a reading: §7 forbids moving the
  guidance line and §9 records that no band is registered. **THEY AGREE. THIS PAIRING WAS RUN
  DELIBERATELY BECAUSE A FILE THAT COLLAPSES A NULL RANGE TO A NUMBER IS EXACTLY THE FILE THAT
  INVITES MOVING THE LINE THAT NULL SITS UNDER** — the same pairing `f53ea21` §10 ran between its
  own §7 and §5.
- **§6.1 AGAINST §8**, because both constrain what "instrument-only" means: §6.1 authorises a
  comment edit and §8 claims the change is instrument-only. **THEY AGREE ONLY BECAUSE THE EDIT IS
  COMMENT-ONLY, AND §8's VERIFICATION IS A READ OF `storage.h` AFTER THE FACT RATHER THAN A CLAIM
  MADE HERE.**

**A DEFECT WAS FOUND IN AN EARLIER DRAFT OF THIS FILE AND IT IS RECORDED RATHER THAN QUIETLY
FIXED, BECAUSE THE WAY IT WAS FOUND IS ITSELF THE FINDING.**

**THE DEFECT:** the draft's §5 declined a second counter on the grounds that RULE 20 forbade it,
and its §11 recorded the consequence as an accepted tension — that the denominator was
**UNOBSERVABLE**. **BOTH CLAIMS WERE WRONG.** RULE 20 forbids reporting an unbreakable agreement
as a finding, not building the counter; and `Dn` is rendered, so the denominator was never
unobservable. **WHAT WAS ACTUALLY UNVERIFIED WAS THE NARROWER CLAIM THAT THE PAIR TEST RAN
EXACTLY `Dn - 1` TIMES.**

**HOW IT WAS FOUND:** by Kristopher, on reading the draft, by asking what use a derived
denominator is if it cannot be observed and whether it could be fixed. **THE QUESTION EXPOSED THAT
THE DRAFT HAD ACCEPTED AN EXPOSURE AS A COST WITHOUT FIRST ASKING WHETHER IT COULD BE CLOSED.**

**IT IS A DISTINCT CLASS FROM THE THREE ALREADY IN THE RECORD** — those are contradictions between
two sentences inside one file. **THIS ONE IS A SINGLE CLAIM THAT WAS SIMPLY NOT CHECKED AGAINST
THE RULE IT CITED**, and no Form 4 pairing would have caught it, because both sentences agreed
with each other. **THEY WERE CONSISTENTLY WRONG.**

**REGISTERED AS A CONSEQUENCE: A SELF-CONSISTENCY CHECK VERIFIES THAT A FILE AGREES WITH ITSELF
AND DOES NOT VERIFY THAT IT AGREES WITH THE RULES IT CITES.** **WHERE A FILE JUSTIFIES A DESIGN
DECISION BY NAMING A RULE, THAT RULE'S TEXT IS RE-READ AND THE JUSTIFICATION CHECKED AGAINST IT
BEFORE THE FILE IS FROZEN.** **THAT IS RULE 21 APPLIED TO A FILE'S OWN CITATIONS AND IT WAS NOT
BEING DONE.**

**THE REMAINING TENSION, WHICH IS REAL AND IS LEFT STANDING:** `P-DCIDENT` is one-sided against a
narrowing edit, per §5.2. **IT CLOSES MOST OF THE EXPOSURE AND NOT ALL OF IT, AND IT IS RECORDED
AS PARTIAL RATHER THAN AS COMPLETE.**

**NO OTHER DEFECT WAS FOUND BY THIS CHECK IN THIS FILE.**

## §12 — Rule 7 check

**NO SUBJECT PHYSIOLOGICAL VALUE APPEARS IN THIS FILE.** No RMSSD, no `f`, no `Hd`, no `Hn`, no
night-specific reading of any kind.

**THE NUMERIC PREDICTIONS FOR THE TWO BOUNDING CASES ARE DERIVED FROM THE MEASURED `f` AND ARE
THEREFORE SUBJECT-DERIVED. THEY ARE DELIBERATELY LEFT IN `f53ea21` IN THE PRIVATE REPO AND ARE
CITED BY SECTION RATHER THAN REPRODUCED**, per §7. **A LATER SESSION MUST NOT COPY THEM INTO THIS
FILE OR INTO ANY OTHER FILE IN `~/recovery`.**

**AND NEITHER MAY THE RATIO BETWEEN THEM. AN EARLIER DRAFT OF THIS FILE CARRIED IT AND THAT WAS A
RULE 7 DEFECT, RECORDED HERE RATHER THAN QUIETLY DELETED.**

**THE DEFECT:** the draft stated a fold ratio between the two bounding cases, and argued in this
section that a ratio without either endpoint reconstructs no physiological value. **THAT ARGUMENT
IS FALSE, AND THIS FILE ITSELF IS WHY.** §7 quotes the inversion `q = 0.81c + 0.18f + 0.01` —
legitimately, since those coefficients are arithmetic from a chance divisibility of one in ten and
carry no subject data. **BUT THE TWO BOUNDING CASES ARE `c = f*f` AND `c = f`, SO THE RATIO
BETWEEN THE TWO PREDICTED READINGS IS A FUNCTION OF `f` ALONE.** **QUOTING THE FORMULA AND THE
RATIO IN THE SAME PUBLIC FILE RECONSTRUCTS `f`.**

**THE RATIO IS NOT RESTATED IN THIS SECTION, AND THAT IS DELIBERATE.** A defect is normally
recorded rather than quietly deleted, per the standing practice this project follows. **HERE THE
DEFECT IS THE VALUE ITSELF, SO RECORDING IT IN THE USUAL FORM WOULD REINTRODUCE EXACTLY WHAT THE
CORRECTION REMOVES.** **THE DEFECT IS THEREFORE RECORDED BY DESCRIPTION AND THE VALUE IS LEFT IN
`f53ea21` §8.** **A LATER SESSION MUST NOT "RESTORE" IT FOR COMPLETENESS.**

**THE DRAFT ALSO DEFENDED CARRYING IT ON THE GROUND THAT `handoff-v69` ALREADY STATES IT IN A
DOCUMENT DESTINED FOR THE PUBLIC ARCHIVE. THAT WAS WRONG TOO** — the handoff archive is on the
server at `/mnt/storage/REM/handoffs` and is not the public repo. **A SECOND FILE STATING
SOMETHING IS NOT A REASON, AND WHERE THAT FILE LIVES WAS ASSUMED RATHER THAN CHECKED.**

**THE GENERAL FORM, REGISTERED BECAUSE IT WILL RECUR: A RULE 7 CHECK THAT EXAMINES EACH VALUE ON
ITS OWN IS NOT SUFFICIENT.** **TWO INDIVIDUALLY HARMLESS QUANTITIES IN ONE FILE CAN COMBINE INTO
A SUBJECT VALUE**, and the combining step here was an equation the same file supplies. **THE
CHECK IS OVER WHAT THE FILE PERMITS A READER TO DERIVE, NOT OVER WHAT IT STATES.**

**SO THIS FILE CARRIES THE INVERSION AND NOT THE RATIO.** **THE COST IS REGISTERED HONESTLY: THE
PUBLIC SPEC NOW ASSERTS THE TWO CASES ARE SEPARABLE WITHOUT SHOWING BY HOW MUCH**, and a reader
of the public record alone must take that on the private file's authority. **THAT IS THE CORRECT
TRADE UNDER RULE 7 AND IT IS NOT A COMFORTABLE ONE.**

**SCAN BEFORE THE PUBLIC COMMIT REGARDLESS. THIS SECTION IS NOT A SUBSTITUTE FOR IT, AND THE SCAN
MUST NOW LOOK FOR DERIVABLE VALUES AND NOT ONLY FOR STATED ONES.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**

**NO READING FROM THIS INSTRUMENT EXISTS. THIS FILE IS FROZEN BEFORE THE COUNTER IS BUILT AND
BEFORE ANY NIGHT HAS RUN WITH IT, AND THAT IS DELIBERATE.**
