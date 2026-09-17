# vib-counter-blindness-2026-09-17

**Dated 2026-09-17. A DATED FINDING AND A SUPERSEDING CORRECTION, NOT AN EDIT. It supersedes the
CLOSURE CLAUSE of `movement-spec-v1.md` §5 — "IF Vib READS ZERO ACROSS THREE CONSECUTIVE NIGHTS
THIS ITEM CLOSES" — and NOTHING ELSE.** `movement-spec-v1` is unchanged and stays exactly as
frozen.

**FILED IN `~/recovery/docs/` — THE PUBLIC REPO. NO SUBJECT PHYSIOLOGICAL VALUE APPEARS IN THIS
FILE.** It names firmware source, app source and two on-watch control readings of a counter.

**IT CHANGES NO CODE, NO DECISION, NO BAND AND NO STORED FIELD.**

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

## §3 — The finding: on this watch, `Vib` cannot count

**`Vib` is `s_vibe_samples`, incremented when an app sample carries `did_vibrate`
(`src/c/main.c:136` at `bc52c03`).** The increment is not gated on recording and is reset only at
session start (`main.c:602`).

**READ FROM PebbleOS SOURCE AT `v4.36.2` (`f1a41a46`), public repository `coredevices/PebbleOS`:**

1. **An app sample's `did_vibrate` is `sys_vibe_history_was_vibrating(timestamp_ms)`**
   (`src/fw/applib/accel_service.c:121`).
2. **The vibe history records a vibration only while one global flag,
   `s_vibe_history_enabled`, is true** (`src/fw/services/vibe_pattern/service.c:64, 113-116,
   141-144`). Every motor on/off transition, from any source, passes through
   `prv_vibes_set_vibe_strength` and records into it when the flag is true (`:243-275`).
3. **The flag is set true ONLY when the accel data subscriber list goes from EMPTY to non-empty**
   (`src/fw/services/accel_manager/service.c:479-483`), and set false when the list becomes empty
   (`:541-544`).
4. **The flag is ALSO set false, unconditionally, whenever an app exits**
   (`src/fw/process_management/app_manager.c:420`, in `prv_app_cleanup`), **whether or not other
   subscribers remain.**
5. **The activity service holds an accel subscription for as long as activity tracking runs** —
   subscribed at `src/fw/services/activity/activity.c:848` when tracking starts, unsubscribed at
   `:893` when it stops. **The heart-rate manager also subscribes while the sensor is on**
   (`src/fw/services/hrm/hrm_manager.c:277`).

**THEREFORE, WITH ACTIVITY TRACKING RUNNING, THE SUBSCRIBER LIST IS NEVER EMPTY.** The flag is set
once, at the first subscription after boot. **The first app exit after that — including the
launcher's, when any app is opened — clears it, and no later event can set it again.** From then
on, **every app sample carries `did_vibrate = false` and `Vib` reads zero whatever the motor does.**

**The same mechanism is present at `v4.33.2`:** `activity.c` is unchanged between the two tags
(`git diff --stat` empty), and `app_manager.c:420`, `accel_manager/service.c:482` and `:543` carry
the same `sys_vibe_history_*` calls at `v4.33.2`. **N29 through N32 ran `v4.33.2`; N33 ran
`v4.36.2`.** Earlier firmware versions in the series were not read, and no claim is made about
them.

**TWO THINGS WERE READ AND RULED OUT AS ALTERNATIVE CAUSES:**
- **Focus.** Nothing in the app-side delivery (`accel_service.c`) or the kernel dispatch
  (`accel_manager/service.c:252-343`) pauses delivery while a notification window has focus.
- **Clock mismatch.** Sample times and vibe-history times both come from `rtc_get_time_ms`
  (`src/fw/drivers/imu/lsm6dso/lsm6dso.c:237-244`; `vibe_pattern/service.c:125, 152`). **A
  separate, smaller defect was found and is recorded, not relied on:** a FIFO batch is stamped
  at read-out (`lsm6dso.c:413`) and that stamp is given to the batch's FIRST sample
  (`accel_manager/service.c:835-840`), so sample times run late by up to about one batch length.
  The batch is set by the most frequent subscriber (`service.c:171-200`); with the heart-rate
  manager subscribed (25 Hz, 4 samples, `hrm_manager_private.h:52`) that is about 160 ms.

## §4 — The on-watch controls

**2026-09-16 and 2026-09-17, `bc52c03` installed, firmware `v4.36.2`.** On two separate launches
of Recovery, a recording was started, **the watch was made to vibrate by a notification while
Recovery stayed open**, the recording was stopped, and DIAG 2 was read. **`Vib` read 0 both
times.** Neither session reached 30 epochs and neither was saved.

**BOTH CONTROLS FAILED, AS §3 PREDICTS.** The controls confirm the counter did not count; **§3,
not the controls, is what establishes why.**

## §5 — What this supersedes and what follows

**THE CLOSURE CLAUSE OF `movement-spec-v1` §5 IS SUPERSEDED.** Three consecutive zeros cannot close
an item whose counter is structurally unable to count on this firmware with activity tracking
running. **P-VIB's standing count is VOID** — N32's and N33's zeros, and every earlier `Vib` zero
in the series, are **NOT EVIDENCE THAT NO VIBRATION OCCURRED.** **No night is re-scored; each
earlier reading stands as recorded and is read under this file.**

**P-VIB IS RECORDED RAW, NOT SCORED, AND CANNOT CLOSE** until a dated file registers a counter or
a procedure that can be shown to count. **A positive control — a known vibration during a
session registering as `Vib > 0` — is the minimum any such file must require.**

**THE UNDERLYING QUESTION IS NOT ANSWERED BY THIS FILE:** whether vibrations reach the movement
counters on a night. **The standing evidence on it is the night file's
`ctx_notifications_overnight` field and DIAG 2's `QT` count**, neither of which reads the motor.

**NOTHING IN THE APP CAN FIX THIS.** The flag is kernel state, set and cleared by the firmware.
**A FIX IS FIRMWARE WORK** — for example, clearing the flag at app exit only if no data subscriber
remains, or setting it when any app subscribes. **Whether to report it upstream is a separate
decision and is not made here.**

## §6 — Rule 2, Rule 3 and Rule 7 checks

**RULE 2 — no constant, band, threshold or code path is touched.** **RULE 3 — this file registers
no scored criterion. It withdraws one.** **RULE 7 — no subject physiological value appears here.
Scan before the public commit regardless.**

## §7 — Self-consistency check, per RULE 19

**FORM 1.** The firmware tag is `v4.36.2` (`f1a41a46`) throughout §3 and §4; `v4.33.2` appears only
where §3 states what was diffed at it. The control count is two in §4 and in §5.
**FORM 2.** At a boot where no app has yet exited, §3 predicts the flag is true and `Vib` could
count; **no night in this series starts in that state**, because opening Recovery follows an app
exit. **No statement fails at either extreme.**
**FORM 3.** §6.
**FORM 4.** §3 against §4, because §4's result is what §3 predicts. **They agree.** §5 against
`movement-spec-v1` §5, because §5 supersedes one clause of it. **Only the closure clause is
superseded; the counter, its render and its reset stand.**

**NO DEFECT WAS FOUND BY THIS CHECK IN THIS FILE.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**
