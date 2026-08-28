# residue-histogram-spec-v1

**Dated 2026-08-27. Owed by `coarse-mixture-model-refutation-2026-08-27` §8.**

**FILED IN `~/recovery/docs/` — THE PUBLIC REPO. RULE 7 CHECK PERFORMED IN §13: NO SUBJECT
PHYSIOLOGICAL VALUE APPEARS IN THIS FILE.**

**INSTRUMENT ONLY. NO CLASSIFIER INPUT, NO STAGE DECISION, NO TOTAL, NO STORED FIELD, NO
PERSISTED STRUCTURE.** `CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION` do NOT change and MUST be
**VERIFIED UNCHANGED BY READING `src/c/storage.h` AFTER IMPLEMENTATION, NOT ASSERTED.**

**IT MEASURES A DISTRIBUTION THAT HAS ONLY EVER BEEN INFERRED.** The two-component mixture model
was refuted at seventeen to twenty-three standard errors on four nights, and both candidate
replacements were refuted with it. **THIS INSTRUMENT REPLACES INFERENCE WITH MEASUREMENT. IT
PROPOSES NO MODEL AND THIS FILE CONTAINS NONE.**

**NOTHING IS IMPLEMENTED FROM THIS FILE UNTIL IT IS FROZEN AND PUSHED.**

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

## §3 — The quantity

**THE COUNT OF ACCEPTED INTERVALS IN EACH RESIDUE CLASS MODULO 20.** Twenty counters, `R0`
through `R19`, where `Rj` counts accepted intervals with `ppi % 20 == j`.

**IT IS A MEASUREMENT AND NOT A FIT.** Every quantity the refuted model estimated becomes a
direct reading or a sum of readings. **NO PARAMETER IS INFERRED AND NONE MAY BE.**

**WHY MODULO 20 AND NOT MODULO 10.** `coarse-mixture-model-refutation-2026-08-27` §7 establishes
that the surviving 10 ms coarse-magnitude decision rests on one untested assumption: **that
intervals congruent to 0 modulo 10 split evenly between congruent to 0 and congruent to 10
modulo 20.** A modulo-10 histogram cannot test it, because those are the same residue modulo 10.
**MODULO 20 TESTS THE LAST ASSUMPTION UNDER A DECISION THE PROJECT IS KEEPING.**

**AND THE CHOICE IS NOT SCOPED TO WHAT IS CURRENTLY DEPENDED ON.** Nothing today depends on the
other nineteen residues splitting evenly modulo 20. **THE MODEL THAT WAS JUST REFUTED WAS ALSO
DEPENDED ON BY NOTHING UNTIL IT WAS**, and a measurement that answers only the questions already
asked is a measurement that will be redone.

## §4 — The counters

**TWENTY COUNTERS. `uint32_t s_res[20]`, SESSION-SCOPED, GATED ON `s_recording`, RESET IN
`prv_start_recording`.** Eighty bytes of static footprint.

**INCREMENT SITE — INSIDE `if (acc)`, AT THE SAME NESTING DEPTH AS `s_dn++` AND THE DIVISOR
TESTS, UNDER NO OTHER CONDITIONAL.** Read from source at `d787269` and unchanged at `b4826b9`,
which touched no code. **ONE INDEXED INCREMENT: `s_res[ppi % 20]++`.**

**IT IS CHEAPER THAN WHAT ALREADY RUNS.** One modulo and one indexed increment replaces nothing
and sits beside seven existing divisibility tests. **NO PREDICTION IS MADE ABOUT POWER AND NONE
MAY BE INFERRED FROM ONE NIGHT** — P4's band and battery's integer-percent resolution are
unchanged by this spec, and **the DIAG 5 timing readout MUST NOT be cited toward it. TIME IS NOT
ENERGY.**

**NO EXISTING COUNTER IS REMOVED, MOVED, RENAMED OR REINTERPRETED.** `D2`, `D3`, `D4`, `D5`,
`D8`, `D10`, `D20`, `Dn`, `Dc` and `Dm` all compute exactly what they computed before.
**REMOVING A DIVISOR WOULD BE MOVING IT, AND FIVE OF THEM BECOME CROSS-CHECKS UNDER §7 —
DELETING THEM WOULD DELETE THE CHECK.** `DM_SENTINEL` is not touched. **`hrv_buf_add` IS NOT
EDITED.**

## §5 — Render

**TWO NEW SCREENS, DIAG 6 AND DIAG 7, FIVE VALUE LINES EACH, TWO VALUES PER LINE.**

```
Diag 6                    Diag 7
R0 <n>   R1 <n>           R10 <n>  R11 <n>
R2 <n>   R3 <n>           R12 <n>  R13 <n>
R4 <n>   R5 <n>           R14 <n>  R15 <n>
R6 <n>   R7 <n>           R16 <n>  R17 <n>
R8 <n>   R9 <n>           R18 <n>  R19 <n>
```

**THE ARITHMETIC, READ FROM `prv_draw_diag4` RATHER THAN ASSUMED.** Header `GOTHIC_24_BOLD` at
28px from `y = 2`, value lines `GOTHIC_14` at 18px. **FIVE VALUE LINES LEAVES `y` AT 120.** DIAG 2
renders nine lines by the same font and advance to `y = 190`, verified on the watch 2026-08-20
with nothing clipped. **120 IS 70px INSIDE A HEIGHT ALREADY PROVEN ON THIS HARDWARE.**

**TWO SCREENS RATHER THAN ONE IS FORCED, NOT CHOSEN.** Twenty values at two per line is ten
lines against a nine-line ceiling. **THE FONT IS NOT SHRUNK AND NO VALUE IS DROPPED**, and three
values per line is not attempted at five-digit counts.

**BOTH SCREENS MUST BE VERIFIED ON THE WATCH, NOTHING CLIPPED AT EITHER END AND THE FIRST LINE
PRESENT, BEFORE ANY NIGHT IS RECORDED UNDER THIS SPEC.** The margin above is a prediction; **the
verification is the evidence.**

**THE UNDEFINED GUARD KEYS ON `s_session_start`.** Zero prints `--` and never `0`, per
**`undefined-value-display-rule-2026-08-27` §3.1 case 2** — a session that never ran. **A DEFINED
`Rj` OF 0 IS A REAL ZERO AND IS A FINDING**, per that file's §3.2: no accepted interval fell in
that residue class. **`measurement-spec-v1` §3.5 IS NOT THE CITATION FOR THIS GUARD** — it
governs the persistence case, per the same file's §7.

**NO FRACTION, RATE OR PERCENTAGE IS RENDERED.** All twenty values and `Dn` are on captured
screens, so deriving every proportion at scoring time is not a RULE 6 violation.

**`R0` THROUGH `R19` REUSE NO EXISTING LABEL ON ANY SCREEN.** RULE 9 check performed against
every rendered label in the project.

**RULE 11 APPLIES:** the new labels are verified by `strings` on `pebble-app.elf` in both defined
and undefined variants before the render is trusted.

## §6 — Navigation and the capture sequence

**READ FROM SOURCE AT `b4826b9`. THE DIAG CHAIN IS LINEAR — DIAG through DIAG 5 — EACH SCREEN
TAKING UP AND BACK TO THE PREVIOUS AND DOWN TO THE NEXT. `MODE_DIAG5` HAS NO DOWN HANDLER
TODAY.**

**DIAG 5 GAINS A DOWN HANDLER TO DIAG 6.** DIAG 6 takes UP and BACK to DIAG 5 and DOWN to DIAG 7.
DIAG 7 takes UP and BACK to DIAG 6 and **has no DOWN handler**, terminating the chain exactly as
DIAG 5 does today. **THIS REPEATS THE EXISTING DIAG-TO-DIAG IDIOM AND ADDS NO NEW ONE.**

**`MODE_DIAG6` AND `MODE_DIAG7` ARE APPENDED TO THE `ScreenMode` ENUM AND TO THE DRAW DISPATCH.**

**DIAG 5's SELECT HANDLER IS NOT TOUCHED, NOT REWIRED AND NOT GUARDED FURTHER.**

**THE CAPTURE SEQUENCE GOES FROM EIGHT CAPTURES TO TEN**, adding DIAG 6 and DIAG 7 after DIAG 5.

**THE HARD REQUIREMENT, AND IT IS A REQUIREMENT AND NOT A NOTE: SELECT IS NEVER PRESSED WHILE ON
DIAG 5.** The sequence walks THROUGH DIAG 5 with DOWN and nothing else. `main.c`'s own comment at
the DIAG 5 select handler records why: **a multi-hundred-millisecond CPU burst during the capture
sequence perturbs the very thing every other instrument is measuring, and the session statics are
live at that moment.** **WALKING PAST DIAG 5 IS SAFE. PRESSING SELECT ON IT IS NOT.**

**A PRESS OF SELECT ON DIAG 5 DURING A CAPTURE SEQUENCE IS A CAPTURE DEVIATION AND IS RECORDED
AS ONE**, and the night's DIAG 5 timing values are then attributable to the capture rather than
to the session.

## §7 — P-RIDENT — the misplacement check, and it is a check on the INSTRUMENT

**REGISTERED AS A CONDITIONING CHECK, SCORED FROM THE FIRST NIGHT THE VALUES READ. IT IS NOT A
BASELINE GATE AND IS NOT PART OF GATE 1**, per `f7cc75d` §4.1.

**SIX RELATIONS. NO TOLERANCE IS REGISTERED ON ANY OF THEM AND NONE MAY BE APPLIED.**

```
R0 + R1 + ... + R19            ==  Dn
R0                             ==  D20
R0 + R10                       ==  D10
R0 + R5 + R10 + R15            ==  D5
R0 + R4 + R8 + R12 + R16       ==  D4
R0 + R2 + R4 + ... + R18       ==  D2
```

**THE FIVE DIVISOR RELATIONS ARE EXACT AND WERE VERIFIED BY ENUMERATION OVER THE RESIDUE CLASSES
RATHER THAN ASSERTED.** They hold because 2, 4, 5, 10 and 20 all divide 20.

**`D8` AND `D3` ARE NOT RECONSTRUCTIBLE AND NO RELATION IS REGISTERED FOR THEM.** Neither 8 nor 3
divides 20 — an interval congruent to 8 modulo 20 may or may not be divisible by 8. **NO
APPROXIMATE OR PARTIAL RELATION IS REGISTERED IN THEIR PLACE.**

**WHAT A PASS MEANS, STATED BEFORE ANY PASS EXISTS. IT MEANS THE COUNTERS ARE NOT MISPLACED. IT
IS NOT EVIDENCE ABOUT THE SENSOR AND IT IS NOT CORROBORATION OF ANYTHING.** Both counter sets
increment inside the same `if (acc)` block, so **NO RUNTIME STATE BREAKS THESE AGREEMENTS AND
THEY ARE CONSTRUCTIONS IN RULE 20's SENSE.** They break only under a misplaced increment, which
is a code state and not a reachable one. **REPORTING A PASS AS A FINDING IS THE DEFECT
`construction-sweep-2026-08-24` AND `hrv-event-per-beat-corroboration-withdrawal-2026-08-24`
EXIST TO PREVENT, AND `hrv-coincidence-null-mixture-s6-correction-2026-08-27` RECORDS THE MOST
RECENT INSTANCE OF IT.**

**THE CHECK IS ONE-SIDED IN THE SAME WAY `P-DCIDENT` IS.** It catches an increment firing too
often or landing in the wrong class. **AN INCREMENT MOVED UNDER A NARROWER CONDITION WOULD LOWER
EVERY COUNTER TOGETHER AND SATISFY THE FIRST RELATION ONLY IF `Dn` MOVED WITH IT — SO THE SUM
RELATION CATCHES THAT CASE, AND THE FIVE DIVISOR RELATIONS CATCH A WRONG-CLASS ASSIGNMENT.**
**WHAT REMAINS UNCOVERED IS A CHANGE THAT MOVES THE RESIDUE INCREMENT AND `s_dn++` TOGETHER**,
and that is closed by a source read at the pin, per night, exactly as
`coarse-adjacency-counter-spec-v1` §5.2 requires for `P-DCIDENT`. **A SOURCE READ IS VALID FOR
THE PIN IT IS RUN AT AND FOR NO LATER PIN.**

## §8 — What is read, and how

**NO BAND IS PLACED ON ANY OF THE TWENTY VALUES FOR THEIR FIRST NIGHT**, following
`awake-runs-readout-spec-v1` §9 and the same clause in `stillness-run-readout-spec-v1`,
`hrv-cadence-readout-spec-v1` §6, `coarse-adjacency-counter-spec-v1` §9 and
`rejected-interval-divisor-counter-spec-v1` §9. **THE NIGHT AFTER THEY FIRST READ MAY BAND
THEM.**

**THE READING IS THE DISTRIBUTION ITSELF, REPORTED AS TWENTY PROPORTIONS OF `Dn`.** Derived at
scoring time, never typed.

**THREE QUESTIONS ARE ANSWERED DIRECTLY BY THE FIRST NIGHT'S READING, AND THEY ARE NAMED IN
ADVANCE:**

1. **THE EVEN-SPLIT ASSUMPTION UNDER THE SURVIVING 10 ms DECISION.** `R0` against `R10`. **Equal
   within counting error supports it; a material imbalance refutes it**, and a refutation
   re-opens `N30-resolution-decision-2026-08-24` on its own terms rather than on this
   instrument's.
2. **THE SHAPE OF THE NON-DECADE POPULATION.** The eighteen residues other than 0 and 10.
   **Uniform among themselves, or structured.** The refuted model required uniform; the
   refutation established it is not, without establishing what it is. **THIS READS IT.**
3. **THE DEPLETION OF RESIDUE 5.** `R5` and `R15` directly. The refutation measured their sum
   through `D5 - D10` and found it at about two thirds of what the model required. **THIS SPLITS
   THAT SUM FOR THE FIRST TIME.**

**WHAT A READING DOES NOT LICENSE. NO READING FROM THIS INSTRUMENT MAY BE FITTED TO A NEW MIXTURE
MODEL ON THE NIGHT IT FIRST READS.** A model proposed from the same night that measured it is
fitted to it. **A MODEL, IF ONE IS EVER PROPOSED, IS ITS OWN DATED FILE RESTING ON MORE THAN ONE
NIGHT, AND IT IS PRE-REGISTERED BEFORE THE NIGHT THAT TESTS IT.**

**NO READING REVISES `Hd`'s NULL, WHICH IS WITHDRAWN AND STAYS WITHDRAWN UNTIL ITS OWN DATED
RE-DERIVATION EXISTS.** **NO READING BEARS ON WHETHER `Hd` IS RE-LATCHING. NO READING BEARS ON
RSA OR RRV. NO READING MOVES A BAND, A GATE, A MULTIPLIER OR `HRV_STALE_SEC`.**

## §9 — What this instrument CANNOT establish

**WHY THE DISTRIBUTION HAS WHATEVER SHAPE IT HAS.** The mechanism is inside the nonfree Goodix
algorithm, which `hrv-resolution-readout-spec-v1` §1 records as **NOT ESTABLISHABLE FROM
SOURCE.** **A MEASURED DISTRIBUTION IS NOT A MECHANISM.**

**ANYTHING ABOUT THE REJECTED POPULATION.** These counters sit inside `if (acc)`.
`rejected-interval-divisor-counter-spec-v1` is the instrument for that question and is separate.

**ANYTHING ABOUT ADJACENCY OR PAIRS.** Single-interval counters. `Dc` is the pair instrument.

**WHETHER THE DISTRIBUTION IS STABLE NIGHT TO NIGHT.** One night has no spread.

**ANYTHING ABOUT RESIDUES MODULO ANYTHING NOT DIVIDING 20** — in particular modulo 3 and modulo
8, which is why `D3` and `D8` are retained rather than superseded.

## §10 — Comparability

**INSTRUMENT ONLY. NO OUTPUT CHANGE.** **`CLASSIFIER_SERIES` STAYS 14 AND `NIGHT_SUMMARY_VERSION`
STAYS 3, TO BE VERIFIED BY READING `src/c/storage.h` AFTER IMPLEMENTATION AND NOT ASSERTED.**

**THE FIRST NIGHT UNDER THIS INSTRUMENT REMAINS COMPARABLE TO N26 ONWARD ON EVERY STAGE LINE.**
The `stage_comparability_key`, `base_comparability_key` and `rri_comparability_key` are
unaffected. No stage decision, no classifier path, no smoother input and no minute total is
touched.

**A NIGHT UNDER THIS SPEC IS NOT A NIGHT OF A REPLICATION RUN AND CANNOT BE ONE.** A run requires
one pin with no code changes of any kind, **instrument-only explicitly included.**

**THE CAPTURE COUNT CHANGES FROM EIGHT TO TEN, WHICH IS A CHANGE TO THE NIGHT-FILE RECORD OF THE
SEQUENCE** and is registered here rather than discovered at scoring time.

## §11 — What the capture sequence must do

**NO PRE-RECORDING CAPTURE IS REQUIRED BY THIS SPEC AND NONE IS INTRODUCED BY IT.** The counters
are reset unconditionally in `prv_start_recording`, so no state carried in from before the press
can reach them. **A SESSION-START CAPTURE WOULD RECORD ONLY ZEROES AND IS NOT ASKED FOR.**

**THE VALUES DIE WITH THE SCREEN** — captured live or lost, exactly as the cadence, stillness,
RSA, resolution and coarse-adjacency values are.

**THE TEN CAPTURES ARE TAKEN IN ORDER AND DIAG 5 IS TRAVERSED WITH DOWN ONLY — SEE §6.**

## §12 — Rule 2 and Rule 3 checks

**RULE 2 — NO CONSTANT ENTERS ANY CODE PATH THAT ANY DECISION READS.** The modulus 20 is **the
definition of the measurement, not a threshold**, exactly as the divisors are. `HRV_STALE_SEC`,
the range band, the jump multiplier, `HRV_BUF_MAX` and `DM_SENTINEL` are read and **UNMOVED**.
**NO BAND IS REGISTERED ON ANY VALUE.**

**RULE 3 — THE CRITERION REGISTERED IS `P-RIDENT` AND IT IS NOT VACUOUS.** **IT CAN FAIL:** a
misplaced or wrongly-indexed increment produces a sum above `Dn` or a divisor reconstruction that
disagrees. **IT CAN PASS.** **ITS ONE-SIDEDNESS AND ITS CONSTRUCTION STATUS ARE STATED IN §7
RATHER THAN LEFT TO BE DISCOVERED.**

**CHECKED AGAINST THE THIRD VACUITY FORM — A FINDING THAT MEANS SOMETHING ELSE. FOUR ARE
REGISTERED:**

1. **A `P-RIDENT` PASS READ AS EVIDENCE ABOUT THE SENSOR.** §7 forbids it.
2. **THE DISTRIBUTION READ AS CONFIRMING OR RESTORING ANY MIXTURE MODEL.** §8 forbids fitting on
   the first night and `coarse-mixture-model-refutation-2026-08-27` §6 forbids the two candidates
   already refuted.
3. **A BALANCED `R0` AGAINST `R10` READ AS CONFIRMING THE 10 ms DECISION.** It supports the
   assumption the decision rests on. **THE DECISION WAS MADE ON `D20 / D10` AND IS NOT RE-DERIVED
   FROM THIS INSTRUMENT.**
4. **A DEFINED `Rj` OF ZERO READ AS AN UNDEFINED VALUE.** §5 and
   `undefined-value-display-rule-2026-08-27` §3.2 forbid it. **A REAL ZERO IS A FINDING.**

**EVERY VALUE THIS SPEC REGISTERS HAS A RENDER SITE**, named in §5, on screens the capture
sequence captures live. **NO FUTURE CRITERION AGAINST THESE VALUES CAN BE UNSCORABLE FOR WANT OF
A RENDER SITE.**

## §13 — Rule 7 check

**NO SUBJECT PHYSIOLOGICAL VALUE APPEARS IN THIS FILE.** No night's counts, no proportions, no
ratios, and nothing from which any could be derived. **THE REFUTATION'S PER-NIGHT TABLES ARE IN
`~/recovery-nights/docs/coarse-mixture-model-refutation-2026-08-27.md` AND ARE NOT REPRODUCED
HERE, NOR MAY THEY BE**, per that file's §10. **THE CHECK IS OVER WHAT THE FILE PERMITS A READER
TO DERIVE AND NOT ONLY OVER WHAT IT STATES.** **SCAN BEFORE THE PUBLIC COMMIT REGARDLESS; THIS
SECTION IS NOT A SUBSTITUTE FOR IT.**

## §14 — Self-consistency check, per RULE 19

**PERFORMED BEFORE THIS FILE IS FROZEN AND RECORDED HERE AS A NAMED SECTION. A FILE THAT DOES NOT
RECORD THE CHECK HAS NOT HAD IT.**

**FORM 1, ONE QUANTITY DESCRIBED TWICE.** The counter count is twenty in §3, §4 and §5's layout,
which shows ten labels per screen across two screens. The line count is five value lines per
screen in §5's heading and in its layout block. The final `y` of 120 follows from 2 + 28 + 5 × 18,
stated once and checked. The capture count is ten in §6, §10 and §11.

**FORM 2, QUANTITATIVE AGAINST PROSE, SUBSTITUTED AT BOTH EXTREMES.** **At a session with no
accepted intervals:** every `Rj` is a defined zero, the sum relation holds at 0 == `Dn` == 0, all
five divisor relations hold at zero, and §5's real-zero clause applies rather than the undefined
guard — **which is correct, because the session ran.** **At a session in which every interval
falls in one residue class:** that `Rj` equals `Dn`, all other counters are defined zeros, and the
divisor relations still hold — **and `Dm` would read the grid spacing rather than 1**, which is
the uniform-grid signature `hrv-resolution-readout-spec-v1` §6 already governs. **No statement in
this file fails at either extreme.**

**FORM 3, EVERY BRANCH REACHABLE.** Checked in §12. §8's three questions each have both outcomes
producible by the running program.

**FORM 4, SECTIONS SERVING DIFFERENT READERS, PAIRED BY SHARED QUANTITY.** §3 serves whoever asks
what is measured; §4 and §5 serve whoever implements; §6 serves whoever captures; §7 and §8 serve
whoever scores; §9 serves whoever is tempted to over-read.

**THE PAIRINGS ACTUALLY RUN:**

- **§3 AGAINST §8**, because both bear on why modulo 20: §3 justifies the modulus by the untested
  assumption and §8 question 1 is the reading that tests it. **THEY AGREE.**
- **§4 AGAINST §7**, because §4 keeps every existing divisor and §7 turns five of them into
  cross-checks. **THEY AGREE, AND §4 STATES THE DEPENDENCE EXPLICITLY** so a later session does
  not delete a divisor as redundant and silently delete the check with it.
- **§7 AGAINST §12**, because §7 calls the relations constructions and §12 requires the criterion
  to be non-vacuous. **THEY AGREE ONLY BECAUSE THE RELATIONS BREAK UNDER A MISPLACED INCREMENT,
  WHICH IS WHAT `P-RIDENT` IS FOR** — and §7 says plainly that a pass is evidence about placement
  and never about the sensor.
- **§8 AGAINST §9**, because §8 names what the first night answers and §9 names what no night
  answers. **THEY AGREE, AND THE MECHANISM QUESTION APPEARS ONLY IN §9.**
- **§6 AGAINST §11**, because both govern the capture: §6 adds two captures and the DIAG 5
  traversal rule, §11 states no pre-recording capture is needed. **THEY AGREE.**

**NO TENSION IS LEFT STANDING IN THIS FILE. A FROZEN FILE MAY NOT CARRY AN UNRESOLVED ITEM; IT IS
RESOLVED OR IT IS DROPPED BEFORE THE FILE IS FROZEN.**

**NO DEFECT WAS FOUND BY THIS CHECK IN THIS FILE.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**

**NOTHING IS IMPLEMENTED FROM THIS FILE UNTIL IT IS FROZEN AND PUSHED.**

**BOTH NEW SCREENS MUST BE VERIFIED ON THE WATCH BEFORE ANY NIGHT IS RECORDED UNDER THIS SPEC.
`CLASSIFIER_SERIES` AND `NIGHT_SUMMARY_VERSION` MUST BE VERIFIED UNCHANGED BY READING
`src/c/storage.h` AFTER IMPLEMENTATION.**

**THIS FILE DISCHARGES THE INSTRUMENT OWED BY `coarse-mixture-model-refutation-2026-08-27` §8.**
