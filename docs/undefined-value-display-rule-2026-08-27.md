# undefined-value-display-rule-2026-08-27

**Dated 2026-08-27. REGISTERS THE UNDEFINED-VALUE DISPLAY RULE IN ITS OWN RIGHT.**

**IT SUPERSEDES THE CITATION CARRIED BY FIFTEEN INSTANCES ACROSS BOTH REPOS AND `src/c/main.c`,
ENUMERATED IN §5. NO FROZEN FILE IS EDITED.** A change is a superseding dated file, never an edit.

**FILED IN `~/recovery/docs/` — THE PUBLIC REPO. RULE 7 CHECK PERFORMED IN §11: THIS FILE RECORDS
NO SUBJECT PHYSIOLOGICAL VALUE.**

**IT ANSWERS THE QUESTION `display-guard-citation-correction-2026-08-26` §7 ENTERED IN THE
DIAGNOSTIC QUEUE, AND IT RESOLVES THE TENSION THAT FILE'S §13 LEFT STANDING.** That file corrected
a section number and recorded, honestly, that the corrected citation was **better and still not
exact**. **THIS FILE MAKES IT EXACT.**

**THE CONVENTION ITSELF IS UNCHANGED AND HAS ALWAYS BEEN CORRECT.** No band moves. No threshold
moves. No guard keys on anything different. No counter is added, moved or removed. **NO NIGHT'S
READING CHANGES, NO GATE VERDICT ON ANY NIGHT CHANGES, AND NO PRE-REGISTRATION IS VOID** — no
value was ever rendered or scored according to a citation.

**IT AUTHORISES NO CODE EDIT OF ANY KIND. THE FOUR `main.c` COMMENTS ARE CORRECT AS THEY STAND**
— see §7.

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

## §3 — THE RULE

**A VALUE THAT WAS NOT MEASURED RENDERS `--`. IT NEVER RENDERS `0`.**

**A VALUE THAT WAS MEASURED AND CAME OUT ZERO RENDERS `0`. IT NEVER RENDERS `--`.**

**THESE ARE TWO DIFFERENT STATES AND NO SCREEN MAY COLLAPSE THEM.** Zero is a measured value and
carries information; **an absent value carries none, and rendering it as `0` manufactures a
measurement that was never taken.**

### 3.1 The four undefined cases this rule governs

**ALL FOUR RENDER `--`.** They are enumerated because §3.5 of `measurement-spec-v1` names only the
first, and the other three are the ones nearly every guard in this project actually keys on.

1. **THE PERSISTED RECORD PREDATES THE FIELD.** A `NightSummary` whose `version` is below the
   version that appended a field has that field absent. **This is `measurement-spec-v1` §3.5's own
   case and it is carried here unchanged.**
2. **NO SESSION HAS RUN IN THIS PROCESS.** A RAM-only session-scoped value whose session-start
   marker is unset was never accumulated. **The guards keying on `s_session_start` are this case.**
3. **A PRECONDITION OF THE VALUE IS UNDEFINED.** A value scoped to minutes at or after onset is
   undefined when onset is undefined, however long the session ran. **The guards keying on the
   onset index are this case.**
4. **A SENTINEL WAS NEVER LOWERED.** A minimum-tracking value initialised to a sentinel and never
   displaced was not measured. **`Dm` is this case, and `DM_SENTINEL` is what distinguishes it.**

### 3.2 What a rendered `0` therefore means

**A RENDERED `0` IS A REAL ZERO AND IS A FINDING.** It means the instrument ran and the count came
out zero — no event of that kind occurred. **THAT IS EVIDENCE AND IT IS READ AS EVIDENCE.**

**A RENDERED `0` WHERE CASE 4 APPLIES IS AN INSTRUMENT DEFECT.** A sentinel-tracked minimum cannot
legitimately read `0`; **if it does, the sentinel logic is wrong and the value is not a finding
about the sensor.**

### 3.3 What accompanies an undefined value

**A COUNT OR SCOPE FIGURE PRINTED ALONGSIDE AN UNDEFINED VALUE RENDERS ITS OWN TRUE VALUE.** It is
not suppressed to `--` merely because the value beside it is undefined. **`k` and `n` are this
case**, and rendering them truthfully is what makes an undefined reading diagnosable rather than
opaque.

### 3.4 What this rule does NOT decide

**IT DOES NOT DECIDE WHETHER A VALUE IS UNDEFINED.** Each instrument spec names its own guard and
what that guard keys on. **THIS RULE GOVERNS THE RENDERING ONCE THAT DETERMINATION IS MADE.**

**IT DOES NOT MAKE `--` A PASS OR A FAIL.** An undefined reading is NOT PERFORMED. **NOT PERFORMED
IS NEVER RECORDED AS A PASS**, per `physiological-plausibility-spec-v1` §2 and the same clause in
`awake-clause-counters-spec-v1-identity-correction-2026-08-18`.

**IT INTRODUCES NO CONSTANT, NO THRESHOLD AND NO BAND.**

## §4 — Why the rule needed its own file, and why a citation correction was not enough

**`measurement-spec-v1` §3.5 STATES THE RULE FOR ONE CASE AND ONE CASE ONLY: a persisted record
predating the version.** Read in full 2026-08-27. Its sentence governs fields treated as absent
because the record is old, and its justification — that zero is a meaningful measured value which
must not be confused with "not recorded" — is stated for that case.

**FOURTEEN OF THE FIFTEEN INSTANCES IN §5 GOVERN CASES 2, 3 OR 4, WHICH §3.5 DOES NOT MENTION.**
The principle transfers, and the project's practice has been correct throughout. **BUT THE
EXTENSION FROM THE PERSISTENCE CASE TO THE OTHERS IS ESTABLISHED PRACTICE AND IS NOT LICENSED BY
§3.5's OWN WORDS**, which `display-guard-citation-correction-2026-08-26` §7 states plainly and
declines to repair.

**A CITATION THAT IS CLOSE BUT NOT EXACT IS NOT GOOD ENOUGH FOR A RULE THAT FOURTEEN INSTRUMENTS
KEY ON.** That is the decision this file records.

**AND THERE IS A MECHANICAL REASON BEYOND EXACTNESS.** `display-guard-citation-correction-2026-08-26`
§6 establishes that `measurement-spec-v1` §3.6 is plausibly an **ATTRACTOR**: §3.6 contains the
word *convention* in a sentence about recorded-value conventions on screens, so a reader hunting
for the display convention lands there on the merits of §3.6's own text, without copying anyone.
**CORRECTING CITATIONS DOES NOT REMOVE AN ATTRACTOR. GIVING THE RULE ITS OWN FILE DOES**, by
supplying a target that is easier to find and unambiguously correct. **THAT FILE HAS NO STANDING
TO EDIT §3.6 AND NEITHER DOES THIS ONE.**

## §5 — THE FIFTEEN INSTANCES, ESTABLISHED BY SURVEY 2026-08-27

**READ BY GREP OVER `~/recovery` AND `~/recovery-nights` FOR EVERY OCCURRENCE OF
`measurement-spec-v1` ACROSS `.md`, `.c`, `.h`, `.py` AND `.csv`, WITH `.git` EXCLUDED. EVERY HIT
WAS THEN READ IN CONTEXT RATHER THAN CLASSIFIED FROM THE MATCHED LINE**, because the matched line
is precisely what misled every file below.

**THE TABLE IS NOT COPIED FROM `display-guard-citation-correction-2026-08-26` §4 AND THAT IS
DELIBERATE.** That file's own §9 and §10 record that an extent claim taken on faith is an
accumulation rather than a survey, and that taking one is the error it exists to correct. **A
THIRD REPETITION OF THAT ERROR IS AVOIDED BY RE-RUNNING THE SURVEY.** The result differs from that
file's table in two ways: `d787269` has since applied its authorised comment edits, so the `main.c`
sites now read §3.5; and two instances exist that did not exist then.

**GROUP A — DOCS STILL LITERALLY CITING §3.6. TEN INSTANCES.** Superseded by
`display-guard-citation-correction-2026-08-26` as to the section number, and superseded by THIS
FILE as to the target.

| File | Where |
|---|---|
| `classifier-spec-v3-anchor-readability-correction.md` | `AD` undefined |
| `awake-clause-counters-spec-v1-identity-correction-2026-08-18.md` | `AwO` undefined |
| `awake-anchor-readout-spec-v1.md` | `Ah` undefined, `k` renders true |
| `awake-runs-readout-spec-v1.md` | §4 |
| `awake-reference-ab-readout-spec-v1.md` | `w` undefined |
| `stillness-run-readout-spec-v1.md` | §4 |
| `hrv-cadence-readout-spec-v1.md` | §5 |
| `rsa-feasibility-readout-spec-v1.md` | §5 |
| `hrv-resolution-readout-spec-v1.md` | §5 |
| `N21-preregistration.md` (`~/recovery-nights`) | P-ANCHOR-BASELINE |

**GROUP B — `main.c` COMMENTS NOW CITING §3.5, CORRECTED BY `d787269`. FOUR INSTANCES.**
Superseded by this file as to the target only. **NO EDIT IS AUTHORISED — SEE §7.**

| Site |
|---|
| the `awake_post_onset` declaration comment |
| the stillness `Sr` render comment |
| the DIAG 4 header comment |
| the DIAG header comment, whose §3.6 clause about the diagnostic screen is CORRECT and stays |

**GROUP C — A DOC CITING §3.5 FROM BIRTH. ONE INSTANCE.**

| File | Where |
|---|---|
| `rejected-interval-divisor-counter-spec-v1.md` | §7, the `Drr` / `Drj` guard |

**IT IS NAMED RATHER THAN EXCLUDED, AND IT WAS FROZEN EARLIER TODAY BY THE SAME SESSION THAT
WROTE THIS FILE.** It cited §3.5, which was correct under the correction then in force and is
inexact under §4 above. **A SURVEY THAT OMITS THE SURVEYOR'S OWN FILE IS NOT A SURVEY.**

**NOT INSTANCES, READ AND CLASSIFIED RATHER THAN ASSUMED:**

- **`storage.h` twice and `storage.c` once** — §3.5 cited for PERSISTENCE: appended at v2 never
  inserted, the size requirement, accept both record sizes. **CORRECT, AND UNTOUCHED BY THIS
  FILE.** §3.5 remains the governing citation for the persistence mechanism.
- **`epoch-readout-spec-v1-nav-correction.md`** — §3.6 cited for button wiring being an
  implementation decision. **CORRECT.**
- **`coarse-adjacency-counter-spec-v1.md`** — its §334 and §336 DISCUSS the miscitation and its
  provenance; it carries no display-guard citation of its own. **NOT AN INSTANCE.**
- **`hrv_math.c`, `main.c` §3.2 and the feature-correction citation, `epoch-readout-spec-v1`,
  `epoch-readout-spec-v1-s2-correction`, `nights.csv`, `fields.py`, `C00-N16.md`, `C00-N17.md`,
  `N16-preregistration.md`, `N17-preregistration.md`** — cite §3.2, §3.3, §3.4, §5 or the file as
  a whole. **NONE CITES THE DISPLAY GUARD.**

## §6 — A SECOND DEFECT AT TWO SITES, NAMED SEPARATELY BECAUSE IT IS NOT A CITATION ERROR

**TWO INSTANCES CARRY A JUSTIFICATION THAT RUNS BACKWARDS.**

`N21-preregistration.md` states *0 is UNDEFINED, not measured*. `classifier-spec-v3-anchor-readability-correction.md`
states *0 is UNDEFINED and not measured*. **BOTH ARE THE INVERSE OF THE RULE.** §3.5 and §3 above
say that zero **IS** a measured value and precisely for that reason must not stand in for an
absent one. **THE TWO SITES REACH THE RIGHT BEHAVIOUR THROUGH A REASON THAT IS THE OPPOSITE OF THE
REAL ONE.**

**THE PRACTICAL CONSEQUENCE IS NOT ZERO.** A reader who takes *0 is undefined* literally would
render a defined zero as `--`, **which §3.2 above makes an error in the other direction** — it
would suppress a finding.

**NEITHER FILE IS EDITED AND NEITHER READING IS RESCORED.** `N21`'s P-ANCHOR-BASELINE reading
stands exactly as recorded, and no anchor value on any night is affected. **THE JUSTIFICATION IS
SUPERSEDED BY §3 AND §3.2 OF THIS FILE; THE BEHAVIOUR IT PRODUCED WAS CORRECT THROUGHOUT.**

**IT IS NAMED HERE BECAUSE `display-guard-citation-correction-2026-08-26` §6 NOTICED THE WORDING
AT ONE OF THE TWO SITES AND USED IT ONLY AS EVIDENCE ABOUT PROVENANCE**, not as a defect in its
own right. **A SENTENCE THAT WOULD MISLEAD A FUTURE IMPLEMENTER IS A DEFECT WHETHER OR NOT IT
BEARS ON WHO COPIED WHOM.**

## §7 — WHAT IS NOT DONE, AND WHY NO CODE IS TOUCHED

**NO CODE EDIT IS AUTHORISED BY THIS FILE. NOT ONE COMMENT, NOT ONE STATEMENT.**

**THE FOUR `main.c` COMMENTS IN GROUP B ARE CORRECT AS THEY STAND.** They cite §3.5 for the
display guard, which is true — §3.5 does state the rule, for one case. **WHAT CHANGES IS THAT A
BETTER TARGET NOW EXISTS, AND THAT IS NOT A DEFECT IN THE COMMENTS.**

**EDITING THEM WOULD MEAN TOUCHING A COMMENT BLOCK TWICE IN TWO DAYS TO CHANGE A REFERENCE THAT
WAS NOT WRONG EITHER TIME.** `display-guard-citation-correction-2026-08-26` §8 records the shape
worth avoiding: **a block touched at a known date reads to a later reviewer as vetted.** Touching
them again for a non-defect spends that signal.

**A FUTURE SPEC CITES THIS FILE. A COMMENT ALREADY CITING §3.5 IS LEFT ALONE UNTIL SOME OTHER
DATED FILE HAS ITS OWN REASON TO TOUCH IT**, at which point the reference may be updated in the
same commit. **NO SESSION CREATES A COMMIT SOLELY TO UPDATE THESE REFERENCES.**

**NO FROZEN DOC IS EDITED AND NO PRE-REGISTRATION IS EDITED.** They are superseded by this file
and corrected by reading it alongside them.

**§3.5 IS NOT WIDENED, NOT REINTERPRETED AND NOT DEPRECATED.** It remains the governing citation
for the persistence mechanism, exactly as `storage.h` and `storage.c` cite it. **THIS FILE TAKES
OVER ONE RULE AND LEAVES §3.5's OTHER CONTENT ALONE.**

**`measurement-spec-v1` §3.6 IS NOT EDITED AND CANNOT BE.** It is frozen, its layout content is
correct, and the attractor in its wording is not removable by any file this project may write.
**WHAT §4 CLAIMS IS THAT A CORRECT AND EASIER TARGET REDUCES THE ATTRACTOR'S PULL, NOT THAT IT
ELIMINATES IT.**

## §8 — Rule 2 check

**NO CONSTANT ENTERS ANY CODE PATH. NO BAND IS REGISTERED ON ANYTHING. NO THRESHOLD IS NAMED.**
**NOTHING IN THIS FILE IS READ BY THE APP.**

## §9 — Rule 3 check

**THIS FILE REGISTERS NO SCORED CRITERION, SO THERE IS NOTHING TO CHECK FOR VACUITY — AND THAT IS
STATED RATHER THAN THE CHECK BEING SKIPPED.**

**ITS CLAIMS COULD EACH HAVE GONE THE OTHER WAY.** Had `measurement-spec-v1` §3.5 stated the rule
for undefined sessions as well as absent records, this file would not exist; **the full read of
that section was that test.** Had the survey returned only the correction's thirteen, §5 would say
so; **it returned fifteen, in three distinct surface states, and one of them is this session's own
file.** Had `coarse-adjacency-counter-spec-v1` carried its own display-guard citation it would be
in Group A; **it was read in context and it does not.**

**CHECKED AGAINST THE THIRD VACUITY FORM — A FINDING THAT MEANS SOMETHING ELSE. THREE ARE
REGISTERED:**

1. **THIS FILE READ AS CHANGING THE CONVENTION.** It does not. The header says so and §7 repeats
   it. **EVERY GUARD KEYS ON WHAT IT ALREADY KEYED ON.**
2. **THIS FILE READ AS DEPRECATING `measurement-spec-v1` §3.5.** §7 forbids it. §3.5 governs
   persistence and is cited correctly in three places that this file does not touch.
3. **§6 READ AS INVALIDATING `N21`'s READING OR ANY ANCHOR VALUE.** §6 forbids it. **THE
   BEHAVIOUR WAS CORRECT; ONLY THE STATED REASON WAS BACKWARDS.**

## §10 — How this survived, and what it is an instance of

**THE CONVENTION WAS CORRECT FROM THE FIRST DAY AND THE RULE BEHIND IT WAS NEVER WRITTEN DOWN AS
A RULE.** It lived as a sentence inside a persistence section, and every instrument that needed it
reached for the nearest plausible citation. **THIRTEEN REACHED FOR THE WRONG SECTION AND TWO
REACHED FOR THE RIGHT ONE INEXACTLY.**

**THAT IS A DIFFERENT DEFECT FROM A MISCITATION AND IT IS THE ONE THIS FILE ADDRESSES.** A
miscitation is corrected by fixing a number. **A RULE WITH NO HOME IS CORRECTED BY GIVING IT
ONE.**

**REGISTERED AS A STANDING CONSEQUENCE: WHERE A CONVENTION IS RELIED ON BY MORE THAN ONE
INSTRUMENT, IT IS REGISTERED IN ITS OWN DATED FILE RATHER THAN CITED OUT OF A SECTION WRITTEN FOR
A DIFFERENT PURPOSE.** **A CONVENTION CITED FROM A SECTION THAT DOES NOT GOVERN IT WILL BE
MISCITED, AND CORRECTING THE CITATIONS DOES NOT PREVENT THE NEXT ONE.**

**AND THE SECOND STANDING CONSEQUENCE, INHERITED AND RE-EARNED:** **AN EXTENT CLAIM IS A SURVEY
RESULT AND IS NEVER AN ACCUMULATION — INCLUDING WHEN THE ACCUMULATION IS ANOTHER FILE'S SURVEY
TAKEN ON FAITH.** §5 re-ran the survey for that reason and found two instances the earlier table
could not have contained.

## §11 — Rule 7 check

**NO SUBJECT PHYSIOLOGICAL VALUE APPEARS IN THIS FILE.** No night's reading, no counter value, no
ratio, and nothing from which one could be derived. **THE INSTRUMENT AND FILE NAMES ARE NOT
VALUES.** **SCAN BEFORE THE PUBLIC COMMIT REGARDLESS; THIS SECTION IS NOT A SUBSTITUTE FOR IT.**

## §12 — Self-consistency check, per RULE 19

**PERFORMED BEFORE THIS FILE IS FROZEN AND RECORDED HERE AS A NAMED SECTION. A FILE THAT DOES NOT
RECORD THE CHECK HAS NOT HAD IT.**

**FORM 1, ONE QUANTITY DESCRIBED TWICE.** The instance count is stated as fifteen in the header,
in §5's heading and in §10, and §5's three groups hold ten, four and one, which sum to fifteen.
The four undefined cases are enumerated in §3.1 and referred to by number in §3.2 and §4; the
references are consistent. §3.5's scope is described in §4 and §7 and reads the same in both.

**FORM 2, QUANTITATIVE AGAINST PROSE, SUBSTITUTED AT BOTH EXTREMES.** The only arithmetic is the
instance count, checked above. **At the low extreme — an instrument with no undefined case
reachable:** the rule never fires, every value renders a real number, and §3.2's real-zero clause
is the only one that ever applies; no statement here fails. **At the high extreme — a process in
which no session ever ran:** every session-scoped value on every screen renders `--`, §3.3 still
requires accompanying counts to render truthfully, and §3.4's NOT PERFORMED clause covers every
reading; **no statement here fails, and in particular §3.2 does not misfire, because no value was
measured.**

**FORM 3, EVERY BRANCH REACHABLE.** Checked in §9.

**FORM 4, SECTIONS SERVING DIFFERENT READERS, PAIRED BY SHARED QUANTITY.** §3 serves whoever
implements or reads a screen; §4 serves whoever asks why the file exists; §5 serves whoever scores
a night against any listed file; §6 serves whoever reads the two backwards justifications; §7
serves whoever is tempted to open an editor.

**THE PAIRINGS ACTUALLY RUN:**

- **§3 AGAINST §6**, because both bear on what zero means: §3.2 says a rendered zero is a finding
  and §6 records two files saying zero is undefined. **THEY AGREE ONLY BECAUSE §6 STATES THE
  SUPERSESSION EXPLICITLY** — without it, a reader holding both would have two live and opposite
  rules.
- **§4 AGAINST §7**, because §4 argues the citation was not exact enough while §7 forbids editing
  the comments that carry it. **THEY AGREE: THE FIX IS A BETTER TARGET, NOT A CHANGED COMMENT**,
  and §7 states the reason rather than leaving it as an omission.
- **§5 AGAINST §10**, because §5 counts fifteen and §10 splits them as thirteen wrong-section and
  two inexact. **THEY AGREE, AND THE SPLIT IS STATED SO THE TWO NUMBERS ARE NOT READ AS
  DISAGREEING.**
- **§5 AGAINST §9**, because §5's survey is what §9's falsifiability rests on. **THEY AGREE, AND
  §5 NAMES ITS METHOD AND ITS EXCLUSIONS SO THE RESULT'S REACH IS READABLE.**
- **§6 AGAINST §7**, because §6 names a defect in two frozen files and §7 edits nothing. **THEY
  AGREE: SUPERSESSION IS THE MECHANISM AND EDITING IS NOT.**

**NO TENSION IS LEFT STANDING IN THIS FILE.** The tension
`display-guard-citation-correction-2026-08-26` §13 recorded — that the corrected citation was
better and still not exact — **IS RESOLVED BY §3 AND §4 RATHER THAN CARRIED FORWARD.** **A FROZEN
FILE MAY NOT CARRY AN UNRESOLVED ITEM; IT IS RESOLVED OR IT IS DROPPED BEFORE THE FILE IS
FROZEN.**

**NO DEFECT WAS FOUND BY THIS CHECK IN THIS FILE.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**

**THE QUESTION `display-guard-citation-correction-2026-08-26` §7 ENTERED IN THE DIAGNOSTIC QUEUE
IS ANSWERED AND THAT ITEM IS CLOSED. NO NEW ITEM IS ENTERED BY THIS FILE.**

**FUTURE SPECS CITE THIS FILE FOR THE DISPLAY GUARD, AND `measurement-spec-v1` §3.5 FOR THE
PERSISTENCE MECHANISM.**
