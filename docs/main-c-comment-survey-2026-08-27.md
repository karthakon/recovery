# main-c-comment-survey-2026-08-27

**Dated 2026-08-27. AN EXHAUSTIVE SURVEY OF EVERY COMMENT IN `src/c/main.c`, ENUMERATING FIFTEEN
DEFECTS BY SITE — AND THE RECORD OF THEIR CORRECTION, APPLIED AND COMMITTED AT `3f41133`.**

**FILED IN `~/recovery/docs/` — THE PUBLIC REPO. RULE 7 CHECK PERFORMED IN §11.**

**IT REPLACES DIAGNOSTIC QUEUE ITEM 7, WHICH ASSERTED "THE FIVE REMAINING WRONG COMMENTS IN
`main.c`" AND NAMED NONE OF THEM.** The count was carried across three handoffs without its
members ever being written down. **THE SURVEY FOUND FIFTEEN, NOT FIVE.**

**SEVEN OF THE FIFTEEN MISSTATED THE CODE. THREE OF THOSE WERE SELF-CONTRADICTIONS INSIDE
`main.c` ITSELF.**

**ALL FIFTEEN ARE CORRECTED IN SOURCE.** `3f41133`, one file, 32 insertions and 17 deletions,
**comment lines only, verified three independent ways — see §7.**

**NO NIGHT IS RE-SCORED, NO NIGHT FILE IS EDITED, NO BAND OR THRESHOLD MOVES, AND NO GATE VERDICT
ON ANY NIGHT CHANGES.** No reading on any night rested on a comment.

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

**THIS FILE IS AN INSTANCE OF THAT RULE OPERATING TWICE.** A partial survey covering only comments
that cite line numbers was drafted and **discarded before freezing** because the exhaustive version
was the one owed — it would have reported five defects and concluded that no comment misstates
behaviour, **and both would have been wrong.** A second draft, written after the full survey,
**declined to authorise the corrections** and was discarded in turn — see §7.

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

**ONE VERDICT WAS WITHDRAWN UNDER THIS RULE DURING THE SURVEY — SEE §6.**

## §3 — WHY THIS FILE EXISTS: THE ITEM WAS A COUNT WITH NO MEMBERS

**DIAGNOSTIC QUEUE ITEM 7 READ: "THE FIVE REMAINING WRONG COMMENTS IN `main.c`." IT NAMED NONE.**

**THE PROVENANCE WAS TRACED THROUGH THE HANDOFF CHAIN AND DOES NOT REACH AN ENUMERATION:**

- **`handoff-v69`** asserts four comments are wrong, as a caution against trusting source
  comments. **IT NAMES NONE OF THE FOUR.**
- **`handoff-v70` §6.2** corrects the count from four to five and **names only the fifth**.
- **`handoff-v71`** carries "the five remaining" and **names none.**

**SO ACROSS THREE HANDOFFS THE ONLY MEMBER EVER IDENTIFIED BY SITE IS v70's FIFTH**, together with
the DIAG 4 header found independently by `coarse-adjacency-counter-spec-v1` and fixed by
`d787269`.

**A COUNT WITH NO MEMBERS CANNOT BE DISCHARGED BY ANY SESSION THAT INHERITS IT.** It can only be
re-surveyed or believed. **IT WAS BELIEVED THREE TIMES.**

**THIS IS THE SAME DEFECT CLASS AS THE EXTENT CLAIMS CORRECTED TWICE ELSEWHERE TODAY** — a survey
result replaced by an accumulation — **sitting in the diagnostic queue rather than in a frozen
file. RULE 1 AND THE SURVEY RULE APPLY TO QUEUE ITEMS EXACTLY AS THEY APPLY TO DOCUMENTS.**

## §4 — THE SURVEY, PERFORMED 2026-08-27 AT `1836e23`

**`src/c/main.c` HELD 2480 LINES AND 654 COMMENT LINES. EVERY ONE WAS INSPECTED.**

**METHOD, STATED SO THE RESULT'S REACH IS READABLE:**

- **Lines 1–470 were read in full, in context.**
- **Every remaining comment line was listed and reviewed**, in ranges, with **no keyword filter
  left as the sole gate on any range** — see §6 for why that matters. Ranges 590–720, 1470–1560,
  1704–1820 and 2096–2200 were additionally read in full because their claims required
  surrounding code.
- **Every checkable claim was verified against the code or the file it describes, read directly:**
  `hrv_math.c` and `hrv_math.h`, `smoother.c` and `smoother.h`, `storage.c` and `storage.h`,
  `sleep_stage.c`, and the stock SDK `emery/include/pebble.h` at `4.33.1`.

**NO CITATION WAS ACCEPTED ON THE STRENGTH OF ITS PROSE BEING PLAUSIBLE.**

## §5 — THE ENUMERATION. FIFTEEN DEFECTS IN THREE CLASSES. LINE NUMBERS ARE PRE-CORRECTION.

### 5.1 STALE POINTER — prose correct, target wrong. FIVE.

| # | Site | What it cited | What is actually there |
|---|---|---|---|
| 1 | `78` | `hrv_buf_add` returns false at `hrv_math.c` 22, 36 | 36 correct; the range return is at **23**, and 22 is `b->rej_range++` |
| 2 | `105` | `quiet_time_is_active` at `pebble.h` 9003 | 9003 is doc prose; the declaration is at **9005** |
| 3 | `1650` | `smoother.c` 215 writes `EpochRecord.reserved` | the write is at **265**; 215 is `if (n < 2) return;` in `rem_resume_repair` |
| 4 | `1701` | `find_onset` at `smoother.c` 93–106 | `find_onset` is **110–123**; 93–106 is the tail of `rem_lat_mult` and all of `emit_cost` |
| 5 | `2031` | `main.c` 538 leaves the Awake set equal to `(c1 \|\| c2)` | 538 is `s_night_baseline_var = 0;`; the assignment is at **918** |

**ITEM 4 IS `handoff-v70` §6.2's FIFTH.** **THAT FILE'S OWN CORRECTION IS ALSO WRONG AND IT IS
RECORDED RATHER THAN PASSED OVER:** it states `find_onset` is at `smoother.c:107-120`. **IT IS AT
110–123.** **THE FILE THAT IDENTIFIED A STALE POINTER MISSTATED THE TARGET IT WAS CORRECTING TO.**

### 5.2 WRONG PROSE — the comment misstated the code. SEVEN.

| # | Site | The claim | The code |
|---|---|---|---|
| 6 | `91` | `s_unknown_min` "rendered on DIAG" | rendered at **1586, inside `prv_draw_diag2`** — and the comment at 1552 records that `Unk` was pushed off DIAG onto DIAG 2 |
| 7 | `259` | of `s_ahr_whole`: "NOTHING READS IT AND IT DECIDES NOTHING" | **line 940 reads it** to compute `s_ab_w`, declared five lines below the comment |
| 8 | `624` | the gate "tests `F(m) > 2*A`" | `gate = base_final`, tested `> gate`, and `prv_measure(anchor)` passes `A_D` — **the test is `F(m) > A`** |
| 9 | `908` | "The 103 is carried from `sleep_stage.c`'s 97" | **`sleep_stage.c` contains no 97 and no HR clause** — `classifier-spec-v3` §4.1 removed the live HR term |
| 10 | `1653` | "registered identity: `rem_total` MUST equal the night's `v_over_gate_count`" | **not registered, and false** |
| 11 | `1815` | DIAG 4 holds "**seven** values" on "**FOUR** lines" | **ten values on six lines**, which the header comment at 1902–1904 tracks explicitly |
| 12 | `2173` | "RUNS reads PERSIST, so unlike DIAG 2 this survives app exit" | `AwO` and its group guard on `has_awo`, set from **`s_onset_epoch_idx`, a RAM-only static** — they print `--` after an app exit |

**ITEM 8 MISSTATED A LIVE THRESHOLD BY A FACTOR OF TWO.** The `2*A` wording is stale from before
`classifier-spec-v4` §2 moved the Light band's upper edge from `2*A_D` to `A_D`.
`prv_base_redecide` implements `v > anchor` and tallies on `v > anchor`, so `prv_measure`'s
`gate = base_final` is **correct** — **the comment was the only thing wrong, four lines above the
code contradicting it.**

**ITEM 10 WAS FALSE THREE WAYS OVER.** `measurement-spec-v1-feature-correction` §2 states the
equality is **expected by construction but explicitly NOT registered.** N32 read `RemN 40` against
`Gate 188`. **AND `main.c` CONTRADICTED ITSELF** — 193–196 and 1539–1542 both say the partition is
NOT guaranteed to equal `Gate`.

**THREE OF THE SEVEN WERE SELF-CONTRADICTIONS WITHIN `main.c`:** item 8 against 628–631, item 10
against 193–196 and 1539–1542, item 12 against 2185–2187.

### 5.3 IMPRECISE — a defensible reading existed. THREE.

| # | Site | The claim | The qualification |
|---|---|---|---|
| 13 | `62` | `s_hd_prev` "assigned on every `ppi > 0` event whether accepted or rejected" | the contrast is correct; the **recording-gate correction moved the assignment inside `if (s_recording)`** and this was not updated |
| 14 | `725` | "This is the ONLY pass that writes `StageAwake`" | false plainly — `prv_close_minute` and `smoother_run` both write it — but **true for the stop-time re-decision passes** |
| 15 | `2459` | app-lifetime "because `prv_close_minute` runs unconditionally" | it is **called** unconditionally and **returns immediately** when not recording |

**THESE THREE WERE RECORDED AS IMPRECISE RATHER THAN ASSERTED WRONG.** A survey that inflates its
count by resolving every ambiguity against the code is not a survey. **THEY WERE CORRECTED
ANYWAY** — see §7.

## §6 — A VERDICT WITHDRAWN DURING THIS SURVEY

**IT WAS STATED MID-SURVEY THAT `handoff-v69`'s "`main.c:1620` REGISTERS A FALSE IDENTITY" WAS NOT
PRESENT AT THIS PIN.** That rested on a search over comments mentioning `Gate` or `RemN` — **the
display labels.** The comment used the **field names**, `rem_total` and `v_over_gate_count`, so the
search never reached it.

**THE VERDICT IS WITHDRAWN, NOT AMENDED, PER RULE 21.** The premise was read and the replacement
verdict is §5.2 item 10: **it was present, and it was false.**

**RECORDED BECAUSE THE FAILURE IS INSTRUCTIVE: A SEARCH OVER RENDERED LABELS DOES NOT REACH
COMMENTS WRITTEN IN FIELD NAMES**, and the same gap would have hidden any other defect phrased
that way. **IT IS WHY §4's METHOD LISTS EVERY COMMENT RATHER THAN FILTERING FOR KEYWORDS.**

## §7 — THE CORRECTION, APPLIED AND VERIFIED

**ALL FIFTEEN ARE CORRECTED IN SOURCE AT `3f41133`.** One file, **32 insertions and 17 deletions,
comment lines only.**

**A PRIOR DRAFT OF THIS FILE DECLINED TO AUTHORISE THE CORRECTIONS AND WAS DISCARDED.** Its
argument was that a comment block touched at a known date reads to a later reviewer as vetted, so
touching fifteen blocks for prose spends that signal. **THAT ARGUMENT IS REJECTED AND THE REASON
IS RECORDED: A WRONG COMMENT IN SOURCE IS A HAZARD TO EVERY READER OF THE SOURCE, AND AN ERRATUM
IN A SEPARATE FILE IS NOT A FIX.** The project's long-term intent is to justify this method from
the public record; **a public record in which `main.c` misstates its own thresholds is not that.**

**FOUR CORRECTIONS DO MORE THAN SWAP A NUMBER, BECAUSE A CORRECTED POINTER ALONE WOULD HAVE LEFT
THE READER MISINFORMED:**

- **Item 8** now states the gate tests `F(m) > A` **and records that `classifier-spec-v4` §2 moved
  T1's edge from `2*A_D` to `A_D`**, so the old wording is identifiable as pre-v4 rather than
  silently deleted.
- **Item 10** now states `rem_total <= v_over_gate_count`, that the equality is not registered and
  does not hold, and the mechanism — matching what 193–196 and 1539–1542 already said.
- **Item 12** now separates what survives an app exit from what does not, and instructs live
  capture with DIAG-2 discipline.
- **Item 9** now says the 97 came from the pre-v3 live HR clause that `classifier-spec-v3` §4.1
  removed, **so a reader grepping `sleep_stage.c` knows why it is not there.**

**THE CHANGE IS COMMENT-ONLY AND THAT IS ESTABLISHED THREE INDEPENDENT WAYS, NOT ASSERTED:**

1. **EVERY ADDED AND REMOVED LINE IN THE DIFF IS A COMMENT LINE.** An audit for non-comment
   changes returned empty.
2. **COMMENT-STRIPPED, WHITESPACE-STRIPPED SOURCE IS BYTE-IDENTICAL** before and after —
   `90d565ff56d2fc9579dcc2af27b5572d` both sides.
3. **THE COMPILED IMAGE DIFFERS IN EXACTLY TWENTY BYTES, AND ALL TWENTY ARE THE BUILD-ID.**
   `pebble-app.raw.bin` is 24056 bytes both sides; `cmp -l` reports 20 differing bytes at offset
   148; the twenty bytes equal the `.note.gnu.build-id` descriptor extracted from each ELF.
   **NO INSTRUCTION, CONSTANT OR DATA BYTE CHANGED.**

**THE ELF ITSELF DIFFERS AND THAT IS EXPECTED AND EXPLAINED:** comments shifted code down fifteen
lines, DWARF line tables encode line numbers, the build-id hashes the ELF, and
`inject-metadata` embeds it. **THE BUILD IS REPRODUCIBLE — a clean rebuild from identical source
gives an identical ELF — SO THE COMPARISON MEANS SOMETHING.**

**THE BUILD SUCCEEDS.** Two warnings are emitted and **both pre-date this change**: `prv_ui_tick`
defined but not used, and a linker note about a LOAD segment with RWX permissions.

## §8 — CONSEQUENCE FOR THE PIN, REGISTERED BECAUSE IT IS EASY TO MISS

**`3f41133` TOUCHES `src/c/main.c`.** The four commits before it were documentation only.

**THEREFORE `P-DCIDENT`'s INCREMENT-SITE SOURCE READ, VALID AT `d787269`, DID NOT SURVIVE THIS
COMMIT.** `coarse-adjacency-counter-spec-v1` §5.2 makes the read valid for the pin it is run at
and no later pin, **with no exception for changes that do not alter behaviour.** The binary
evidence in §7 shows the increments cannot have moved; **that is an argument for the outcome and
not a substitute for the read.**

**THE READ WAS RE-PERFORMED AT `3f41133` BEFORE THIS FILE WAS FROZEN AND BEFORE ANY N33 DOCUMENT
WAS WRITTEN.** It confirms: `s_dc` increments inside `if (acc)`, at the same nesting depth as
`s_dn++` and the divisor tests, under no other conditional, with the `Dm` block closed above it
and `s_dm_prev = ppi;` as the last statement before the block closes, all inside the `s_recording`
gate.

**IT IS VALID FOR `3f41133` AND NO LATER PIN.**

## §9 — What this survey establishes and what it does not

**IT ESTABLISHED THAT SEVEN COMMENTS MISSTATED THE CODE BENEATH THEM**, three contradicted other
comments in the same file, and one misstated a live threshold by a factor of two. **THAT IS A
WORSE RESULT THAN THE QUEUE ITEM IMPLIED, NOT A BETTER ONE.**

**IT DOES NOT MAKE SOURCE COMMENTS TRUSTWORTHY AND DOES NOT SOFTEN THE RULE THAT THEY ARE NOT
EVIDENCE.** `construction-sweep-2026-08-24` §7 stands unchanged. **THE CODE IS READ. A CORRECTED
COMMENT IS STILL NOT EVIDENCE**, and correcting fifteen does not license trusting the six hundred
and thirty-nine that were already right.

**IT ESTABLISHES NOTHING ABOUT COMMENTS IN `smoother.c`, `storage.c`, `hrv_math.c`,
`sleep_stage.c` OR ANY HEADER.** Those were read **as targets of `main.c`'s citations**, not
surveyed for their own. **NO CLAIM IS MADE ABOUT THEM IN EITHER DIRECTION, AND THE ABSENCE OF A
CLAIM IS NOT A CLEAN BILL.**

**IT DOES NOT ESTABLISH THAT NO DEFECT WAS MISSED.** Every comment was inspected and every
checkable claim verified, **but a comment subtly wrong in a way that reads as correct would pass
this survey**, exactly as item 10 passed every prior one.

## §10 — WHAT REPLACES DIAGNOSTIC QUEUE ITEM 7

**ITEM 7 IS CLOSED AND ITS SUBJECT IS DISCHARGED, NOT DEFERRED — the corrections are in source.**

**NOTHING IS ENTERED IN THE DIAGNOSTIC QUEUE BY THIS FILE.**

**TWO STANDING CONSEQUENCES ARE REGISTERED:**

1. **A DIAGNOSTIC QUEUE ITEM ASSERTING A COUNT MUST NAME ITS MEMBERS.** An item that says "the
   five remaining X" without naming them cannot be discharged, verified or inherited. **A COUNT IS
   A SURVEY RESULT AND A QUEUE IS NOT EXEMPT FROM THAT.**
2. **A SURVEY OVER A CODEBASE SEARCHES FOR THE THING, NOT FOR THE NAME IT IS USUALLY CALLED.**
   Rendered labels, field names, spec labels and local variable names are different vocabularies
   for the same quantity, **and a search in one establishes nothing about the others** — §6.

## §11 — Rule 2, Rule 3 and Rule 7 checks

**RULE 2 — NO CONSTANT ENTERS ANY CODE PATH AND NO EXECUTABLE BYTE CHANGED**, established in §7.
No band, threshold or parameter moves.

**RULE 3 — THIS FILE REGISTERS NO SCORED CRITERION, AND THE CHECK IS STATED RATHER THAN SKIPPED.**
Its claims are falsifiable by re-running the survey: **a row in §5 whose citation turns out to have
been correct would be removed, and a comment not in §5 that misstated its code would be added.**
§4 names the method so either can be run.

**CHECKED AGAINST THE THIRD VACUITY FORM — A FINDING THAT MEANS SOMETHING ELSE. FOUR ARE
REGISTERED:**

1. **THE COUNT OF FIFTEEN READ AS SUPERSEDING v69's FOUR PLUS v70's FIFTH BY ARITHMETIC.** §3
   forbids it — **the members were never named, so the sets cannot be compared.**
2. **ITEM 8 READ AS A DEFECT IN `prv_measure`.** §5.2 forbids it. **THE CODE WAS CORRECT.**
3. **§7's CORRECTIONS READ AS LICENSING TRUST IN COMMENTS.** §9 forbids it.
4. **§5.2 READ AS IMPLYING SOME NIGHT WAS MISSCORED.** No reading rested on a comment. **NO NIGHT
   IS RE-SCORED.**

**RULE 7 — THE ONLY NIGHT VALUES NAMED ARE `RemN 40` AND `Gate 188` FROM N32**, instrument
counters rendered on a diagnostic screen rather than physiological measurements, stated because
item 10's falsity is demonstrated by them. **SCAN BEFORE THE PUBLIC COMMIT REGARDLESS; IF THE SCAN
JUDGES THEM SUBJECT DATA THEY ARE REMOVED AND THE DEMONSTRATION CITES `C00-N32.md` INSTEAD.**

## §12 — Self-consistency check, per RULE 19

**PERFORMED BEFORE THIS FILE IS FROZEN AND RECORDED HERE AS A NAMED SECTION. A FILE THAT DOES NOT
RECORD THE CHECK HAS NOT HAD IT.**

**FORM 1, ONE QUANTITY DESCRIBED TWICE.** The total is fifteen in the header, §5's heading and
§10, and §5's three tables hold five, seven and three, summing to fifteen. The self-contradiction
count is three in the header and three in §5.2's closing line, and the three named match. The diff
size is 32/17 in the header and in §7. `find_onset`'s true location is 110–123 in §5.1 row 4 and
again in §5.1's correction of v70.

**FORM 2, QUANTITATIVE AGAINST PROSE, SUBSTITUTED AT BOTH EXTREMES.** **Had the survey found
zero:** §3's finding would stand unchanged, §5 would report an empty enumeration, and §7 would
have nothing to apply. **Had it found only stale pointers:** §7's rejected argument would have been
stronger and §9's first claim would reverse. **THE PROSE IS WRITTEN AGAINST WHAT WAS FOUND AND
SAYS SO** — which the first discarded draft did not do, and is why it was discarded.

**FORM 3, EVERY BRANCH REACHABLE.** Checked in §11.

**FORM 4, SECTIONS SERVING DIFFERENT READERS, PAIRED BY SHARED QUANTITY.** §3 serves whoever asks
why the item could not be worked; §4 and §5 serve whoever checks the survey; §6 serves whoever
repeats it; §7 serves whoever reviews the commit; §8 serves whoever writes N33; §9 serves whoever
would over-read the result.

**THE PAIRINGS ACTUALLY RUN:**

- **§5 AGAINST §7**, because §5 enumerates and §7 records what was applied. **THEY AGREE — all
  fifteen rows are corrected, including the three §5.3 calls imprecise**, and §7 says so rather
  than leaving the reader to infer which were acted on.
- **§7 AGAINST §8**, because §7's commit is what invalidated the pin §8 re-reads. **THEY AGREE,
  AND §8 REFUSES THE SHORTCUT §7's OWN BINARY EVIDENCE WOULD HAVE OFFERED.**
- **§7 AGAINST §9**, because §7 corrects comments and §9 denies that corrected comments are
  evidence. **THEY AGREE — the correction is for the reader, not for the record of what is
  established.**
- **§6 AGAINST §4**, because §6 records a search that missed and §4 states the method that
  replaced it. **THEY AGREE, AND §4's "NO KEYWORD FILTER AS THE SOLE GATE" IS THERE BECAUSE OF
  §6.**
- **§3 AGAINST §10**, because §3 diagnoses the item and §10 replaces it. **THEY AGREE, AND §10's
  SECOND STANDING RULE COMES FROM §6 RATHER THAN §3**, stated so the two are not conflated.

**NO TENSION IS LEFT STANDING IN THIS FILE. A FROZEN FILE MAY NOT CARRY AN UNRESOLVED ITEM; IT IS
RESOLVED OR IT IS DROPPED BEFORE THE FILE IS FROZEN.**

**NO DEFECT WAS FOUND BY THIS CHECK IN THIS FILE.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**

**DIAGNOSTIC QUEUE ITEM 7 IS CLOSED AND DISCHARGED. NO NEW ITEM IS ENTERED.**

**THE CORRECTIONS ARE IN SOURCE AT `3f41133`. `P-DCIDENT`'s INCREMENT-SITE READ IS VALID AT
`3f41133` AND NO LATER PIN.**
