# p6-instrument-disqualification-2026-08-27

**Dated 2026-08-27. Written after N32 is scored and pushed at `7793af0`, and owed since N31
closed diagnostic queue item 41.**

**FILED IN `~/recovery/docs/` — THE PUBLIC REPO. RULE 7 CHECK PERFORMED IN §9: NO SUBJECT
PHYSIOLOGICAL VALUE APPEARS IN THIS FILE. No IDLE reading from any night is reproduced here,
not as a value and not as a difference.**

**IT DISQUALIFIES AN INSTRUMENT. IT CHANGES NO CODE, NO PARAMETER, NO BAND AND NO SPEC. IT
SUPERSEDES NOTHING AND EDITS NOTHING.**

**IT IS OWED BY N31, WHICH CLOSED DIAGNOSTIC QUEUE ITEM 41 BY A SOURCE READ AND RECORDED THAT
P6's STANDING REQUIRED ITS OWN DATED FILE.** N32 recorded P6 NOT PERFORMED and did not score
it. **THIS FILE IS THAT DATED FILE AND NOTHING MORE.**

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

## §3 — What P6 was, and what it was taken to show

**P6 COMPARED THE PPI-DERIVED RATE AGAINST THE DISPLAYED HR, BOTH READ FROM THE IDLE SCREEN
AFTER A RECORDING STOPPED.** A positive difference in the registered direction was recorded on
every night it was scored.

**THE SERIES WAS SIXTEEN NIGHTS LONG AND UNBROKEN IN ONE DIRECTION.** That length is the reason
this file exists rather than a footnote: **a long consistent series is exactly what makes an
instrument stop being questioned.**

**THE READING WAS NEVER OF THE NIGHT.** It was of two app-lifetime values sampled minutes after
the session ended, during the capture sequence, with the wearer awake and moving. **That is
established below from source and not from the shape of the series.**

## §4 — THE SOURCE READ, PERFORMED AT `d787269` ON 2026-08-27, AND WHY IT WAS RE-PERFORMED

**N31's READ WAS PERFORMED AT `ebd03f2` AND DOES NOT CARRY.** A source read is valid for the pin
it was run at and for no later pin — the standard this project already applies to `P-DCIDENT`'s
increment-site read, and it applies here identically. **THE PIN HAS MOVED TWICE SINCE, AND ONE OF
THOSE COMMITS CHANGED `main.c`.** The read is therefore re-performed in full rather than cited.

**THE LINE NUMBERS IN N31's ACCOUNT ARE STALE AND THAT IS RECORDED RATHER THAN QUIETLY
CORRECTED.** N31 named the IDLE render site as `main.c:1206`; **at `d787269` it is `main.c:1223`.**
N31 named the `s_last_ppi` assignment as `main.c:455`; **at `d787269` it is `main.c:466`.** All
line numbers in this project are STALE BY CONSTRUCTION and the identifiers, not the numbers, are
what a later session greps for.

**THE ENUMERATION IS COMPLETE AND IS A SURVEY RESULT, NOT AN ACCUMULATION.** A `grep` over both
identifiers across the whole of `src/c/main.c` returns every site, and the sites are:

- **`s_last_ppi`** — declared at 130; the prohibition comment at 19; assigned at **466**; read at
  **1223**. **No other write exists.**
- **`s_last_hr`** — declared at 131 with its own comment at 132; read at 335 and 338 for the
  freshness-gated night sum; assigned at **451**; referenced at 701 in a comment; read at
  **1223**. **No other write exists.**

**FINDING 1 — `s_last_ppi` IS ASSIGNED OUTSIDE THE RECORDING GATE.** The assignment at 466 sits
inside `if (ppi > 0)` and **BELOW the `if (s_recording)` block that closes above it.** The gated
block contains `s_hn`, the `Hd` comparison and `s_hd_prev`; **the `s_last_ppi` write is outside
it.** Every PPI event writes it whether a session is running or not.

**FINDING 2 — `s_last_hr` IS NOT GATED AT ALL, AND THIS IS READ FROM THE ASSIGNMENT SITE RATHER
THAN FROM A COMMENT.** The whole `HealthEventHeartRateUpdate` branch carries **no `s_recording`
test**. The assignment at 451 is conditioned only on `hr > 0`. **N31 TOOK THE HR SIDE FROM THE
COMMENT AT WHAT WAS THEN `main.c:62`. A COMMENT IS NOT A SOURCE READ, and this file establishes
the property directly.** The comment at 132 happens to be correct; **that it is correct is a
result of this read and was not an input to it.**

**FINDING 3 — NEITHER VALUE IS RESET AT SESSION START.** `prv_start_recording` was read in full.
It resets `s_last_hr_time` — with its `c-spec-v3 s3.2` trailing comment — and **it does not touch
`s_last_hr` or `s_last_ppi`.** The `grep` above independently confirms it: **if a reset existed it
would be a write, and there are no writes other than 451 and 466.**

**FINDING 4 — THE IDLE RENDER READS BOTH.** `main.c:1223` formats the `HR` / `PPI` line from
`s_last_hr` and `s_last_ppi` directly. **It applies no session guard, no freshness test and no
recording test.**

**THEREFORE THE IDLE LINE REPORTS THE MOST RECENT VALUES THE APP HAS SEEN AT THE MOMENT THE
SCREEN IS DRAWN, WHICH DURING THE CAPTURE SEQUENCE IS MINUTES AFTER THE RECORDING STOPPED.**
**THEY ARE NOT NIGHT QUANTITIES AND THEY NEVER WERE.**

**THE CODE ALREADY SAID SO.** The comment at 19 states outright that `s_last_ppi` is app-lifetime
and unreset and **must NOT be used** — written for the `Hd` reference, which correctly declines
it in favour of a gated session-scoped `s_hd_prev`. **THE HRV CADENCE WORK AVOIDED EXACTLY THIS
DEFECT WHILE THE IDLE RENDER CARRIED IT UNEXAMINED, AND THE TWO SITES ARE FOURTEEN LINES APART IN
THE SAME FILE.**

## §5 — What is disqualified, and in what sense

**P6 IS DISQUALIFIED AS AN INSTRUMENT. IT IS NOT WITHDRAWN AS A CRITERION AND THE DISTINCTION IS
LOAD-BEARING.** A withdrawn criterion is one the project decided not to score. **A disqualified
instrument is one whose readings were never of the quantity the criterion names.**

**EVERY PRIOR P6 READING IN THE SERIES IS DISQUALIFIED, INCLUDING ALL SIXTEEN POSITIVE ONES.**
They all came from this same render site and the defect is a property of the site, not of any
night. **THE SIXTEEN-NIGHT SERIES IS NOT EVIDENCE ABOUT ANY NIGHT AND MUST NOT BE CITED AS
EVIDENCE ABOUT ANYTHING.**

**A DISQUALIFIED READING IS NOT A FALSIFIED ONE.** Nothing here establishes that the PPI-derived
rate does NOT sit below the displayed HR. **It establishes that the instrument never tested it.**
**NOT PERFORMED IS NOT A PASS AND IT IS NOT A FAIL**, and a later session must not convert the
disqualification into a negative finding.

**N31's ANOMALOUS READING IS RECORDED AS THE OCCASION AND NOT AS THE EVIDENCE.** RULE 12 held it
as presumed malformed until its own correctness was established, and the read then established
that it was not correct. **THE DEFECT WOULD HAVE BEEN THERE HAD THE READING LOOKED ORDINARY, AND
IT WAS THERE ON EVERY NIGHT THAT DID.** The values themselves are in the night files and are not
reproduced here.

## §6 — What is NOT changed by this file

- **NO CODE IS CHANGED AND NONE IS AUTHORISED.** In particular **the IDLE render site is NOT
  changed**, not to add a guard, not to add a reset, not to relabel the line.
- **THE SESSION THAT FINDS A CHANGE CONVENIENT IS THE SESSION THAT MUST NOT MAKE IT.** This file
  exists because P6's standing needed a dated record, **and a file written to record a standing
  stays answerable by not also authorising a repair.**
- **`s_hrv_events`, `s_last_hr_time`, `HR_STALE_SEC` 180 AND THE FRESHNESS GATE AT 335 ARE NOT
  TOUCHED, NOT RELABELLED AND NOT REINTERPRETED.** `HR_STALE_SEC` 180 remains a different
  constant from `HRV_STALE_SEC` 10. RULE 9.
- **NO SPEC IS EDITED.** Any spec naming P6 stays as frozen; this file is filed beside them.
- **NO BAND MOVES AND NO PARAMETER MOVES.** P6 gated nothing, so its removal loosens nothing.
- **THE CAPTURE SEQUENCE IS UNCHANGED AT EIGHT CAPTURES.** The IDLE screen is still captured;
  what changes is what may be read off it, which is nothing that is scored.

## §7 — What would restore an instrument for this question, NAMED AND NOT SPECIFIED HERE

**THE QUESTION P6 EXISTED TO ANSWER IS NOW UNANSWERED AND NOTHING CURRENTLY RENDERED ANSWERS
IT.** That is stated plainly rather than left as an implication.

**WHAT AN INSTRUMENT WOULD REQUIRE, in the shape the cadence counters already demonstrate:** a
pair of session-scoped values, gated on `s_recording`, holding their own references rather than
the app-lifetime ones, reset in `prv_start_recording`, and rendered on a screen the stop sequence
captures live. **THE `Hd` REFERENCE `s_hd_prev` IS THE WORKED PRECEDENT AND IT IS FOURTEEN LINES
FROM THE DEFECT.**

**THIS FILE DOES NOT SPECIFY THAT INSTRUMENT AND MUST NOT BE READ AS HAVING SPECIFIED IT.** It
would be its own dated spec, and **whether the question is worth an instrument at all is not
decided here.** It is not on the path to a working app: the blocker is the classifier, and Awake
over-reports by hours.

## §8 — Rule 2 and Rule 3 checks

**RULE 2 — NO CONSTANT IS INTRODUCED, MOVED OR FITTED.** No value in this file comes from any
recorded night. **The line numbers are read from source at one pin and are stale by
construction.**

**RULE 3 — THIS FILE REGISTERS NO SCORED CRITERION**, so there is no vacuity check to run on a
criterion. **THE CHECK IS STATED RATHER THAN SKIPPED, because "not applicable" written without
looking is how a check goes missing.** The nearest analogue is the disqualification itself, and
it is falsifiable in the only way that matters: **a read at a later pin showing the values gated
and reset would establish that the defect no longer holds there.** That read is exactly what §4
requires per pin, and it can come out either way.

**CHECKED AGAINST THE THIRD VACUITY FORM — A FINDING THAT MEANS SOMETHING ELSE. TWO ARE
REGISTERED:**

1. **THE DISQUALIFICATION READ AS A FINDING THAT THE PPI-DERIVED RATE IS FINE, OR THAT IT IS
   NOT.** §5 forbids both.
2. **THE DISQUALIFICATION READ AS LICENSING THE OBVIOUS ONE-LINE REPAIR.** §6 forbids it.
   **THIS IS THE INSTANCE MOST LIKELY TO ARRIVE LOOKING LIKE HOUSEKEEPING.**

## §9 — Rule 7 check

**NO SUBJECT PHYSIOLOGICAL VALUE APPEARS IN THIS FILE.** No IDLE HR, no IDLE PPI, no RMSSD, no
night-specific reading of any kind, and **no difference between any two of them** — a difference
is a derivable value and the check is over what the file permits a reader to derive, not over
what it states, per `coarse-adjacency-counter-spec-v1` §12.

**THE NIGHT COUNT IS NOT A PHYSIOLOGICAL VALUE.** Sixteen nights is a count of readings, not a
reading.

**SCAN BEFORE THE PUBLIC COMMIT REGARDLESS. THIS SECTION IS NOT A SUBSTITUTE FOR IT.**

## §10 — Self-consistency check, per RULE 19

**PERFORMED BEFORE THIS FILE IS FROZEN AND RECORDED HERE AS A NAMED SECTION. A FILE THAT DOES NOT
RECORD THE CHECK HAS NOT HAD IT.**

**FORM 1, ONE QUANTITY DESCRIBED TWICE.** The pin appears in the header, in §4 and in §8 and
reads `d787269` in all three. The render site appears in §4's enumeration and in §4's finding 4
and reads 1223 in both. The assignment sites appear in the enumeration and in findings 1 and 2
and read 466 and 451 in both places.

**FORM 2, QUANTITATIVE AGAINST PROSE, SUBSTITUTED AT BOTH EXTREMES.** **At a session that never
started:** both values hold whatever the app last saw, the IDLE line still renders, and §4's
claim that the line is not session-scoped holds. **At a session that stopped one second before
the screen was drawn:** the values are still the last seen rather than the night's, and the claim
holds again — **the defect is not a function of how much time elapsed, which is why the elapsed
minutes in §3 are context and not the argument.**

**FORM 3, EVERY BRANCH REACHABLE.** Checked in §8.

**FORM 4, SECTIONS SERVING DIFFERENT READERS, PAIRED BY SHARED QUANTITY.** §3 serves whoever asks
what P6 was; §4 serves whoever checks the source; §5 serves whoever scores a night; §6 and §7
serve whoever is tempted to fix it.

**THE PAIRINGS ACTUALLY RUN:**

- **§4 AGAINST §5**, because both constrain what the disqualification rests on: §4 establishes it
  from source at one pin and §5 states its scope over the whole series. **THEY AGREE ONLY BECAUSE
  THE DEFECT IS A PROPERTY OF THE RENDER SITE RATHER THAN OF ANY NIGHT**, and §5 says so
  explicitly rather than relying on §4 being read first.
- **§5 AGAINST §7**, because both constrain what is now unknown: §5 says the instrument never
  tested the question and §7 says nothing currently rendered answers it. **THEY AGREE.**
- **§6 AGAINST §7**, because both constrain what may be built: §7 names the shape an instrument
  would take and §6 forbids changing the render site. **THEY AGREE ONLY BECAUSE §7 NAMES A NEW
  INSTRUMENT AND NOT A REPAIR OF THE EXISTING LINE**, and a later session must not collapse the
  two.
- **§4 AGAINST §8**, because both constrain the standing of a source read: §4 re-performs N31's
  read on the ground that a read does not carry across pins, and §8's falsifiability rests on the
  same per-pin obligation. **THEY AGREE.**

**ONE TENSION IS RECORDED RATHER THAN RESOLVED.** **THIS FILE DISQUALIFIES AN INSTRUMENT AND
LEAVES ITS QUESTION UNANSWERED, WHILE FORBIDDING THE ONE-LINE CHANGE THAT WOULD ANSWER IT.** That
is deliberate and it is uncomfortable. **The discomfort is the point: the session that discovers
a defect is the session with the strongest motive to repair it immediately, and this project has
already recorded that a spec written to answer one question stays answerable by not absorbing a
second.**

**NO OTHER DEFECT WAS FOUND BY THIS CHECK IN THIS FILE.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**

**DIAGNOSTIC QUEUE ITEM 1 IS CLOSED BY THIS FILE. `D10r` REMAINS OWED AND IS NOT ADDRESSED
HERE.**
