# rejected-interval-divisor-counter-spec-v1

**Dated 2026-08-27. Written after N32 is scored and pushed, and after
`hrv-coincidence-null-mixture-s6-correction-2026-08-27` corrected the support for the weakness
this file answers.**

**FILED IN `~/recovery/docs/` — THE PUBLIC REPO. RULE 7 CHECK PERFORMED IN §13: NO SUBJECT
PHYSIOLOGICAL VALUE APPEARS IN THIS FILE.**

**INSTRUMENT ONLY. NO CLASSIFIER INPUT, NO STAGE DECISION, NO TOTAL, NO STORED FIELD, NO
PERSISTED STRUCTURE.** `CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION` do NOT change and MUST be
**VERIFIED UNCHANGED BY READING `src/c/storage.h` AFTER IMPLEMENTATION, NOT ASSERTED.**

**NOTHING IS IMPLEMENTED FROM THIS FILE UNTIL IT IS FROZEN AND PUSHED.**

---

## §0 — THIS IS THE INSTRUMENT THE RECORD CALLS `D10r`. READ THIS FIRST IF YOU GREPPED FOR IT.

**`D10r` IS THE NAME THE RECORD USES FOR THE INSTRUMENT THIS FILE SPECIFIES.** It appears in
`coarse-adjacency-counter-spec-v1`, in `p6-instrument-disqualification-2026-08-27`, in
`hrv-coincidence-null-mixture-s6-correction-2026-08-27`, in `N32-preregistration.md`, in
`C00-N32.md` and in the handoff series, always as an instrument that is OWED.

**THIS FILE DISCHARGES THAT DEBT. THE COUNTERS IT DEFINES ARE NAMED `Drr` AND `Drj`, NOT
`D10r`.**

**`D10r` NAMES NO COUNTER, NO VARIABLE, NO RENDERED VALUE AND NO SYMBOL IN ANY SOURCE FILE. IT
NEVER DID.** A search of `src/` at `5c68e6a` returns no occurrence of it. **NOTHING HAS BEEN
DROPPED, RENAMED IN CODE, OR LOST.**

**WHY THE NAME CHANGED, AND IT IS NOT COSMETIC.** `D10`, `D20`, `D3` and the rest name divisors
over the ACCEPTED population. **A label of the form `D10r` reads as "`D10`, but rejected" and
therefore as the same quantity over a different population.** It is not the same quantity — see
§3 — and the cross-population confusion that shorthand invites is precisely what this instrument
exists to examine. **RULE 9: TWO DIFFERENT QUANTITIES MUST NOT CARRY SIMILARLY-SHAPED LABELS.**

**THE FROZEN FILES NAMING `D10r` ARE NOT EDITED.** A change is a superseding dated file, never an
edit. **THIS SECTION IS THE BRIDGE, AND IT IS PLACED FIRST SO A REVIEWER FINDS IT IMMEDIATELY.**

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

## §3 — The quantity, and what it is NOT

**IT MEASURES WHETHER INTERVALS THE GATES REJECT ARE DIVISIBLE BY 10 AT THE SAME RATE AS
INTERVALS THE GATES ACCEPT.**

**IT IS NOT AN ADJACENCY TEST AND IT IS NOT `Dc` OVER A DIFFERENT POPULATION.** `Dc` counts
consecutive accepted PAIRS in which both members are coarse. **A REJECTED INTERVAL HAS NO PAIR IN
THAT SENSE** — it never enters the buffer, it never becomes anyone's predecessor, and `s_dm_prev`
is not advanced by it. **THIS IS A SINGLE-INTERVAL DIVISIBILITY TEST over the rejected
population**, the same shape as `D10` over the accepted one.

**IT DOES NOT MEASURE `c` FOR THE REJECTED POPULATION AND MUST NOT BE READ AS DOING SO.** `c` is
a pair quantity. **NO INVERSION FROM `f53ea21` §8 APPLIES TO ANYTHING THIS INSTRUMENT PRODUCES.**

**IT DOES NOT REVISE ANY NULL.** `f53ea21` §5's range, and the closed null N32 produced, are over
the accepted population and are unaffected by any reading here. **A LATER SESSION MUST NOT
RECOMPUTE `Hd`'s NULL FROM THIS INSTRUMENT'S OUTPUT.**

**IT DOES NOT ESTABLISH WHETHER THE GATES ARE RIGHT.** `rsa-feasibility-readout-spec-v1` §6
registers that a high rejection count is not grounds to move the range band, the jump multiplier
or `HRV_STALE_SEC`, and that holds here unmodified. **A DIFFERENCE BETWEEN THE POPULATIONS IS NOT
A DEFECT IN THE GATES.**

## §4 — Why the split by gate is built in rather than deferred

**THE STATED PURPOSE NEEDS ONE NUMBER: the divisibility rate over the rejected population.
BUILDING IT AS TWO COUNTERS SPLIT BY GATE DOES NOT COMPROMISE THAT PURPOSE, BECAUSE THE AGGREGATE
IS THE SUM AND IS DERIVABLE AT SCORING TIME.** Both split denominators already render on RESULTS
as `rng` and `jmp`, and their sum already renders as `rej`. **NOTHING THE SINGLE-COUNTER FORM
WOULD HAVE ANSWERED IS LOST.**

**`hrv-resolution-readout-spec-v1` §11's REASONING FOR NOT FOLDING IN A SECOND QUESTION IS
CONSIDERED AND DOES NOT APPLY, AND THE DIFFERENCE IS STATED RATHER THAN WAVED.** That reasoning
refuses a second question whose answer would be entangled with the first. **HERE THE SPLIT IS A
PARTITION OF THE SAME POPULATION BY A LABEL THE CODE ALREADY MAINTAINS**, and the first
question's answer is recovered by addition. **A PARTITION IS NOT A SECOND QUESTION.**

**THE COST IS THE SAME.** One render line either way; DIAG 4 goes from six lines to seven in both
designs.

**THE ASYMMETRY BETWEEN THE TWO LIMBS IS REAL AND IS HANDLED BY §9's READING RULE, NOT LEFT AS A
WEAKNESS.** The range gate's denominator is typically an order of magnitude below the jump gate's.
**A PER-NIGHT RANGE LIMB WOULD THEREFORE READ AS NOISE ON MOST NIGHTS, AND A COUNTER THAT IS
UNINTERPRETABLE ON MOST NIGHTS IS NOT AN INSTRUMENT.** §9 resolves this by reading the range limb
POOLED and never per-night. **THAT IS A DESIGN DECISION TAKEN HERE, NOT A TENSION RECORDED FOR A
LATER SESSION.**

## §5 — The counters

**TWO COUNTERS. BOTH SESSION-SCOPED, BOTH `uint32_t`, BOTH GATED ON `s_recording`, BOTH RESET IN
`prv_start_recording`.**

| Label | Meaning |
|---|---|
| `Drr` | intervals REJECTED BY THE RANGE GATE on `s_night_buf` that are divisible by 10 |
| `Drj` | intervals REJECTED BY THE JUMP GATE on `s_night_buf` that are divisible by 10 |

**DEFINED OVER `s_night_buf` AND OVER NOTHING ELSE**, on the grounds
`rsa-feasibility-readout-spec-v1` §3 states: the gates are PER-BUFFER, each `HrvBuffer` carries
its own references, and the same interval can be accepted by one buffer and rejected by another.
**A COUNTER THAT DOES NOT NAME ITS BUFFER IS MEANINGLESS.** `s_live_buf` is ungated and
`s_minute_buf` resets every minute; **neither is counted here.**

**NO THIRD COUNTER IS ADDED.** In particular **no counter is added for quality rejections** — the
quality gate is unreachable, all three call sites passing the literal `1`, and a counter whose
value can only ever be zero is not an instrument. **`rej_quality` IS NOT TOUCHED, NOT RENDERED AND
NOT REINTERPRETED.**

**NO DENOMINATOR COUNTER IS ADDED.** `rng` and `jmp` already render on RESULTS from
`s_night_buf.rej_range` and `s_night_buf.rej_jump`.

**NOTHING IS PERSISTED.** `NightSummary` does not change, no field is appended, and
`NIGHT_SUMMARY_VERSION` stays 3. **THE POOLING IN §9 IS PERFORMED AT SCORING TIME ACROSS NIGHT
FILES AND REQUIRES NO STORAGE ON THE WATCH** — see §9's justification for why no on-watch
accumulation is needed.

## §6 — The increment site, and the structural change it requires

**READ FROM SOURCE AT `d787269`, RE-READ AT `5c68e6a` WHICH TOUCHED NO CODE.**

**THERE IS NO `else` BRANCH ON `if (acc)` TODAY.** The `if (acc)` block closes and
`s_g_prev_rej = !acc;` follows it. **THIS SPEC ADDS AN `else` BRANCH THAT DOES NOT CURRENTLY
EXIST**, attached to the same `if (acc)`. **THAT IS A STRUCTURAL ADDITION RATHER THAN A LINE ADDED
TO AN EXISTING BLOCK, AND IT IS NAMED AS SUCH BECAUSE §8's MISPLACEMENT CHECK MUST COVER A BRANCH
THAT DID NOT PREVIOUSLY EXIST.**

**`ppi` IS IN SCOPE THERE.** It is the local assigned from `health_service_peek_hrv_ppi_ms()`
above the `if (ppi > 0)` guard, and it remains live through `s_g_prev_rej = !acc;`.

**THE GATE IDENTITY IS NOT AVAILABLE FROM `acc`.** `hrv_buf_add` returns a bare `bool`; **which
gate rejected the interval is known only inside it.** **`hrv_buf_add` IS NOT EDITED — a standing
prohibition, and this spec seeks no exception to it.**

**THE MECHANISM: SNAPSHOT BEFORE, COMPARE AFTER.** `rej_range` and `rej_jump` are already
maintained per-buffer by `hrv_buf_add` and are already readable from the caller.

- Immediately BEFORE the `s_night_buf` call, capture `s_night_buf.rej_range` and
  `s_night_buf.rej_jump` into two locals.
- In the new `else` branch, whichever of the two has increased identifies the gate.
- **THE DIVISIBILITY TEST IS ON `ppi` AND ON NOTHING READ BACK FROM THE BUFFER.** The rejected
  interval's value is never stored, so `ppi` is the only source for it.

**WHY A SNAPSHOT RATHER THAN RE-DERIVING THE GATE CONDITION AT THE CALLER.** Re-deriving would
duplicate the range band and the jump multiplier outside `hrv_math.c`. **THAT WOULD PUT TWO COPIES
OF A CONSTANT IN THE CODE AND IS FORBIDDEN BY RULE 2. THE SNAPSHOT READS THE DECISION THE GATE
ACTUALLY MADE RATHER THAN GUESSING AT IT**, and it stays correct if a later dated spec ever moves
either constant.

**EXACTLY ONE OF THE TWO CAN HAVE INCREASED PER CALL**, because `hrv_buf_add` returns immediately
from whichever gate fires. **IF NEITHER INCREASED WHILE `acc` IS FALSE, THAT IS A QUALITY
REJECTION AND IS UNREACHABLE; §8 REGISTERS WHAT TO DO IF IT IS EVER OBSERVED.**

**DECLARATION AND RESET SITES FOLLOW THE LOCAL CONVENTION, READ FROM SOURCE:** declarations in the
statics block beside `s_dc`, each with a comment citing this spec by name; resets in
`prv_start_recording` beside the `s_dc` reset, with a trailing `rejected-interval-divisor-spec-v1
s5` comment, matching the form every session-scoped counter in that function already uses.

## §7 — Render

**DIAG 4 GOES FROM SIX LINES TO SEVEN.** One new line, two values: **`Drr <n>  Drj <n>`.**

**IT SITS ABOVE `Dm`.** Both the `D20` and `Dc` render comments state the rule and its reason
outright — **`Dm` REMAINS THE LAST LINE, WHICH IS WHERE THE CAPTURE SEQUENCE'S READER HAS LEARNED
TO FIND IT.** The new line goes immediately after `Dc` and immediately before `Dm`.

**THE ARITHMETIC, READ FROM `prv_draw_diag4` RATHER THAN ASSUMED.** Header `GOTHIC_24_BOLD` at
28px from `y = 2`; six value lines at `GOTHIC_14`, 18px each; **final `y` is 138.** A seventh value
line draws at 138 and leaves `y` at 156. **DIAG 2 RENDERS NINE LINES BY THE SAME FONT AND ADVANCE
TO `y = 190`, VERIFIED ON THE WATCH 2026-08-20 WITH NOTHING CLIPPED AT EITHER END.** 156 is 34px
inside a height already proven on this hardware. **THAT IS AN EMPIRICAL READ AND NOT AN INFERENCE
FROM A DISPLAY CONSTANT.**

**SEVEN LINES MUST STILL BE VERIFIED ON THE WATCH, WITH NOTHING CLIPPED AT EITHER END AND THE
FIRST LINE PRESENT, BEFORE ANY NIGHT IS RECORDED UNDER THIS SPEC.** The margin above is a
prediction; **the verification is the evidence.**

**THE FONT IS NOT SHRUNK ON ANY SCREEN AND NO VALUE IS DROPPED.** The nine-line precedent makes
seven fit at `GOTHIC_14`; **a smaller font is neither needed nor permitted here.**

**THE UNDEFINED GUARD KEYS ON `s_session_start`**, identical to every other line on this screen,
so the values still read after stop. **Zero prints `--` and NEVER `0`, per `measurement-spec-v1`
§3.5** — cited as §3.5 and not §3.6, per `display-guard-citation-correction-2026-08-26` §8.

**A DEFINED `Drr` OR `Drj` OF 0 IS A REAL ZERO AND IS A FINDING** — no interval rejected by that
gate was divisible by 10. **IT IS NOT THE SAME STATE AS `--`.**

**NO FRACTION AND NO RATE IS RENDERED.** Both numerators are on this screen and both denominators
are on RESULTS, so deriving at scoring time is not a RULE 6 violation — the same reasoning the
anchor, stillness, cadence and coarse-adjacency readouts give.

**`Drr` AND `Drj` REUSE NO EXISTING LABEL ON ANY SCREEN.** RULE 9 check: `rej`, `rng` and `jmp` on
RESULTS are counts of rejections, not counts of divisible rejections, and are different
quantities. **THE LABELS ARE DELIBERATELY NOT `D10`-SHAPED — see §0.**

**THE STALE HEADER COMMENT ON `prv_draw_diag4` IS CORRECTED IN THE SAME COMMIT.** It records its
own history of going stale — seven when written, nine at the divisor extension without being
updated, ten at coarse adjacency. **THE COUNT BECOMES TWELVE AND THE COMMENT SAYS SO.** **A SPEC
THAT ADDS A VALUE AND LEAVES THAT COMMENT WRONG CREATES A SIXTH WRONG COMMENT IN `main.c` RATHER
THAN LEAVING FIVE**, and `construction-sweep-2026-08-24` §7 records why that matters: source
comments are not admissible as evidence anywhere in this project, and these are why.

## §8 — P-DRIDENT — the misplacement check, ONE-SIDED, and it is a check on the INSTRUMENT

**REGISTERED AS A CONDITIONING CHECK, SCORED FROM THE FIRST NIGHT THE VALUES READ. IT IS NOT A
BASELINE GATE AND IS NOT PART OF GATE 1**, per `f7cc75d` §4.1.

**THE BOUNDS:**

```
Drr  <=  rng
Drj  <=  jmp
```

**NO TOLERANCE IS REGISTERED ON EITHER AND NONE MAY BE APPLIED.** A counter of divisible
rejections cannot exceed the count of rejections by that gate. **A VIOLATION MEANS THE COUNTER IS
MISPLACED — incrementing outside the `else` branch, or under the wrong gate's arm, or outside the
`s_recording` gate — AND IS A DEFECT IN THE INSTRUMENT, NOT A FINDING ABOUT THE SENSOR.**

**IT IS ONE-SIDED AND THAT IS STATED PLAINLY RATHER THAN LEFT TO BE DISCOVERED.** It catches an
increment that fires too often. **IT DOES NOT CATCH AN INCREMENT THAT FIRES TOO SELDOM** — an edit
moving either increment under a narrower condition leaves both bounds satisfied and the check
silent. **NOTHING AT RUNTIME WOULD CATCH THAT MOVE.**

**THE UNCOVERED SIDE IS CLOSED BY A SOURCE READ AT THE PIN, PER NIGHT, EXACTLY AS
`coarse-adjacency-counter-spec-v1` §5.2 REQUIRES FOR `P-DCIDENT`.** The read confirms: both
increments sit inside the new `else` branch attached to `if (acc)`; each sits under its own
snapshot comparison and under no other conditional; both sit inside the `s_recording` block; and
the snapshots are taken immediately before the `s_night_buf` call and nowhere else. **A SOURCE
READ IS VALID FOR THE PIN IT IS RUN AT AND FOR NO LATER PIN. IT NEVER ACCUMULATES.**

**WHAT IS NOT REGISTERED AS A CHECK, AND WHY — THIS IS A RULE 20 SECTION.**

- **`Drr + Drj` AGAINST A TOTAL.** No total counter exists and none is added. Were one added it
  would agree by construction.
- **THE QUALITY-GATE ARM.** `acc` false with neither `rej_range` nor `rej_jump` increased is
  unreachable, since all three call sites pass the literal `1`. **NO COUNTER IS ADDED FOR IT AND NO
  CHECK IS REGISTERED ON IT.** **IF IT IS EVER OBSERVED — a rejection attributable to neither gate
  — THAT IS A FINDING ABOUT THE CALL SITES AND IT IS RECORDED AND DIAGNOSED BEFORE ANYTHING ELSE
  ON THE SCREEN IS READ.** It is not silently folded into either counter.
- **`Drr <= Drr + Drj` AND SIMILAR.** Constructions. **THE INCREMENT SITES SHARE A PATH AND NO
  REACHABLE STATE BREAKS THEM.** Reporting one as a finding is the defect
  `construction-sweep-2026-08-24` and `hrv-event-per-beat-corroboration-withdrawal-2026-08-24`
  exist to prevent, and `hrv-coincidence-null-mixture-s6-correction-2026-08-27` records the most
  recent instance.

## §9 — How a reading is taken, REGISTERED BEFORE ANY READING EXISTS

**NO BAND IS PLACED ON `Drr` OR `Drj` FOR THEIR FIRST NIGHT**, following
`awake-runs-readout-spec-v1` §9, the same clause in `stillness-run-readout-spec-v1`,
`hrv-cadence-readout-spec-v1` §6 and `coarse-adjacency-counter-spec-v1` §9.

**THE COMPARISON IS ALWAYS AGAINST `D10 / Dn` OVER THE SAME NIGHTS AND NEVER AGAINST ANOTHER
PERIOD'S.**

### 9.1 The power guard, DERIVED

**A LIMB IS READ ONLY WHEN ITS DENOMINATOR IS AT LEAST 30.** Below that it is recorded **NOT
INFORMATIVE** and no reading is taken from it.

**THE DERIVATION, STATED SO IT CAN BE CHECKED RATHER THAN TRUSTED.** The two hypotheses the
instrument must separate are: the rejected population is quantised like the accepted one, at a
rate near the accepted population's; and the rejected population has no coarse component at all,
in which case its rate is the chance-divisibility rate of 0.100. **At n = 30 the standard error on
a proportion near the accepted rate is about 0.086 and the 95% half-interval is about 0.169**, so
an observed accepted-like rate spans roughly 0.165 to 0.503 — **which excludes 0.100.** At n = 30
the normal approximation is also comfortable, `n · p` being near 10. **AT n = 20 THE INTERVAL
WIDENS TO ABOUT 0.207 AND THE SEPARATION IS MARGINAL; AT n = 60 IT NARROWS TO ABOUT 0.119.**

**30 IS NOT A CONSTANT IN THE CODE, IS READ BY NOTHING, AND BRANCHES NO DECISION IN THE APP** —
it is arithmetic in the guidance, the same standing `rsa-feasibility-readout-spec-v1` §7 gives its
"3 to 6 beats per breath". **IT IS DERIVED ABOVE, NOT ASSERTED, AND IT IS NOT MOVED IN RESPONSE TO
A RUN OF READINGS THAT FAIL IT.**

**A READING NEAR THE BOUNDARY IS REPORTED WITH ITS INTERVAL AND NEVER AS A BARE POINT.**

### 9.2 The jump limb is read PER NIGHT

**ITS DENOMINATOR CLEARS THE GUARD ON AN ORDINARY NIGHT.** It is scored per night against that
night's own `D10 / Dn`.

### 9.3 The range limb is read POOLED, and NEVER per night

**THE PER-NIGHT RANGE LIMB IS RECORDED RAW AND IS NEVER INTERPRETED ALONE.** Its denominator does
not clear the guard on an ordinary night, and **a per-night reading that cannot clear its own
power guard is not a weak finding — it is not a finding.**

**IT IS READ AS A POOLED QUANTITY: `Drr` SUMMED ACROSS NIGHTS AGAINST `rng` SUMMED ACROSS THE SAME
NIGHTS**, once the pooled denominator clears 30, compared against `D10` summed against `Dn` summed
over those same nights.

**NO ON-WATCH ACCUMULATION IS NEEDED AND NONE IS BUILT.** DIAG 4 is captured in the stop sequence
every night and both values land in that night's record, exactly as every other DIAG 4 value does.
**POOLING IS ARITHMETIC OVER NIGHT FILES AT SCORING TIME.** **THIS IS WHY §5 PERSISTS NOTHING:
persistence would be required only if the watch had to do the accumulating, and it does not.**

**WHY POOLING IS LEGITIMATE HERE WHEN IT WOULD NOT BE FOR A PHYSIOLOGICAL QUANTITY.**
`hrv-resolution-readout-spec-v1` §8 grounds one-night sufficiency for the divisors on the fact
that **quantisation is a property of the SENSOR and not of the wearer.** That same ground licenses
pooling: nights are exchangeable for a sensor property in a way they are never exchangeable for a
sleep quantity. **NO PHYSIOLOGICAL QUANTITY IN THIS PROJECT MAY BE POOLED ON THE STRENGTH OF THIS
SECTION.**

**THE POOL IS SCOPED TO NIGHTS AT AN UNCHANGED GATE CONFIGURATION.** A dated spec moving the range
band, the jump multiplier or `HRV_STALE_SEC` changes which intervals land in the rejected
population. **THE POOL STARTS OVER AT SUCH A CHANGE AND NIGHTS ACROSS IT ARE NEVER COMBINED.**
The pool is not scoped to a single pin otherwise — an instrument-only commit does not change which
intervals are rejected.

**THE POOLED READING IS NOT AVAILABLE ON THE FIRST NIGHT AND THAT IS NOT A DEFICIENCY.** It
becomes available when the denominator does. **NO SESSION MAY TAKE A RANGE READING EARLIER BY
LOWERING THE GUARD.**

### 9.4 The branches, all reachable and all distinguishable

1. **A LIMB'S RATE NEAR `D10 / Dn`.** The rejected population is quantised like the accepted one.
   **Weakness 6's scope mismatch would then be a difference in POPULATION and not in
   QUANTISATION** — which narrows the weakness without closing it.
2. **A LIMB'S RATE MATERIALLY BELOW.** The gates preferentially reject fine intervals.
3. **A LIMB'S RATE MATERIALLY ABOVE.** The gates preferentially reject coarse intervals, which
   would mean the accepted population UNDERSTATES the sensor's coarse fraction — **and `f` is
   measured over the accepted population.**
4. **THE TWO LIMBS DIFFERING FROM EACH OTHER**, both past the guard. The two rejection paths see
   different populations.
5. **A LIMB BELOW THE GUARD.** Recorded NOT INFORMATIVE. **NOT a finding in any direction, and it
   says nothing beyond what `rng` or `jmp` already render.**

**WHAT NO BRANCH LICENSES.** **NONE MOVES A BAND, A GATE, A MULTIPLIER OR `HRV_STALE_SEC`. NONE
REVISES `Hd`'s NULL. NONE BEARS ON WHETHER `Hd` IS RE-LATCHING. NONE BEARS ON RSA OR RRV.** Branch
3 is the one most likely to be read as licensing a correction to `f`; **it does not, and such a
correction would be its own dated spec resting on more than one reading.**

## §10 — What this instrument CANNOT establish

**WHY ANY INTERVAL IS QUANTISED.** Inside the nonfree Goodix algorithm, which
`hrv-resolution-readout-spec-v1` §1 records as **NOT ESTABLISHABLE FROM SOURCE.**

**WHETHER THE REJECTED INTERVALS ARE REAL BEATS.** The gates exist because some intervals are
artefact. **A DIVISIBILITY RATE SAYS NOTHING ABOUT WHICH.**

**WHETHER `Hd` IS RE-LATCHING.** Unchanged by anything here.

**WHETHER THE GATES SHOULD REJECT WHAT THEY REJECT.** §3.

**ANYTHING ABOUT `c`.** §3. This is a single-interval instrument.

**ANYTHING FROM ONE NIGHT ON THE RANGE LIMB** — by construction of §9.3, which is a property of
the population and not a limitation to be worked around.

## §11 — Comparability

**INSTRUMENT ONLY. NO OUTPUT CHANGE.** No classifier input, no stage decision, no minute total, no
stored field, no persisted structure. **`CLASSIFIER_SERIES` STAYS 14 AND `NIGHT_SUMMARY_VERSION`
STAYS 3, TO BE VERIFIED BY READING `src/c/storage.h` AFTER IMPLEMENTATION AND NOT ASSERTED.**

**THE FIRST NIGHT UNDER THIS INSTRUMENT REMAINS COMPARABLE TO N26 ONWARD ON EVERY STAGE LINE.**
The `stage_comparability_key`, `base_comparability_key` and `rri_comparability_key` are unaffected.

**A NIGHT UNDER THIS SPEC IS NOT A NIGHT OF A REPLICATION RUN AND CANNOT BE ONE.** A run requires
one pin with no code changes of any kind, **instrument-only explicitly included.** It is a fresh
baseline attempt at a new pin.

**ONE COST IS REGISTERED HONESTLY: two loads and two comparisons on every HRV event, plus up to
two divisibility tests on the rejection path only.** **NO PREDICTION IS MADE ABOUT ITS POWER COST
AND NONE MAY BE INFERRED FROM ONE NIGHT** — P4's band and battery's integer-percent resolution are
unchanged by this spec, and **the DIAG 5 timing readout MUST NOT be cited toward it. TIME IS NOT
ENERGY.**

## §12 — What the capture sequence must do

**THE CAPTURE SEQUENCE IS UNCHANGED AT EIGHT CAPTURES AND NO NEW SCREEN IS ADDED.** DIAG 4 is
already captured; it carries one more line.

**THE COUNTERS ARE RESET IN `prv_start_recording` AND ARE SESSION-SCOPED**, so they read from the
press of record to the press of stop. **THEY DIE WITH THE SCREEN** — captured live or lost, exactly
as the cadence, stillness, RSA and coarse-adjacency values are.

**NO PRE-RECORDING CAPTURE IS REQUIRED BY THIS SPEC AND NONE IS INTRODUCED BY IT.** The reset is
unconditional at session start, so no state carried in from before the press can reach these
counters. **A SESSION-START CAPTURE WOULD RECORD ONLY ZEROES AND IS NOT ASKED FOR.**

## §13 — Rule 2, Rule 3 and Rule 7 checks

**RULE 2 — NO CONSTANT ENTERS ANY CODE PATH.** The divisor 10 is the existing coarse grid decided
by `N30-resolution-decision-2026-08-24` and is read, not introduced. **The range band and the jump
multiplier are NOT duplicated at the caller — that is the whole reason for the snapshot mechanism
in §6.** The 30 in §9.1 is arithmetic in the guidance, derived there, and is read by nothing.

**RULE 3 — THE CRITERION REGISTERED IS `P-DRIDENT` AND IT IS NOT VACUOUS.** **IT CAN FAIL:** a
counter incrementing outside its branch produces a value above its denominator, a reading the
running program can produce under a misplaced increment. **IT CAN PASS.** **IT IS ONE-SIDED AND §8
SAYS SO.**

**CHECKED AGAINST THE THIRD VACUITY FORM — A FINDING THAT MEANS SOMETHING ELSE. THREE ARE
REGISTERED:**

1. **A `P-DRIDENT` PASS READ AS EVIDENCE ABOUT THE SENSOR.** It is evidence about the counter's
   placement and nothing else.
2. **BRANCH 3 READ AS LICENSING A CORRECTION TO `f`.** §9.4 forbids it.
3. **A BELOW-GUARD LIMB READ AS A NULL RESULT.** §9.4 branch 5. **IT IS AN ABSENCE OF MEASUREMENT
   AND NOT A FINDING THAT THE POPULATIONS AGREE**, the same distinction `f53ea21` §2 draws for the
   nights before `6439950`.

**EVERY VALUE THIS SPEC REGISTERS HAS A RENDER SITE**, named in §7, on a screen the capture
sequence captures live. **NO FUTURE CRITERION AGAINST THESE VALUES CAN BE UNSCORABLE FOR WANT OF A
RENDER SITE.**

**RULE 7 — NO SUBJECT PHYSIOLOGICAL VALUE APPEARS IN THIS FILE.** No night's `rng`, `jmp`, `rej`,
`D10`, `Dn` or `f` is stated, **and no ratio among them is stated or made derivable.** §4 and §9.3
characterise the range denominator as typically an order of magnitude below the jump denominator;
**that is a characterisation of a counter's typical size, names no night, and permits no value to
be recovered.** The 0.100 in §9.1 is a chance-divisibility rate derived from the divisor, not a
measurement. **THE CHECK IS OVER WHAT THE FILE PERMITS A READER TO DERIVE AND NOT ONLY OVER WHAT
IT STATES**, per `coarse-adjacency-counter-spec-v1` §12. **SCAN BEFORE THE PUBLIC COMMIT
REGARDLESS; THIS SECTION IS NOT A SUBSTITUTE FOR IT.**

## §14 — Self-consistency check, per RULE 19

**PERFORMED BEFORE THIS FILE IS FROZEN AND RECORDED HERE AS A NAMED SECTION. A FILE THAT DOES NOT
RECORD THE CHECK HAS NOT HAD IT.**

**FORM 1, ONE QUANTITY DESCRIBED TWICE.** The line count appears in §7's heading claim and in §7's
arithmetic and reads seven in both. The final `y` appears once as 138 before the new line and once
as 156 after it, and 138 + 18 = 156. The two counters are named in §5's table, §7's render line and
§8's bounds, and match in all three. The buffer is named `s_night_buf` in §5, §6 and §8. The guard
value 30 appears in §9.1 twice and in §9.3 once, consistently.

**FORM 2, QUANTITATIVE AGAINST PROSE, SUBSTITUTED AT BOTH EXTREMES.** **At zero rejections:** both
denominators are zero, both limbs fall below the guard, both are NOT INFORMATIVE, and
`P-DRIDENT`'s bounds hold with both counters at zero — **and §7's real-zero clause does not
misfire, because §9.1's guard is checked ahead of it.** **At all intervals rejected:** `Dn` is
zero, `D10 / Dn` is undefined, **every §9.4 branch loses its referent and the night yields nothing
from this instrument** — which the prose accommodates, since the branches are stated as
comparisons against `D10 / Dn` and a missing referent makes them NOT PERFORMED rather than false.

**FORM 3, EVERY BRANCH REACHABLE.** Checked in §13. §9.4's five branches are each producible by
the running program.

**FORM 4, SECTIONS SERVING DIFFERENT READERS, PAIRED BY SHARED QUANTITY.** §0 serves a reviewer
who grepped `D10r`; §3 serves whoever asks what is measured; §5 and §6 serve whoever implements;
§7 serves whoever renders; §8 and §9 serve whoever scores; §10 serves whoever is tempted to
over-read a result.

**THE PAIRINGS ACTUALLY RUN:**

- **§4 AGAINST §9.3**, because both bear on the split's value: §4 argues the split costs nothing
  and §9.3 supplies the reading rule that makes the weaker limb interpretable. **THEY AGREE, AND
  §4 STATES THE ASYMMETRY ITSELF RATHER THAN LEAVING IT TO §9** — an argument for a design that
  omits its own cost is a shape this project has had to correct before.
- **§5 AGAINST §9.3**, because both bear on persistence: §5 persists nothing and §9.3 explains why
  nothing needs persisting. **THEY AGREE, AND §5 POINTS AT §9.3 EXPLICITLY** so the absence of a
  stored field reads as a decision rather than an omission.
- **§6 AGAINST §8**, because both constrain where the increments sit: §6 places them in a new
  `else` branch and §8's source read checks exactly that placement. **THEY AGREE, AND §8's READ IS
  SPECIFIED AGAINST §6's STRUCTURE RATHER THAN A REMEMBERED ONE.**
- **§3 AGAINST §9.4 BRANCH 3**, because both constrain what may be concluded about `f`: §3 forbids
  revising any null and branch 3 names the reading most likely to invite it. **THEY AGREE.**
- **§7 AGAINST §12**, because both bear on capture: §7 adds a line to a captured screen and §12
  states the sequence is unchanged and no pre-recording capture is needed. **THEY AGREE.**

**NO TENSION IS LEFT STANDING IN THIS FILE.** An earlier draft recorded the range limb's expected
uninformativeness as a tension between a design decision and its cost. **THAT WAS A DEFECT
PREDICTED IN ADVANCE AND SHIPPED, NOT A TRADE-OFF**, and §9.3 resolves it. **A FROZEN FILE MAY NOT
CARRY AN UNRESOLVED ITEM; IT IS RESOLVED OR IT IS DROPPED BEFORE THE FILE IS FROZEN.**

**NO DEFECT WAS FOUND BY THIS CHECK IN THIS FILE.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**

**NOTHING IS IMPLEMENTED FROM THIS FILE UNTIL IT IS FROZEN AND PUSHED.**

**THE SEVEN-LINE DIAG 4 RENDER MUST BE VERIFIED ON THE WATCH BEFORE ANY NIGHT IS RECORDED UNDER
IT. `CLASSIFIER_SERIES` AND `NIGHT_SUMMARY_VERSION` MUST BE VERIFIED UNCHANGED BY READING
`src/c/storage.h` AFTER IMPLEMENTATION.**

**THIS FILE DISCHARGES THE INSTRUMENT THE RECORD OWES AS `D10r`. SEE §0.**
