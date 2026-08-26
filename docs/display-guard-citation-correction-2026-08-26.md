# display-guard-citation-correction-2026-08-26

**Corrects a CITATION carried by NINE FROZEN SPEC AND CORRECTION FILES, ONE PRE-REGISTRATION AND
THREE COMMENT SITES IN `main.c`. NO FROZEN FILE IS EDITED.** A change is a superseding dated
file, never an edit.

**IT ALSO SUPERSEDES THE EXTENT CLAIM IN `coarse-adjacency-counter-spec-v1` §6 (`24bd38a`), WHICH
STATES THE DEFECT APPEARS IN TWO SPECS. IT APPEARS IN THIRTEEN PLACES. THAT FILE IS FROZEN AND IS
NOT EDITED — SEE §9.**

**RULE 7: this file records no subject physiological value.**

**IT CORRECTS A SECTION NUMBER AND NOTHING ELSE. THE CONVENTION ITSELF IS CORRECT, HAS ALWAYS
BEEN CORRECT, AND IS NOT ALTERED IN ANY WAY.** No band moves. No threshold moves. No counter is
added, moved or removed. **NO NIGHT'S READING CHANGES AND NO GATE VERDICT ON ANY NIGHT CHANGES**,
because no value was ever rendered or scored according to a section number.

**IT AUTHORISES COMMENT-ONLY EDITS AT FOUR SITES IN `main.c`, SPECIFIED IN §8.** No executable
statement is touched.

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

## §3 — What is wrong

**THE DISPLAY-GUARD RULE — THAT AN UNDEFINED VALUE RENDERS `--` AND NEVER `0` — IS CITED TO
`measurement-spec-v1` §3.6 THROUGHOUT THE PROJECT. IT IS IN §3.5.**

**WHAT §3.6 ACTUALLY CONTAINS, READ IN FULL 2026-08-26:** a display-layout section for the
ORIGINAL DIAG screen, giving a suggested one-screen layout of seven movement values — `Vmax`,
`P90`, `Vmed`, `Vn`, `Gate`, `Bmin`, `Bmax` — a statement that RESULTS is unchanged and every
existing recorded-value convention still applies, and a statement that the button wiring is an
implementation decision recorded in the commit message rather than fixed in the spec. **IT
CONTAINS NO RULE ABOUT `--` VERSUS `0` AND NO RULE ABOUT UNDEFINED VALUES AT ALL.**

**WHAT §3.5 CONTAINS, AND IT IS THE RULE:**

> A record whose version is below 2 has its seven new fields treated as absent and **displayed as
> `--`, never as 0** - zero is a meaningful measured value here and must not be confused with "not
> recorded".

## §4 — THE FULL EXTENT, ESTABLISHED BY A SURVEY OF BOTH REPOS

**READ 2026-08-26 BY GREP OVER `~/recovery` AND `~/recovery-nights`, EXCLUDING `.git`, FOR EVERY
OCCURRENCE OF `measurement-spec-v1`. EVERY HIT WAS THEN READ IN CONTEXT RATHER THAN CLASSIFIED
FROM THE MATCHED LINE**, because the matched line is precisely what misled every file below.

**WRONG — THE DISPLAY-GUARD RULE CITED TO §3.6. THIRTEEN INSTANCES.**

| File | Where | Frozen |
|---|---|---|
| `classifier-spec-v3-anchor-readability-correction.md` | §3 | `d4c12f6` 2026-08-14 |
| `awake-clause-counters-spec-v1-identity-correction-2026-08-18.md` | `AwO` undefined | `e8fe888` 2026-08-18 |
| `awake-anchor-readout-spec-v1.md` | `Ah` undefined | `ed504f3` 2026-08-19 |
| `awake-runs-readout-spec-v1.md` | §4 | `7e1a0b4` 2026-08-19 |
| `awake-reference-ab-readout-spec-v1.md` | `w<whole>` undefined | `6ee15aa` 2026-08-19 |
| `stillness-run-readout-spec-v1.md` | §4 | `1adf80d` 2026-08-20 |
| `hrv-cadence-readout-spec-v1.md` | §5 | `2d0cc15` 2026-08-20 |
| `rsa-feasibility-readout-spec-v1.md` | §5 | `511e54a` 2026-08-22 |
| `hrv-resolution-readout-spec-v1.md` | §5 | `7a2f58d` 2026-08-22 |
| `N21-preregistration.md` | P-ANCHOR-BASELINE | in `~/recovery-nights` |
| `src/c/main.c` | `awake_post_onset` declaration | `b304e1f` 2026-08-18 |
| `src/c/main.c` | stillness `Sr` render | `042a98a` 2026-08-20 |
| `src/c/main.c` | DIAG 4 header | `6439950` 2026-08-22 |

**RIGHT, AND NOT TOUCHED BY THIS FILE — §3.6 CITED FOR WHAT §3.6 ACTUALLY SAYS:**

- **`epoch-readout-spec-v1-nav-correction.md`** (`dff61ec`, 2026-08-10) — cites §3.6 for button
  wiring being an implementation decision. **CORRECT, AND IT IS THE EARLIEST DOC CITATION OF §3.6
  IN THE PROJECT — SEE §6.**
- **`src/c/main.c`, the DIAG header comment** — cites §3.6 for DIAG being a diagnostic screen with
  RESULTS unchanged. **CORRECT IN PART — SEE §5.**

**RIGHT, AND EVIDENCE THAT THE SECTION NUMBER IS NOT GENERALLY MISUNDERSTOOD — §3.5 CITED
CORRECTLY WHEREVER PERSISTENCE IS THE SUBJECT:**

- **`src/c/storage.h`** — appended at v2, never inserted.
- **`src/c/storage.h`** — the size requirement.
- **`src/c/storage.c`** — accept BOTH record sizes.

**THAT SPLIT IS THE FINDING AND IT NARROWS THE DEFECT.** **THE PROJECT UNDERSTANDS §3.5 AND §3.6
CORRECTLY WHEREVER IT REASONS ABOUT PERSISTENCE OR ABOUT SCREEN LAYOUT.** The error is confined
to one rule — the display guard — and it is a citation error rather than a conceptual one.

**OTHER CITATIONS OF `measurement-spec-v1` WERE READ AND ARE UNAFFECTED:** §3.2 in `hrv_math.c`
and `main.c`, the feature correction §2 in `main.c`, §3.3 in `epoch-readout-spec-v1-s2-correction`,
§3.4 and §5 in `C00-N16.md` and `N16-preregistration.md`, and the whole-file references in
`epoch-readout-spec-v1`, `nights.csv`, `fields.py`, `C00-N17.md` and `N17-preregistration.md`.
**NONE CITES THE DISPLAY GUARD AND NONE IS CORRECTED.**

## §5 — ONE INSTANCE RESISTS CLASSIFICATION AND IS RECORDED AS MIXED RATHER THAN FORCED

**THE DIAG HEADER COMMENT IN `main.c` CARRIES ONE CITATION COVERING TWO SECTIONS AND IS RIGHT
ABOUT ONE OF THEM.**

It cites §3.6, then states that DIAG is a diagnostic screen and RESULTS is unchanged — **which is
§3.6 and is correct** — and immediately continues that a v1 record has no measured tail so it
prints `--` rather than `0`, **which is §3.5 and is the miscited rule.**

**THE MECHANISM IS ESTABLISHED BY MATCHING EACH CLAUSE AGAINST BOTH SECTIONS, NOT BY INFERENCE
FROM DATES.** §3.6 opens by stating the values are shown on a new DIAG screen rather than crowded
onto RESULTS, and that RESULTS is unchanged. **THE COMMENT'S FIRST TWO CLAUSES PARAPHRASE THAT
ALMOST DIRECTLY.** §3.5 ends with the words *zero is a meaningful measured value here and must not
be confused with "not recorded"*. **THE COMMENT'S CLOSING PHRASE — `zero is a meaningful measured
value here` — IS THAT SENTENCE, LIFTED NEARLY WORD FOR WORD.**

**SO THE CITATION WAS CORRECT WHEN WRITTEN AND THEN SILENTLY ANNEXED THE SENTENCE THAT FOLLOWED
IT.** **NOBODY MIS-NUMBERED ANYTHING AT THIS SITE.** **THAT IS A DIFFERENT DEFECT FROM A WRONG
SECTION NUMBER AND IT IS NAMED SEPARATELY: A CITATION WHOSE SCOPE QUIETLY GREW PAST WHAT IT
COVERS.**

**IT IS NOT COUNTED AMONG THE THIRTEEN AND IT IS NOT COUNTED AS CORRECT.** **A COMMENT THAT IS
RIGHT ABOUT ITS FIRST CLAUSE AND WRONG ABOUT ITS SECOND IS NEITHER, AND FORCING IT INTO EITHER
COLUMN WOULD MISSTATE THE RECORD.** §8 specifies how it is repaired, and it is repaired by
splitting rather than by swapping **because §5 establishes there is a correct citation in it worth
keeping.**

## §6 — Chronology, and what is and is not established about origin

**ESTABLISHED BY READ, USING `git log --diff-filter=A` FOR THE DOCS — WHICH ARE FROZEN ON COMMIT
AND NEVER EDITED, SO THE ADDING COMMIT IS THE FREEZE — AND `git log -S` OVER `main.c` AND OVER
THE WHOLE TREE ACROSS ALL HISTORY.**

**NOTHING WAS LOST TO A DELETED OR RENAMED FILE, AND THAT IS TESTED RATHER THAN ASSUMED.** The
pickaxe over the whole tree, unrestricted by path, returns the same four `main.c` commits and no
others. **THE CLAIM THAT NO INSTANCE PREDATES `cc305dd` IS A SEARCHED RESULT.**

**THE EARLIEST OCCURRENCE ANYWHERE IS `cc305dd`, 2026-08-09, IN `main.c`** — the commit that
implemented `measurement-spec-v1` itself. **THAT IS THE MIXED INSTANCE IN §5, AND §5 ESTABLISHES
IT IS AN ANNEXED CITATION RATHER THAN A WRONG NUMBER.**

**THE EARLIEST DOC CITATION OF §3.6 IS `dff61ec`, 2026-08-10, AND IT IS CORRECT** — the RUNS
navigation correction, citing §3.6 for button wiring being an implementation decision. **THE
PROJECT'S FIRST DOC-LEVEL USE OF §3.6 WAS RIGHT AND WAS ABOUT IMPLEMENTATION DECISIONS ON
SCREENS.**

**THE EARLIEST WRONG DOC INSTANCE IS `d4c12f6`, 2026-08-14**, in
`classifier-spec-v3-anchor-readability-correction.md`, four days after the correct one.

**WHAT THE EVIDENCE ABOUT `d4c12f6` ACTUALLY SUPPORTS, STATED CAREFULLY BECAUSE AN EARLIER DRAFT
OF THIS SECTION OVERSTATED IT.** Its wording is *0 is UNDEFINED and not measured - the convention
`measurement-spec-v1` section 3.6 already established.* **"ALREADY ESTABLISHED" POINTS BACKWARD.
BUT THE PHRASE `0 IS UNDEFINED AND NOT MEASURED` MATCHES NEITHER THE `main.c` COMMENT NOR §3.5
CLOSELY** — both of those run the other way round, saying zero IS a meaningful measured value and
so must not stand in for an absent one. **IT IS A PARAPHRASE OF THE IDEA AND NOT A COPY OF EITHER
TEXT.**

**AND §3.6's OWN WORDING PLAUSIBLY ATTRACTS THE CITATION WITHOUT ANY COPYING AT ALL.** §3.6
contains the sentence that RESULTS is unchanged **so every existing recorded-value convention and
every prior night's screen-reading procedure still applies.** **THE WORD `CONVENTION` IS IN §3.6,
IN A SENTENCE ABOUT RECORDED-VALUE CONVENTIONS ON SCREENS — AND `d4c12f6` CALLS WHAT IT IS CITING
`THE CONVENTION`.** **A READER LOOKING FOR "THE DISPLAY CONVENTION" LANDS ON §3.6 ON THE MERITS OF
§3.6's OWN TEXT.**

**SO THERE ARE TWO LIVE ACCOUNTS AND THIS FILE ADOPTS NEITHER:**

1. **PROPAGATION** — one instance was copied forward, directly or at one remove.
2. **INDEPENDENT LANDING** — separate authors each reached §3.6 unaided, because §3.6 is the
   section that talks about screens and conventions and §3.5 is filed under persistence.

**NEITHER IS ESTABLISHED AND NEITHER IS GUESSED. CHRONOLOGY ORDERS THE INSTANCES AND DOES NOT
DEMONSTRATE DERIVATION.** **THE SECOND ACCOUNT IS NOW AT LEAST AS WELL SUPPORTED AS THE FIRST,
WHICH AN EARLIER DRAFT DID NOT CONSIDER AT ALL.**

**IT MATTERS FOR ONE PRACTICAL REASON AND IS RECORDED FOR THAT REASON ALONE:** **IF INSTANCES
AROSE INDEPENDENTLY, CORRECTING THEM DOES NOT PREVENT THE NEXT ONE, BECAUSE THE ATTRACTOR IS
§3.6's OWN TEXT AND THIS FILE HAS NO STANDING TO EDIT IT.** **THAT STRENGTHENS THE CASE FOR THE
QUEUED QUESTION IN §7 — WHETHER THE UNDEFINED-VALUE CONVENTION SHOULD HAVE ITS OWN REGISTERED
RULE RATHER THAN LIVING AS AN EXTENSION OF A PERSISTENCE RULE NOBODY LOOKING FOR A DISPLAY RULE
WOULD OPEN.**

**AN EARLIER DRAFT OF THIS CORRECTION NAMED `hrv-cadence-readout-spec-v1` (2026-08-20) AS THE
EARLIEST SPEC INSTANCE. IT IS SEVENTH OF NINE.** That draft surveyed only the files that session
had happened to read and presented the result as an extent. **THAT IS THE SAME ERROR THIS FILE
CORRECTS, COMMITTED WHILE WRITING THE FILE THAT CORRECTS IT**, and it is recorded here rather
than quietly replaced.

## §7 — What is corrected, and what is emphatically not

**CORRECTED: THE SECTION NUMBER. IN EVERY INSTANCE IN §4's FIRST TABLE, `measurement-spec-v1`
§3.6 READS `measurement-spec-v1` §3.5.**

**NOT CORRECTED, BECAUSE NOTHING ELSE IS WRONG:**

- **THE CONVENTION IS UNCHANGED.** `--` for undefined, never `0`. Every guard keys on what it
  already keyed on.
- **EVERY OTHER SENTENCE IN EVERY LISTED FILE STANDS.** In particular: that a defined `He`, `D8`,
  `Gp` or `SrN` of 0 is a REAL ZERO and a FINDING; that `Dm` prints `--` when never lowered from
  its sentinel and a rendered `Dm 0` is an instrument defect; that `AwO`'s undefined case is
  recorded NOT PERFORMED rather than as a passing identity; and that `k` and `n` render their
  true values even when the value they accompany is undefined.
- **NO NIGHT IS RESCORED AND NO PRE-REGISTRATION IS VOID.** `N21`'s P-ANCHOR-BASELINE reading
  stands exactly as recorded.

**A SCOPE DIFFERENCE IS REGISTERED RATHER THAN GLOSSED, BECAUSE THE CORRECTED CITATION IS STILL
NOT AN EXACT FIT.** §3.5's rule governs fields **ABSENT BECAUSE THE PERSISTED RECORD PREDATES THE
VERSION.** Most of the guards above govern a **SESSION THAT HAS NOT RUN**, or an onset that is
undefined. **THE UNDERLYING PRINCIPLE IS THE SAME — AN ABSENT VALUE MUST NOT RENDER AS A MEASURED
ZERO — BUT THE EXTENSION FROM ONE CASE TO THE OTHERS IS ESTABLISHED PRACTICE AND IS NOT LICENSED
BY §3.5's OWN WORDS.**

**THIS FILE DOES NOT REPAIR THAT.** It corrects a number that is simply wrong. **WHETHER THE
UNDEFINED-VALUE CONVENTION DESERVES ITS OWN REGISTERED RULE RATHER THAN AN EXTENSION OF A
PERSISTENCE RULE IS A SEPARATE QUESTION AND IS ENTERED IN THE DIAGNOSTIC QUEUE.** **A file
written to correct one citation stays answerable by not absorbing a second question**, per
`hrv-resolution-readout-spec-v1` §11.

## §8 — The `main.c` edits this file authorises

**COMMENT-ONLY, AT FOUR SITES. NO EXECUTABLE STATEMENT IS TOUCHED. NO BEHAVIOUR CHANGES.**

1. **The `awake_post_onset` declaration comment** — `measurement-spec-v1 s3.6` reads `s3.5`.
2. **The stillness `Sr` render comment** — `measurement-spec-v1 s3.6` reads `s3.5`.
3. **The DIAG 4 header comment** — `measurement-spec-v1 s3.6` reads `s3.5`.
4. **The DIAG header comment, the mixed instance of §5** — the §3.6 citation STAYS on the
   diagnostic-screen clause where it is correct, and the `--`-rather-than-`0` clause gains its own
   reference to §3.5. **THIS IS THE ONE SITE WHERE A NUMBER IS NOT SIMPLY SWAPPED**, and it is
   specified separately for that reason.

**ALL FOUR ARE APPLIED IN ONE COMMIT, TOGETHER WITH THE SEVEN-TO-TEN COUNT CORRECTION AUTHORISED
BY `coarse-adjacency-counter-spec-v1` §6.1.** **EDITING A COMMENT WHILE KNOWINGLY LEAVING A
SECOND DEFECT IN IT WOULD PRODUCE A BLOCK THAT HAS BEEN TOUCHED AT A KNOWN DATE AND STILL CARRIES
A KNOWN ERROR** — which a later reader would reasonably take as vetted. **THAT IS THE
STALE-ARTEFACT-WEARING-A-CURRENT-DATE SHAPE THIS PROJECT KEEPS FINDING, AND IT IS NOT
MANUFACTURED ON PURPOSE.**

**NO FROZEN DOC IS EDITED AND NO PRE-REGISTRATION IS EDITED.** Those are superseded by this file
and corrected by reading it alongside them.

**NO OTHER COMMENT IN `main.c` IS EDITED BY THIS FILE.** The other wrong comments in that file
stand for their own dated files.

## §9 — What this supersedes in an already-frozen file

**`coarse-adjacency-counter-spec-v1` (`24bd38a`) §6 STATES THAT THE MISCITATION APPEARS IN TWO
INDEPENDENTLY FROZEN SPECS AND THAT THE CHAIN STOPS AT THE `main.c` DIAG 4 COMMENT. BOTH
STATEMENTS ARE WRONG AND ARE SUPERSEDED BY §4 AND §6 ABOVE.**

**THAT FILE IS FROZEN AND PUSHED AND IS NOT EDITED.** Its §6.1 count correction, its §6.2 watch
and capture requirements, and every other section stand unaffected — **the extent claim was
incidental to that file's purpose and load-bearing for none of its requirements.**

**IT WAS WRONG FOR A REASON WORTH CARRYING: IT SURVEYED THE FILES THAT SESSION HAD READ AND
REPORTED THE RESULT AS AN EXTENT.** **AN EXTENT CLAIM REQUIRES A SURVEY AND NOT AN ACCUMULATION.**

## §10 — How this survived, and what it is an instance of

**THE CONVENTION WAS CARRIED FAITHFULLY AND THE CITATION ATTACHED TO IT WAS NEVER CHECKED AGAINST
THE CITED FILE.** **RULE 4's SECOND CLAUSE — RE-DERIVE THE PREMISE, NOT ONLY THE EVIDENCE.** It
is the same shape as the `rej_quality` defect, which
`resolution-spec-s11-correction-2026-08-23` records as a correct read of `hrv_math.c` whose
premise — who calls it, and with what — was never read.

**IT WOULD NOT HAVE BEEN CAUGHT BY THE STANDING SELF-CHECK, AND THAT IS THE PART WORTH
CARRYING.** `resolution-extension-s6-correction-2026-08-24` §8 registers four forms and **ALL
FOUR ARE WITHIN-FILE.** Every file listed in §4 is internally consistent: the wrong section
number agrees with itself everywhere it appears, and no sentence in any of them contradicts any
other. **A CROSS-FILE CITATION DEFECT IS INVISIBLE TO A WITHIN-FILE CHECK. ONLY READING THE CITED
FILE REACHES IT.**

**THAT IS RULE 21 AND NOT RULE 19, AND THE TWO ARE NOT SUBSTITUTES.** **REGISTERED AS A STANDING
CONSEQUENCE: WHERE A FILE JUSTIFIES A DECISION BY CITING A SECTION OF ANOTHER FILE, THAT SECTION
IS READ BEFORE THE CITING FILE IS FROZEN.** A self-consistency check verifies that a file agrees
with itself. **IT DOES NOT VERIFY THAT THE FILE AGREES WITH WHAT IT CITES.**

**AND THE NON-RETROACTIVITY CLAUSE IS WHY NONE OF THEM WAS RE-AUDITED.** §8 applies from
2026-08-24 forward and explicitly not retroactively; **every file in §4's table predates it.**
That same §8 states the remedy for exactly this case: **if a later reading exposes a defect in one
of them, it is corrected then, by the same mechanism. THIS FILE IS THAT MECHANISM.**

**A SECOND STANDING CONSEQUENCE, FROM §9 AND FROM THE DRAFT ERROR IN §6:** **A CLAIM ABOUT HOW
MANY PLACES SOMETHING APPEARS IS A SURVEY RESULT AND IS NEVER AN ACCUMULATION OF WHAT HAS BEEN
READ SO FAR.** **AN EMPTY GREP IS NOT ABSENCE — RULE 1 — AND A NON-EMPTY GREP OVER PART OF THE
TREE IS NOT AN EXTENT.**

## §11 — Rule 2 check

**NO CONSTANT ENTERS ANY CODE PATH. NO BAND IS REGISTERED ON ANYTHING. NOTHING IN THIS FILE IS
READ BY THE APP** except comments, which the compiler discards.

## §12 — Rule 3 check

**THIS FILE REGISTERS NO CRITERION AND SCORES NOTHING, SO THERE IS NOTHING TO CHECK FOR VACUITY —
AND THAT IS STATED RATHER THAN THE CHECK BEING SKIPPED.**

**THE CLAIMS IT MAKES COULD EACH HAVE GONE THE OTHER WAY.** Had §3.6 contained the display-guard
rule, this file would not exist. **THE FULL READ OF `measurement-spec-v1` WAS THAT TEST.** Had the
survey returned two instances, §4's table would have two rows; **it returned thirteen, and the
draft that assumed two is recorded in §6 as wrong.** Had `git log -S` placed the earliest
instance in a spec rather than in `main.c`, §6 would read the other way.

**A GREP FOR THE SECTION NUMBER ALONE WOULD HAVE RETURNED §3.6's HEADING AND SETTLED NOTHING —
RULE 1 — WHICH IS WHY THE FILE WAS READ IN FULL AND WHY EVERY SURVEY HIT WAS READ IN CONTEXT.**

## §13 — Self-consistency check, per RULE 19 — the standing rule registered in `resolution-extension-s6-correction-2026-08-24` §8

**THE RECORD CARRIES TWO LABELS FOR THIS CHECK AND BOTH ARE NAMED SO NEITHER READS AS A DIFFERENT
RULE.** `rsa-feasibility-exit-2026-08-25` calls it RULE 19;
`hrv-coincidence-null-mixture-rederivation-2026-08-24` §10 calls it *RULE 18 and §8 of the 08-24
correction*. **THEY ARE THE SAME FOUR-FORM CHECK.**

**FORM 1, ONE QUANTITY DESCRIBED TWICE.** The count of wrong instances is stated in the header as
nine specs plus one pre-registration plus three comment sites, and in §4 as a thirteen-row table;
**the table has thirteen rows and nine of them are spec or correction files.** **THE MIXED
INSTANCE IS EXCLUDED FROM BOTH STATEMENTS AND §5 SAYS SO EXPLICITLY**, which is the statement that
would otherwise make the two disagree. The wrong section and the right one appear in §3, §4, §7
and §8 and agree in all four.

**FORM 2, QUANTITATIVE AGAINST PROSE.** The only quantitative claims are counts and dates, checked
above against the table. **There is no arithmetic in this file to substitute at extremes, and that
is recorded as the form being inapplicable rather than as it having passed.**

**FORM 3, EVERY BRANCH REACHABLE.** Checked in §12.

**FORM 4, SECTIONS SERVING DIFFERENT READERS.** §3, §4 and §7 serve whoever scores a night against
any listed file; §6 serves whoever asks where it started; §8 serves whoever applies the edits; §9
serves whoever reads the frozen `coarse-adjacency-counter-spec-v1` and finds it disagrees with
this file.

**THE PAIRINGS ACTUALLY RUN:**

- **§7 AGAINST §8**, because §7 says the convention is unchanged while §8 authorises code edits,
  and a reader could take the edits as evidence that behaviour changed. **THEY AGREE: THE EDITS
  ARE TO COMMENTS AND THE CONVENTION THEY DESCRIBE IS THE SAME CONVENTION AS BEFORE.**
- **§5 AGAINST §8 ITEM 4**, because both describe the mixed comment. **THEY AGREE, AND §8 ITEM 4
  IS THE ONLY EDIT THAT IS NOT A NUMBER SWAP PRECISELY BECAUSE §5 ESTABLISHES THE COMMENT IS
  RIGHT ABOUT ONE CLAUSE.**
- **§5 AGAINST §6**, because §5 concludes the earliest instance is an ANNEXED citation rather than
  a wrong number, while §6 lists it as the earliest instance of the defect. **THEY AGREE ONLY
  BECAUSE §6 CARRIES §5's QUALIFICATION EXPLICITLY.** **WITHOUT IT, §6 WOULD READ AS THOUGH THE
  DEFECT BEGAN AS A MIS-NUMBERING, WHICH §5 SHOWS IT DID NOT.**
- **§6 AGAINST §7**, because §6's independent-landing account predicts recurrence that §7's
  correction cannot prevent. **THEY AGREE, AND §6 SAYS SO IN TERMS RATHER THAN LEAVING THE
  IMPLICATION** — the queued question in §7 is the only remedy that would reach an attractor in
  §3.6's own text.
- **§4 AGAINST §9**, because §4 states the extent and §9 states what that supersedes in a frozen
  file. **THEY AGREE.**
- **§6 AGAINST §9**, because both record the same draft error — one as a chronology mistake, one
  as a superseded extent claim. **THEY AGREE AND THEY ARE THE SAME MISTAKE SEEN FROM TWO
  DIRECTIONS**, which is stated so a later reader does not count it as two.

**ONE TENSION IS RECORDED RATHER THAN RESOLVED.** §7 corrects the citation to §3.5 while also
recording that §3.5's scope does not strictly cover the undefined-session and undefined-onset
cases. **THE CORRECTED CITATION IS THEREFORE BETTER AND STILL NOT EXACT, AND THAT IS LEFT
STANDING RATHER THAN ARGUED AWAY OR FIXED BY WIDENING §3.5, WHICH THIS FILE HAS NO STANDING TO
DO.**

**NO OTHER DEFECT WAS FOUND BY THIS CHECK IN THIS FILE.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**
