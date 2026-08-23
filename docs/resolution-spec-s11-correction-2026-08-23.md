# resolution-spec-s11-correction-2026-08-23

**Corrects `hrv-resolution-readout-spec-v1` §11, frozen at `7a2f58d`. THE SPEC IS NOT EDITED.**
A change is a superseding dated file, never an edit.

**RULE 7: this file records no subject physiological value. The night readings that prompted it
are in `~/recovery-nights`.**

## What §11 registered

§11 records `rej_quality` as **a third rejection path** — `hrv_buf_add` rejecting on
`quality == 0` before the range gate, incrementing both `rejected` and `rej_quality` — notes
that `rsa-feasibility-readout-spec-v1` §1.5 names only the range and jump gates and is
therefore INCOMPLETE, and registers `rej_quality` for **its own dated spec** as "a real
discovery and a cheap one — one counter already maintained, one render line."

## What is correct in it, and what is not

**THE READ OF `hrv_math.c` WAS CORRECT.** The gate exists, it precedes the range gate, and it
increments both counters. **`rsa-feasibility-readout-spec-v1` §1.5 IS GENUINELY INCOMPLETE AS
AN ACCOUNT OF WHAT THE FUNCTION DOES, AND THAT PART OF §11 STANDS.**

**WHAT IS WRONG IS THE PREMISE THAT THE PATH CAN FIRE.**

**READ FROM `main.c` 2026-08-23, `prv_health_handler`, all three call sites:** every
`hrv_buf_add` call passes the literal `1` as `quality`. **THE GATE CANNOT FIRE FROM THIS CALL
SITE ON ANY BUFFER, ON ANY NIGHT.**

**AND THE LITERAL IS NOT A CHOICE THIS APP MADE. READ FROM THE SDK HEADER 2026-08-23** at
`~/.local/share/pebble-sdk/SDKs/4.33.1/sdk-core/pebble/emery/include/pebble.h`, the entire
app-facing HRV surface is three items — the `HealthEventHRVUpdate` event, the `uint16_t`
PPI peek, and the sample-period setter. **THERE IS NO QUALITY ACCESSOR AND NO CONFIDENCE
ACCESSOR.** The only occurrence of the word quality is a doc comment about sampling being
reduced due to sensor reading quality issues — **prose about scheduling, not an API.**

**THE FIRMWARE'S CONFIDENCE-TO-QUALITY MAPPING RECORDED IN §1 OF THE SAME SPEC EXISTS AT
DRIVER LEVEL AND IS NOT EXPOSED THROUGH THIS SDK.** So `1` is the only value any Pebble app can
supply. **THE PATH IS DEAD BY API, NOT BY OVERSIGHT.**

## What is corrected

**`rej_quality` IS NOT A LIVE THIRD REJECTION PATH. IT IS AN UNREACHABLE ONE.**

**THE OWED `rej_quality` READOUT SPEC MUST NOT BE WRITTEN AS §11 DRAFTS IT.** A readout of
`rej_quality` would render a permanent zero on every night. **THAT IS A NEW INSTRUMENT WHOSE
ONLY POSSIBLE READING IS A CRITERION THAT CANNOT FAIL — RULE 3's FIRST DEFECT FORM**, and
building it would spend a render line and a screen position on a value that carries no
information.

**THE QUEUE ENTRY IS NOT DELETED. IT IS RESTATED.** What is owed is no longer a readout. It is
a decision on one of three, and none of them is "render it":

1. **Leave the gate unreachable and record it as such.** Cheapest, and loses nothing that is
   currently obtainable.
2. **Establish whether the firmware's quality value can be surfaced to apps at all.** That is a
   firmware and SDK question, **NOT ESTABLISHED**, and `~/PebbleOS` is DO NOT TOUCH — firmware
   work is CLOSED, not deferred.
3. **Remove the gate.** **NOT RECOMMENDED AND NOT AUTHORISED BY THIS FILE.** It is unreachable,
   not wrong, and it would become live again the moment a real quality value existed.

## What this file does NOT do

**IT DOES NOT AUTHORISE ANY EDIT TO `hrv_buf_add`.** The gate stays.

**IT DOES NOT CLAIM THE FIRMWARE QUALITY VALUE WOULD BE USEFUL.** Nothing is known about what
it would contain.

**IT DOES NOT AFFECT ANY OTHER SECTION OF `hrv-resolution-readout-spec-v1`.** §1 through §10
stand as frozen, and the divisor counters, `Dm`, the identities and the §8 decision rule are
untouched. **`Gp` REMAINS CORRECT** — it counts discard-adjacent positions regardless of which
gate fired, exactly as §11 itself records.

## How this defect survived

**THE EVIDENCE WAS CARRIED FAITHFULLY AND THE PREMISE WAS NEVER CHECKED AGAINST THE CALL
SITE.** The gate was found by a correct read of `hrv_math.c`; what was never read was who calls
it and with what. **RULE 4's SECOND CLAUSE — RE-DERIVE THE PREMISE, NOT ONLY THE EVIDENCE.**

## Status

**Frozen on commit. A change is a superseding dated file, never an edit.**
