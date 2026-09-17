# ui-tick-removal-count-correction-2026-09-16

**Dated 2026-09-16. A SUPERSEDING DATED FILE, NOT AN EDIT. It supersedes the LINE RANGE and the
DELETION COUNT in `ui-tick-removal-spec-v1.md` (`3e22413`) and NOTHING ELSE.** That file is
unchanged and stays exactly as frozen.

**FILED IN `~/recovery/docs/` — THE PUBLIC REPO. NO SUBJECT PHYSIOLOGICAL VALUE APPEARS IN THIS
FILE.**

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

## §3 — The defect

**`ui-tick-removal-spec-v1` §3, §4 and §10 give `prv_ui_tick` as `main.c:568-574` (seven lines),
the following blank as `main.c:575`, and the required diffstat as NINE deletions. ALL THREE ARE
WRONG.**

**READ FROM SOURCE AT `387d05c` — `sed -n '566,576p' src/c/main.c` on 2026-09-16, and confirmed by
the edit script's exact-text assertions:** line 566 is the closing brace of `prv_health_handler`,
567 is blank, **568-573 are the six lines of `prv_ui_tick`**, **574 is blank**, and 575 is
`static void prv_start_recording(void) {`. The edit script asserted the seven lines at 568-574 to
be the six function lines followed by an empty line, and asserted line 575 to be
`prv_start_recording`. **Both assertions passed.**

**THE RANGE WAS NOT COUNTED WHEN THE SPEC WAS WRITTEN.** `568-574` was carried from
`handoff-v73` §12 item 2 ("The function is `main.c:568-574` at HEAD") without enumeration.
**`handoff-v73` §12 ITEM 2 CARRIES THE SAME WRONG RANGE AND IS NOT EDITED; THIS PARAGRAPH IS ITS
CORRECTION.** The spec's §10 Form 1 check compared the count against the range it had already
written and so could not find the error — **a consistency check between two statements of one
unread premise is not a read of the premise.**

**THE DECLARATION LINE `main.c:291` IS CORRECT** and was read from source before the spec was
frozen.

## §4 — What replaces what

- **`ui-tick-removal-spec-v1` §3 and §4: `prv_ui_tick` IS `main.c:568-573`, SIX LINES. The blank
  line removed with it is `main.c:574`.**
- **`ui-tick-removal-spec-v1` §4 and §5 step 1: `git diff --stat` MUST READ ONE FILE, 0
  INSERTIONS, 8 DELETIONS** — one declaration line, six function lines, one blank line.
- **`ui-tick-removal-spec-v1` §8 (Rule 3): the failing diff is "a diff other than eight
  deletions".**
- **`ui-tick-removal-spec-v1` §10 Form 1: the count enumerates as 1 + 6 + 1 = 8.**

**NOTHING ELSE IN THE SPEC CHANGES.** The change it specifies — the declaration, the whole
function and the single blank line after it, and no other line — was stated in words and is
unchanged. **§4's instruction that a wrong count means the edit is redone from `387d05c` applied
to a defective edit; here the count was defective and the edit was not, and the edit is not
redone.**

## §5 — The edit already made, checked against the corrected text

**The edit made on 2026-09-16 was read in full as a unified diff before this file was written.**
It removes exactly `static AppTimer *s_ui_timer = NULL;` in the hunk at 288, and exactly the six
lines of `prv_ui_tick` plus the blank line after it in the hunk at 565. **`git diff --stat` read
`1 file changed, 8 deletions(-)`. THAT MATCHES §4 OF THIS FILE.** It was not committed before this
file was frozen.

## §6 — Rule 2, Rule 3 and Rule 7 checks

**RULE 2 — no constant, band, threshold or code path is touched by this file.** **RULE 3 — the
corrected diffstat check can fail and can pass, exactly as the original.** **RULE 7 — no subject
physiological value appears here. Scan before the public commit regardless.**

## §7 — Self-consistency check, per RULE 19

**FORM 1.** The function is six lines in §3 and §4, 568 through 573 inclusive is six, and the
count is 1 + 6 + 1 = 8 in §4 and §5. **The range was enumerated from `sed` output, not carried.**
**FORM 2.** Not applicable — no quantitative rule. **FORM 3.** §6. **FORM 4.** §3 against §5,
because §5 checks the edit against §3's reading; **they agree.**

**NO DEFECT WAS FOUND BY THIS CHECK IN THIS FILE.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**
