# ui-tick-removal-spec-v1

**Dated 2026-09-16. Written after N33 was scored and pushed (`recovery-nights` `4b324ed`) and
before N34 is pre-registered.**

**FILED IN `~/recovery/docs/` — THE PUBLIC REPO. RULE 7 CHECK PERFORMED IN §9: NO SUBJECT
PHYSIOLOGICAL VALUE APPEARS IN THIS FILE.**

**DEAD-CODE REMOVAL ONLY. NO CLASSIFIER INPUT, NO STAGE DECISION, NO TOTAL, NO STORED FIELD, NO
PERSISTED STRUCTURE, NO RENDER, NO NAVIGATION.** `CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION`
do NOT change and MUST be **VERIFIED UNCHANGED BY READING `src/c/storage.h` AFTER
IMPLEMENTATION, NOT ASSERTED.**

**DISCHARGES handoff-v73 §12 ITEM 2, KRISTOPHER'S DECISION OF 2026-08-31 TO FIX `prv_ui_tick`
AFTER N33 IS SCORED AND BEFORE N34. THE FORM OF THE FIX — REMOVAL, NOT WIRING — IS DECIDED BY
THIS FILE AND ITS REASON IS §3.**

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

## §3 — What was read, and what it established

**`src/c/main.c` AT `387d05c` WAS READ IN FULL (2626 lines).** `git diff --stat 16c3df7 387d05c
-- src/` is empty, so the read is valid at the code pin `16c3df7`.

- **`prv_ui_tick` (`main.c:568-574`)** clears `s_ui_timer`, returns when not recording, marks the
  canvas dirty and re-arms itself with `app_timer_register(1000, prv_ui_tick, NULL)`.
- **`s_ui_timer` (`main.c:291`)** is `static AppTimer *s_ui_timer = NULL;`.
- **BOTH ARE `static`, SO A FULL READ OF `main.c` IS AN EXHAUSTIVE CALL-SITE SURVEY.** No other
  translation unit can name either. **`prv_ui_tick` is named at its definition and at its own
  re-arm and nowhere else. `s_ui_timer` is named at its declaration and inside `prv_ui_tick` and
  nowhere else. NOTHING CALLS `prv_ui_tick`, SO THE CHAIN NEVER STARTS.** This discharges the
  call-site survey handoff-v73 §12 item 2 owed; the compiler's `-Wunused-function` warning was
  never that survey (RULE 1).
- **`grep -c 'prv_ui_tick' src/c/main.c` returns 2**, matching the read.
- **The clean rebuild of 2026-09-16 at `387d05c` emitted `main.c:568:13: warning:
  'prv_ui_tick' defined but not used [-Wunused-function]`.** The warning fires at the pin's
  source, which v73 recorded as NOT ESTABLISHED.

**THE HISTORY, READ FROM `git log -S'prv_ui_tick'` AND FROM `git show 6c8a041` IN FULL (891
lines):**

- **Both names entered at `6c8a041` (2026-07-21), "Initial Recovery app scaffold", whose message
  says it reuses hrv-monitor code and describes the recording screen as "static".**
- **The scaffold's `main.c` names them at the same four sites** (`git show 6c8a041:src/c/main.c`
  lines 21, 88, 89, 92), **and its `prv_start_recording` does not start the timer. `prv_ui_tick`
  HAS NEVER BEEN CALLED ON ANY BUILD OF THIS APP.**
- **The only other commits `-S` returns are `608fd85` and `387d05c`, both documents.**
- **Its only possible target was IDLE's `Dur m:ss` line.** IDLE is drawn only in `MODE_IDLE`
  (`prv_canvas_update`), and while recording the mode is `MODE_RECORDING`, whose screen
  (`prv_draw_recording`) shows `clock_copy_time_string` — hours and minutes — and is already
  redrawn by the minute tick (`prv_tick_handler`). **A one-second refresh has nothing to refresh.**
- **WHY IT WAS WRITTEN IS NOT ESTABLISHED.** No commit message or document states it. The
  hrv-monitor origin is what the scaffold message says and nothing more.

**WHY REMOVAL AND NOT WIRING.** Wiring the chain would add a redraw every second for the whole
night with nothing on the recording screen changing between minutes, and it would change runtime
behaviour on a measured night. **Removal changes no behaviour on any reachable path, because the
removed code is unreachable.**

## §4 — The change

**TWO DELETIONS IN `src/c/main.c` AND NOTHING ELSE:**

1. **The declaration line** `static AppTimer *s_ui_timer = NULL;` (`main.c:291`).
2. **The whole function** `prv_ui_tick` (`main.c:568-574`) **and the single blank line that
   follows it**, so `prv_start_recording` follows `prv_health_handler` with one blank line
   between, as every other function boundary in the file has.

**NO OTHER LINE IS TOUCHED.** No comment is added at the removal sites. **No other file changes.**
In particular `prv_health_handler`, whose closing brace sits immediately above the removed
function, **is not edited**, and `prv_start_recording` is not edited.

**`git diff --stat` MUST READ ONE FILE, 0 INSERTIONS, 9 DELETIONS** — one declaration line, the seven function lines `main.c:568-574`, and the blank line `main.c:575`. Any other count is a defect
in the edit and the edit is redone from `387d05c`, not patched.

## §5 — Verification, performed in full before the commit, in this order

1. **The diff is read in full** and contains exactly the nine deletions of §4.
2. **`src/c/storage.h` is read** and still sets `CLASSIFIER_SERIES 14` and
   `NIGHT_SUMMARY_VERSION 3`.
3. **`grep -rn 'prv_ui_tick\|s_ui_timer' src/` returns nothing.** Here an empty result IS the
   finding, because the survey in §3 already enumerated every site and this checks that each was
   removed — it is a check of the edit, not an absence claim about unread code.
4. **A clean build (`pebble clean && pebble build`) finishes successfully and the
   `-Wunused-function` warning for `prv_ui_tick` is gone.** Every other warning is compared line
   for line with the 2026-09-16 build at `387d05c`; **the RWX LOAD-segment linker warning is
   expected to remain and is recorded, not acted on.**
5. **THE IMAGE COMPARISON, THE SAME THREE-WAY STANDARD `3f41133` MET.** Build `387d05c` and the
   edited tree into separate directories and compare:
   - **`arm-none-eabi-size` on both `pebble-app.elf`.** `text`, `data` and `bss` are recorded for
     both.
   - **`cmp -l` on both `pebble-app.raw.bin`**, recording the count and offsets of differing
     bytes, and **`arm-none-eabi-readelf -n`** on both elf files to read each build-id.
   - **`arm-none-eabi-nm` on both `pebble-app.elf`**, recording whether `prv_ui_tick` and
     `s_ui_timer` appear in the `387d05c` image. **This is what says which outcome below is
     expected, and it is read before the sizes are interpreted.**
   - **THREE OUTCOMES ARE REGISTERED IN ADVANCE AS CONSISTENT WITH THIS SPEC:**
     **(a)** `text`, `data` and `bss` identical, and every differing byte lies inside the build-id
     — the compiler had already discarded the dead code; **(b)** `bss` exactly 4 bytes smaller,
     `text` and `data` identical — the pointer's storage was retained before and is gone now.
     **(c)** `nm` shows `prv_ui_tick` present in the `387d05c` image, `text` smaller by exactly
     that symbol's size (plus alignment, read from `nm -S`), and `bss` either unchanged or 4 bytes
     smaller as in (b) — the compiler had retained unreachable text. **In (b) and (c) the
     raw-image comparison is recorded as it reads and every size difference is accounted for from
     `nm -S`, not assumed.**
   - **ANY OTHER OUTCOME STOPS THE WORK.** It means something besides dead code changed, and it is
     diagnosed from source before anything is committed or flashed.
6. **The build is installed and the watch is observed** launching to IDLE, starting a recording,
   showing the recording screen, stopping it with the long SELECT, and walking RESULTS, HYPNO,
   DIAG through DIAG 7 and RUNS with nothing clipped. **That smoke session is not a night and is
   recorded as a test session in the ring** if it reaches 30 epochs; **it should be stopped before
   30 minutes so it does not save.**
7. **THE INCREMENT-SITE SOURCE READ IS RE-PERFORMED AT THE NEW PIN**, because the removed function
   sits immediately below `prv_health_handler` and N33-preregistration §4 requires the read to be
   redone if any commit touches the HRV event path. **The hunk borders it without entering it;
   the read is performed anyway and its result recorded against the new pin.**

## §6 — Comparability

**NO CLASSIFIER INPUT, NO STAGE DECISION, NO SMOOTHER INPUT, NO MINUTE TOTAL, NO STORED FIELD AND
NO PERSISTED STRUCTURE IS TOUCHED, AND NO REACHABLE PATH CHANGES.** **THE FIRST NIGHT AT THE NEW
PIN REMAINS COMPARABLE TO N26 ONWARD ON EVERY STAGE LINE.** The comparability argument is the
unreachability established in §3, **not the size of the change.**

**THE NEW PIN IS A CODE CHANGE, SO THE FIRST NIGHT ON IT IS A FRESH BASELINE ATTEMPT AND NOT A
NIGHT OF A REPLICATION RUN.** A run requires one pin with no code changes of any kind. **No run is
open, so none is broken.**

**NO PREDICTION IS MADE ABOUT POWER.** P4's band and battery's integer-percent resolution are
unchanged. **Under every registered outcome of §5 the removed code was never executed, so no power effect
is possible from its removal.**

## §7 — What this does NOT change

- **The IDLE screen's `if (s_recording)` branch** (`Dur m:ss`) is left exactly as it is, although
  §3 finds `MODE_IDLE` is not drawn while recording. **Whether that branch is reachable is a
  separate question and is not absorbed here.**
- **No persist return value is checked by this change** — handoff-v73 §12 item 4 stands as its own
  decision.
- **No constant, band, threshold, gate, divisor, counter or screen moves.**
- **No frozen spec is edited.**

## §8 — Rule 2, Rule 3 and Rule 11 checks

**RULE 2 — NO CONSTANT ENTERS OR LEAVES ANY PATH A DECISION READS.** The `1000` inside the
removed function was read by no decision and never executed.

**RULE 3 — THIS SPEC REGISTERS NO SCORED CRITERION.** Its §5 checks can fail: a diff other than
nine deletions, a surviving name, a surviving warning, or an image comparison outside the three
registered outcomes. **Each can also pass.**

**RULE 11 — NO FORMAT STRING IS ADDED OR CHANGED**, so no `strings` verification is owed. **The
new screens' strings verified on 2026-09-16 remain in the binary and §5 step 6 observes them
rendering.**

## §9 — Rule 7 check

**NO SUBJECT PHYSIOLOGICAL VALUE APPEARS IN THIS FILE.** It names commits, line numbers, symbols
and build warnings only. **Scan before the public commit regardless; this section is not a
substitute for it.**

## §10 — Self-consistency check, per RULE 19

**PERFORMED BEFORE THIS FILE IS FROZEN.**

**FORM 1, ONE QUANTITY DESCRIBED TWICE.** The deletion count is nine in §4 and in §5 step 1, and it enumerates as 1 + 7 + 1. The declaration line is `main.c:291` in §3 and §4, read from source by `grep -n` after an earlier draft of this file carried a wrong number — **the draft was never frozen, and the line was read before this file was.** The occurrence count is two in §3 for `grep -c`, which counts LINES containing `prv_ui_tick` (the definition and the re-arm), and four named sites in the scaffold for the two names together; **they agree.**

**FORM 2, SUBSTITUTED AT BOTH EXTREMES.** At a build where the compiler discarded everything
already, `nm` shows neither symbol and outcome (a) applies. At a build where it retained both,
`nm` shows both and outcome (c) applies with the 4-byte `bss` change. **Every size difference
between those extremes is accounted for from `nm -S` or the work stops. No statement fails at
either extreme.**

**FORM 3.** Checked in §8.

**FORM 4, PAIRINGS ACTUALLY RUN.**
- **§3 against §6**, because §6's comparability rests on §3's unreachability. **They agree.**
- **§4 against §5**, because §5 checks §4's diffstat. **They agree at nine.**
- **§5 step 6 against §6**, because a smoke session could enter the ring. **They agree; step 6
  directs it be stopped before 30 minutes.**

**NO DEFECT WAS FOUND BY THIS CHECK IN THE FILE AS FROZEN.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**

**NOTHING IS IMPLEMENTED FROM THIS FILE UNTIL IT IS FROZEN AND PUSHED.**
