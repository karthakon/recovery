# epoch-position-readout-spec-v1

**Dated 2026-10-13. Written after N36 was scored and filed (`recovery-nights` `5a56c63`), from a
source read at `b425a3b` (`src/c/main.c` md5 `256fff8aa5d4b492a569e6afb5eb9845`, `src/c/smoother.c`
md5 `dc9512acaa59cff7b1d6429bd1d20261`, `src/c/storage.h` md5 `0e6c8038b61e5df4e3c5b0fda744c040`,
`src/c/storage.c` md5 `06796c34ca0bdba0a35ba1327dcb182e`). Every line number below is at that pin.**

**FILED IN `~/recovery/docs/` — THE PUBLIC REPO. RULE 7 CHECK PERFORMED IN §13: NO SUBJECT
PHYSIOLOGICAL VALUE APPEARS IN THIS FILE.**

**INSTRUMENT ONLY. NO CLASSIFIER INPUT, NO STAGE DECISION, NO TOTAL, NO STORED FIELD, NO PERSISTED
STRUCTURE.** `CLASSIFIER_SERIES` STAYS 15 AND `NIGHT_SUMMARY_VERSION` STAYS 3, **TO BE VERIFIED BY
READING `src/c/storage.h` AFTER IMPLEMENTATION, NOT ASSERTED.**

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

## §3 — Why this exists

**EVERY INSTRUMENT IN THE APP REPORTS HOW MANY MINUTES, NONE REPORTS WHICH.** The Awake clause
counters, `C2s`/`C2n`, `AwO`, `AwMx`/`AwP`/`AwR`/`Aw5`, `RemN` and the transition counters are
counts and run statistics. `awake-runs-readout-spec-v1` §7 registered the gap in advance: *where the
runs fall* needs the per-minute sequence, which that spec did not build. `prv_draw_hypno`
overpaints when epochs exceed plot pixels (`epoch-readout-spec-v1-transition-counters-2026-08-16`
§1), so the hypnogram is a sample and no position may be read off it.

**SINCE `awake-stillness-gate-spec-v1` THE GAP DECIDES WHAT A NIGHT CAN ESTABLISH.** The first
series-15 night's record leaves, as NOT ESTABLISHED for want of a render, each of the following:

1. where the pre-smoother Awake minutes sat;
2. where the minutes the stillness gate removed from the stage decision sat — in particular whether
   any fell inside the self-reported awake window after the press, which is the question of whether
   the gate deepened the onset under-call (gate 6's opposite-failure branch);
3. which minutes the smoother promoted to REM;
4. whether any reported Awake minute coincides with a self-reported wake.

**ALL FOUR ARE POSITION QUESTIONS. A CLASSIFIER CHANGE ARGUED BEFORE THEY CAN BE ANSWERED WOULD REST
ON AN UNREAD PREMISE.** This file builds the render; it changes nothing it renders.

## §4 — The four series

**A SERIES IS A SET OF EPOCH INDICES. IT IS RENDERED AS ITS MAXIMAL RUNS OF CONSECUTIVE INDICES, EACH
AS `start+length`, IN ASCENDING ORDER OF `start`.** `start` is the epoch index, which is minutes from
the press (one epoch is one minute, `prv_close_minute`). Converting to clock time is arithmetic at
scoring time against the recording start, and is not rendered (RULE 6 permits the derivation
because both inputs are recorded).

| Page | Series | Membership of epoch `i` | Source | Survives app exit |
|---|---|---|---|---|
| `Pos G` | **gate-suppressed c2** | `c2 && c2_in_run` in `prv_awake_redecide` | NEW RAM bitmap, §5 | **NO** — capture live |
| `Pos A` | **pre-smoother Awake** | `rec.reserved == StageAwake` | stored epochs | yes, until the next recording |
| `Pos W` | **post-smoother Awake** | `rec.stage == StageAwake` | stored epochs | yes, until the next recording |
| `Pos R` | **smoother-promoted REM** | `rec.stage == StageREM && rec.reserved != StageREM` | stored epochs | yes, until the next recording |

**WHY THESE FOUR AND NO OTHERS.** G answers §3 item 2. A answers item 1; because after
`prv_awake_redecide` the Awake set is exactly `c1 || c2_eff` (main.c 966, 1002) and
`prv_base_redecide` skips Awake, A is the decision's Awake set at stop, which `smoother.c` 265
preserves in `reserved`. R answers item 3. W answers item 4 and is what RESULTS and the hypnogram
report. **The minutes the smoother removed from Awake are A minus W and are derivable at scoring
time from the two pages, so no fifth series is built.** The REM minutes the smoother demoted are
rendered as a count only (§6), because no open question asks where they sat; **a later question
that does is a later dated spec.**

**WHAT G IS NOT.** G is not "minutes that would have been Awake without the gate". c1 can also mark a
G minute; such a minute is in A as well. **Whether a G minute became Awake by another route is read
by intersecting G with A at scoring time, never assumed.**

**RULE 9.** No page label and no value label reuses an existing label on any screen. `Pos` names no
existing quantity; `G`, `A`, `W`, `R` appear only as page suffixes and never as value labels.
**`A` here is not `AwO`, `Ah` or `AH`; `R` here is not `RemN`.** The value labels on these pages are
`n`, `m`, `rf`, `dem` and `p`, defined in §6, none used elsewhere.

## §5 — The one new static, and its single write site

**ONE RAM-ONLY BITMAP:** `static uint8_t s_epoch_c2g[(EPOCH_VAR_MAX + 7) / 8];` — 120 bytes,
declared beside `s_epoch_in_run` (main.c 258), the same shape and indexing.

**RESET** in `prv_start_recording`, beside `memset(s_epoch_in_run, …)` (main.c 580), with a
trailing `epoch-position-readout-spec-v1 s5` comment, matching the local convention.

**WRITTEN AT EXACTLY ONE SITE: inside the existing branch at main.c 984 that increments
`s_c2_still`, under the same condition and no other.** That branch sits inside
`if (c2 && s_onset_epoch_idx >= 0 && i >= (uint16_t)s_onset_epoch_idx)` (main.c 982) and tests the
same `s_epoch_in_run` bit that `c2_in_run` (main.c 963) reads. **So the bit is set for precisely the
minutes `C2s` counts, by construction, and `C2s` is the count of set bits.** The increment and the
bit-set are made one statement block:

```c
if ((s_epoch_in_run[i >> 3] & (uint8_t)(1 << (i & 7))) != 0) {
  s_c2_still++;
  s_epoch_c2g[i >> 3] |= (uint8_t)(1 << (i & 7));   // epoch-position-readout-spec-v1 s5
}
```

**NOTHING READS `s_epoch_c2g` EXCEPT THE RENDER.** It is not read by `c2_eff`, by any decision, by
any counter or by any persist call. **It is never written to storage**: `NightSummary` and
`EpochRecord` do not change.

**WHY THE WRITE IS NOT AT THE `c2_eff` SITE (main.c 964).** That line runs for every minute
including pre-onset ones. c2 is structurally false before onset at this pin — `s_epoch_ahr` is
zeroed at main.c 837 and filled only for `m >= onset` (main.c 844, 871), and c2 requires
`a_hr_m > 0` (main.c 954–955) — so the two sites would agree. **Placing the write at 984 makes the
agreement with `C2s` a property of one block rather than an argument across three line ranges**, and
it stays exact if a later spec makes c2 reachable before onset.

**THE STORED-EPOCH SERIES (A, W, R) NEED NO NEW STATE.** They are computed at render time from
`storage_epoch_read`, exactly as `prv_compute_runs` computes RUNS (main.c 2105), read-only, **never
`storage_epoch_update`.**

## §6 — Render

**EACH SERIES IS ONE OR MORE PAGES. EVERY PAGE HAS ONE HEADER-VALUE LINE AND UP TO EIGHT RUN LINES,
THREE RUNS PER LINE, SO 24 RUNS PER PAGE.** A series with more than 24 runs continues on further
pages of the same series. **NO RUN IS EVER DROPPED, TRUNCATED OR SUMMARISED, AND THE FONT IS NOT
SHRUNK.** A series with zero runs has exactly one page.

**TITLE** (`GOTHIC_24_BOLD`, 26 px advance, as DIAG 2): `Pos G`, `Pos A`, `Pos W`, `Pos R`.

**HEADER-VALUE LINE** (`GOTHIC_14`, 18 px):

- G: `n <runs>  m <minutes>  p <page>/<pages>`
- A and W: `n <runs>  m <minutes>  rf <failed reads>  p <page>/<pages>`
- R: `n <runs>  m <minutes>  dem <k>  p <page>/<pages>`, where `dem` is the count of epochs with
  `rec.reserved == StageREM && rec.stage != StageREM`.

`rf` is the count of epochs in `0 .. storage_epoch_count()-1` whose `storage_epoch_read` failed.
**A FAILED READ BREAKS A RUN AND NO EPOCH IS FABRICATED**, the convention of the transition counters
(`epoch-readout-spec-v1-transition-counters-2026-08-16` §2), not the bridging convention of the REM
and Awake run statistics: a position readout that bridged an unread epoch would render a position
nobody read. **`rf` is rendered on A and W so a nonzero count is visible; R shares the same pass and
the A/W value applies to it, so it is not repeated.**

**RUN LINES** (`GOTHIC_14`, 18 px): up to three `start+length` items separated by two spaces, e.g.
`s+l  s+l  s+l`.

**THE ARITHMETIC, FROM SOURCE AND NOT ASSUMED.** Title 26 px from `y = 2`, then nine lines at 18 px,
ends at `y = 190` — **exactly DIAG 2's height, verified on the watch 2026-08-20 with nothing clipped
at either end** (`prv_draw_diag2`, nine `GOTHIC_14` lines after a 26 px title). **HEIGHT is
therefore within a proven bound. WIDTH IS NOT:** the longest possible run line is three items of
`959+959`, 23 characters, and no existing screen renders a line that long. **THE WIDTH MUST BE
VERIFIED ON THE WATCH WITH A LINE OF THAT LENGTH BEFORE ANY NIGHT IS RECORDED UNDER THIS SPEC**
(§12). **REGISTERED FALLBACK, IF THE WIDTH CHECK FAILS: two runs per line and 16 runs per page.**
Nothing else changes in the fallback, and it is applied in the implementing commit before any
night, not after.

**UNDEFINED AND REAL ZERO** (`undefined-value-display-rule-2026-08-27`):

- **G** renders `n --  m --` and no run lines when `s_session_start == 0` — §3.1 case 2, no session
  in this process. **This is the state after any app exit, and it is why G is captured live.**
- **A, W, R** render `n --  m --` and no run lines when `storage_epoch_count() == 0`.
- **A defined `n 0  m 0` is a REAL ZERO and is a finding** (§3.2): the series was computed and is
  empty. It is not the same state as `--`.

**NO FRACTION, RATE OR CLOCK TIME IS RENDERED.** Every input of every derivation is on these pages or
on RESULTS, RUNS, DIAG 3 and HISTORY.

**NAVIGATION IS AN IMPLEMENTATION DECISION, RECORDED IN THE COMMIT MESSAGE**
(`epoch-readout-spec-v1-nav-correction` §2, `measurement-spec-v1` §3.6). The constraints are: every
page of every series must be reachable after stop **and** from IDLE on a cold launch (A, W and R are
needed then); the existing routes into RUNS, DIAG and HISTORY do not change; DIAG 5's SELECT handler
is not touched. **The page order is fixed: G pages, then A, then W, then R.**

## §7 — What does NOT change

- **No decision, no threshold, no window, no stage, no minute total.** `c2_eff` and both decision
  sites (main.c 966, 1002) are untouched.
- **No `EpochRecord` field, no `NightSummary` field, no persist key.** `storage.c` and `storage.h`
  are not edited.
- **`smoother.c`, `sleep_stage.c`, `hrv_math.c` are not edited.**
- **Every existing screen renders exactly what it rendered.** RUNS, DIAG 2 and DIAG 3 are
  unchanged.
- **No constant is introduced.** 24 and 3 are layout counts read by no decision (§11).
- **`git diff --stat` must name `src/c/main.c` alone.**

## §8 — Registered checks, each citing the lines that compute both sides

**THESE ARE CONDITIONING CHECKS ON THE INSTRUMENT. NONE IS A BASELINE GATE. A PASS IS ABOUT
PLACEMENT AND SAYS NOTHING ABOUT SLEEP.**

**P-POSG — `m` on `Pos G` (summed over its pages) EQUALS `C2s` (DIAG 3).** Both are produced in the
one block of §5 at main.c 984. **A CONSTRUCTION IN RULE 20's SENSE, NOT A CORROBORATION.** A
mismatch means the write is misplaced, or a page dropped a run, and is diagnosed before anything else
on `Pos G` is read. **IT IS ONE-SIDED IN THE SAME WAY P-DRIDENT IS:** a write moved under a narrower
condition AND an increment moved with it would still agree. **The uncovered side is closed by a
source read at the pin, per night: the bit-set sits inside the main.c 984 branch, beside
`s_c2_still++`, under no other conditional.**

**P-POSA — the `Pos A` minutes at indices `>= Ons` (RUNS) EQUAL `AwO` (RUNS).** `AwO` is counted at
main.c 2142 over `reserved == StageAwake` and `t >= s_onset_epoch_idx`; `Pos A` reads the same field.
**A construction where no read fails.** **IT CAN DIFFER ONLY THROUGH `rf`**, because `AwO` skips a
failed read and so does `Pos A`; with `rf 0` a mismatch is an instrument defect. **Scoreable only
while `Ons` is defined**, i.e. on the live capture; `AwO` reads `--` after app exit.

**P-POSW — `m` on `Pos W` EQUALS RESULTS `Awake`.** RESULTS renders `s_mins[StageAwake]`, set by
`smoother_run`'s `out_mins[rec.stage]++` (`smoother.c` 268) for every epoch it wrote back; `Pos W`
reads the stored `stage` back. **THIS IS NOT A CONSTRUCTION: it can fail**, if an
`storage_epoch_update` at `smoother.c` 267 did not persist or a later read fails, and a failure is a
finding about storage.

**P-POSR — RESULTS `REM` EQUALS `RemN` (RUNS) − `dem` + `m` on `Pos R`.** Post-smoother REM minutes
are pre-smoother REM minutes, less those the smoother demoted, plus those it promoted. **Arithmetic
over one pass of stored epochs; it can fail by the same storage route as P-POSW.**

**NOT REGISTERED, AND WHY (RULE 20):** `Pos A` minutes against the six clause counters (the
P-AWO relation on series 15 is already `AwO = six-counter sum − C2s`, and `Pos A` adds no new
population to it); G against `C2n` (`C2n − C2s` is the ungated c2 count reaching the decision, which
§5 does not record); any check between G and A, which is a reading, not an identity.

## §9 — How a reading is taken, REGISTERED BEFORE ANY READING EXISTS

**NO BAND, NO THRESHOLD AND NO EXPECTED VALUE IS PLACED ON ANY POSITION.** Positions are recorded.
**A pre-registration may register a criterion against them only after they have read at least
once**, the restriction `awake-runs-readout-spec-v1` §9 and `stillness-run-readout-spec-v1` §9
placed on themselves.

**THE READINGS THIS RENDER MAKES POSSIBLE, NAMED SO THEY ARE NOT INVENTED LATER:**

1. **Whether any `Pos G` run overlaps the self-reported awake window after the press**, converted to
   epoch indices from the self-reported latency. **No tolerance is registered here; the next
   pre-registration registers one, with its derivation, before the night it is scored on.**
2. **Whether any `Pos W` run (or `Pos A` run) lies near a self-reported wake.** Same rule.
3. **Where `Pos R` runs lie relative to label-onset `OnsL` and the smoothing-spec-v2 latency rows**
   (180 and 300 minutes after onset).
4. **G ∩ A** — gate-suppressed minutes that became Awake anyway through c1.

**A SELF-REPORT IS A REPORT, NOT GROUND TRUTH.** A coincidence or a miss is recorded as one. **NO
BRANCH OF ANY READING MOVES A CONSTANT, A BAND, THE GATE, OR THE SMOOTHER.** Any change argued from
these readings is its own dated pre-registration.

## §10 — What this CANNOT establish

**WHETHER ANY MINUTE IS CORRECTLY STAGED.** Positions describe the classifier's output. Garmin is not
ground truth and neither is the self-report.

**WHAT AN UNGATED NIGHT WOULD HAVE LOOKED LIKE.** The smoother decodes the whole night jointly; the
ungated decode is not computed and is not derivable from G.

**WHY A MINUTE IS IN G.** G records that c2 fired inside a stillness run; it does not separate REM
from quiet wakefulness (`stillness-run-readout-spec-v1` §7).

**ANYTHING ABOUT A PAST NIGHT FOR G.** G is RAM-only and reads forward only. **A, W and R for the
LAST night are readable after this build is installed, until the next recording starts**
(`epoch-readout-spec-v1-transition-counters-2026-08-16` §6) — **and only if that night's epochs were
written back by `smoother_run`.** A session killed before stop leaves `reserved` at the 0 written in
`prv_close_minute`, which equals `StageAwake`; **A and R are then meaningless and W is the live
stage, a limitation `epoch-readout-spec-v1` §2 already registers for RUNS.**

**ANYTHING FROM ONE NIGHT.**

## §11 — Comparability and cost

**INSTRUMENT ONLY. NO OUTPUT CHANGE.** **`CLASSIFIER_SERIES` STAYS 15 AND `NIGHT_SUMMARY_VERSION`
STAYS 3, VERIFIED BY READING `src/c/storage.h` AFTER IMPLEMENTATION.** The stage, BASE and RRI
comparability keys are unaffected. **A night under this spec is comparable to N36 on every stage
line, and is not a night of a replication run** — a run requires one pin with no code change of any
kind, instrument-only included. It is a fresh series-15 baseline attempt at a new pin.

**COST, REGISTERED HONESTLY:** 120 bytes of BSS; one OR into a byte per gated minute at stop; at
render, one pass of `storage_epoch_read` over the stored epochs per page draw, the same order of work
`prv_compute_runs` already does on every RUNS draw. **NO PREDICTION IS MADE ABOUT POWER**, P4's band
is unchanged, and the DIAG 5 timing readout must not be cited toward it. **The build's memory
footprint is reported at the build and compared with `b425a3b`'s; a heap or stack failure stops the
night.**

**24 AND 3 ARE LAYOUT COUNTS, NOT CONSTANTS UNDER RULE 2:** 3 is the run items per line the width
check verifies and 24 = 3 × 8 run lines follows from the proven nine-line height. **No decision
reads either. The fallback in §6 changes them before any night, not in response to one.**

## §12 — Build, verify, and capture

1. **Implement** per §5–§6 in one commit on `~/recovery` main; `git diff --stat` names
   `src/c/main.c` alone. **Read `src/c/storage.h` after the edit:** `CLASSIFIER_SERIES 15`,
   `NIGHT_SUMMARY_VERSION 3`.
2. **RULE 11 — new format strings are verified in the built ELF** (`strings` on the app binary,
   each new format string present exactly as written).
3. **`pebble build` clean**, with only the pre-existing unused-function and RWX-LOAD-segment
   warnings and NO new warning; footprint recorded beside `b425a3b`'s.
4. **Reinstall (app code changed).** **Before starting any recording, open `Pos A`, `Pos W` and
   `Pos R` from IDLE and photograph every page: they render the LAST recorded night's stored
   epochs, which the next recording destroys.** `Pos G` reads `--` then, by §6.
5. **Width check.** No test session is recorded for it: a session of 30 epochs or more saves a
   non-night to the ring, which is forbidden, and a shorter one leaves nothing worth checking. **The
   width is verified on the first page that renders a full three-item run line**, whether in step
   4's read or on the night's own capture. **A clipped run line on a night's capture voids that page
   for that night only**, recorded as a capture deviation, and §6's fallback is applied before the
   following night.
6. **Capture sequence (the next pre-registration owns it):** after RUNS, every page of `Pos G`,
   `Pos A`, `Pos W`, `Pos R`, in that order, **live, before the app is exited.**

## §13 — Rule 2, Rule 3 and Rule 7 checks

**RULE 2 — NO CONSTANT ENTERS ANY DECISION PATH.** `STILL_RUN_MIN`, the 103, `AW_MOVED_MIN`, the
smoother constants and the gate are read, never moved. 24 and 3 are layout counts (§11).

**RULE 3 — FOUR CHECKS ARE REGISTERED (§8) AND EACH CAN FAIL AND CAN PASS.** P-POSG fails on a
misplaced write or a dropped run; P-POSA on a misplaced read or a dropped run; P-POSW and P-POSR on a
storage write-back or read failure. **P-POSG and P-POSA are constructions where no read fails and
are stated as such; they are placement checks, not evidence.** **EVERY RENDERED VALUE HAS A RENDER
SITE** (§6) on pages the capture sequence captures.

**CHECKED AGAINST THE THIRD VACUITY FORM — A FINDING THAT MEANS SOMETHING ELSE. THREE ARE
REGISTERED:**

1. **A `Pos G` run read as "would have been Awake".** §4 forbids it: c1 may have made it Awake
   anyway, and the smoother may have absorbed it.
2. **A `Pos W` run coinciding with a self-reported wake read as validation.** §9 and §10 forbid it.
3. **A P-POSG or P-POSA pass read as evidence about sleep.** §8 forbids it.

**RULE 7 — NO SUBJECT PHYSIOLOGICAL VALUE APPEARS IN THIS FILE.** §3 names what a past night's record
leaves NOT ESTABLISHED and states no count, no index, no time and no ratio from it. The 959 in §6 is
the maximum epoch index from `EPOCH_VAR_MAX` 960, a code constant. **The check is over what the file
permits a reader to derive, not only over what it states. SCAN BEFORE THE PUBLIC COMMIT REGARDLESS;
THIS SECTION IS NOT A SUBSTITUTE FOR IT.**

## §14 — Self-consistency check, per RULE 19

**FORM 1, ONE QUANTITY DESCRIBED TWICE.** The new static is 120 bytes in §5 and §11, and
`(960 + 7) / 8 = 120`. Runs per page are 24 in §6 and §11, and 3 × 8 = 24. The page height ends at
`y = 190` in §6, and 2 + 26 + 9 × 18 = 190. The write site is main.c 984 in §5, §8 and §13. The four
page labels match in §4, §6 and §12.

**FORM 2, SUBSTITUTED AT BOTH EXTREMES.** **At a night with no onset:** `s_epoch_ahr` stays zero, c2
never fires, G is a real `n 0 m 0` with `C2s` reading `--` — **P-POSG is then NOT PERFORMED, not
passed**, because `C2s` is undefined; P-POSA likewise. A, W and R still render. **At a night of 960
epochs:** the largest start index is 959 and the largest run length is 960, so no item exceeds seven
characters and §6's 23-character figure (`959+959` three times) is an upper bound no line exceeds.
**No statement fails at either extreme.**

**FORM 3, EVERY BRANCH REACHABLE.** Each page's `--`, real-zero and populated states are reachable;
continuation pages are reachable at more than 24 runs; the §6 fallback branch is reachable on a
failed width check.

**FORM 4, PAIRINGS ACTUALLY RUN.**
- **§5 against §8 P-POSG:** §5 puts the write in the `s_c2_still` block and §8's source read checks
  exactly that. **They agree.**
- **§6 `rf` against §8 P-POSA:** both runs skip failed reads, and §8 names `rf` as the only route to
  a mismatch. **They agree.**
- **§6 undefined rules against §12 step 4:** `Pos G` reads `--` on a cold launch, and step 4 does not
  ask for it then. **They agree.**
- **§10's killed-session limit against §12 step 4:** the pre-recording read of A, W, R is valid only
  for a night that stopped normally, and §10 says so. **They agree.**
- **§11 against §7:** no output change, `main.c` alone. **They agree.**

**ONE DEFECT WAS FOUND AND FIXED BEFORE FREEZING:** a first draft of §12 step 5 proposed a test
session to check the width, which would either save a non-night to the ring (30 epochs or more) or
check nothing. **It is replaced by the check-on-first-render rule now in step 5.** No other defect
was found.

## Status

**DRAFT until Kristopher freezes it. Frozen on commit to `~/recovery` and push. A change is then a
superseding dated file, never an edit.**

**NOTHING IS IMPLEMENTED FROM THIS FILE UNTIL IT IS FROZEN AND PUSHED. N37 IS NOT RECORDED UNTIL THE
BUILD IS INSTALLED, ITS PAGES VERIFIED, AND N37's PRE-REGISTRATION FROZEN.**
