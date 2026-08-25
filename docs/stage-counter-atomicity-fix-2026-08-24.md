# Stage-counter atomicity fix, 2026-08-24

**Frozen BEFORE the code change is applied. A DATED SPEC, NOT AN EDIT TO ANY FROZEN FILE.**

**IT SPECIFIES ONE CHANGE TO `src/c/main.c` AND NOTHING ELSE.** No band moves, no parameter moves,
no decision changes, no persisted structure changes.

**IT ALSO RECORDS A CONSEQUENCE FOR THE BASELINE CONDITION'S GATE 7**, which
`baseline-condition-correction-2026-08-24.md` (`f7cc75d`) is frozen against and therefore cannot
state. **That file is not edited. This is the further dated file.**

---

## RULE 18 — THERE IS NO TIME PRESSURE. THIS BLOCK IS REPRODUCED VERBATIM IN EVERY DOCUMENT.

**Frozen in `resolution-extension-s6-correction-2026-08-24.md` §10 (`6f7ccb3`). It is copied
word for word into every handoff, spec, extension, correction and pre-registration from
2026-08-24 forward. IT IS NOT SUMMARISED, NOT PARAPHRASED, NOT ABBREVIATED AND NOT REPLACED BY A
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

---

## RULE 21 — IF A READ CAN SETTLE A QUESTION, THE READ IS PERFORMED.

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

---

## 1. The invariant

**`sum(s_mins)` IS INTENDED TO EQUAL THE STORED-EPOCH COUNT.**

`main.c:383` credits exactly one minute to `s_mins[st]` immediately before
`storage_epoch_write(&rec)` at `main.c:384`. **Every subsequent write to `s_mins` in the two
re-decision passes is a MOVE between buckets, never a creation or a destruction.** A move is
`-1` from the bucket the epoch is currently credited to and `+1` to the bucket it is moving into.

**P-MINS reads this invariant.** `Awake + REM + Light` from RESULTS against `Ep` from RUNS.
**`s_mins[StageDeep]` is excluded because `main.c:1139` overwrites it wholesale from
`health_service_sum`, so it is not part of the conserved sum.**

---

## 2. The defect, READ FROM SOURCE

**BOTH RE-DECISION PASSES GUARD THE DECREMENT AND LEAVE THE INCREMENT UNGUARDED.**

`main.c:939-940`, in `prv_awake_redecide`:

```
    if (s_mins[rec.stage] > 0) s_mins[rec.stage]--;
    s_mins[ns_stage]++;
```

`main.c:1064-1065`, in `prv_base_redecide`: **byte-identical text.**

**IF `s_mins[rec.stage]` IS ALREADY ZERO, THE DECREMENT IS SKIPPED AND THE INCREMENT STILL
FIRES.** **The move manufactures a minute, and `sum(s_mins)` exceeds the stored-epoch count by
one for every such occurrence.**

**THE FIVE LINES ARE IDENTICAL AT BOTH SITES**, which is recorded here because any apply script
must anchor wider than the changed lines to match each site exactly once.

**THIS IS NOT A NEW FINDING.** `N21-preregistration.md` §P-MINS registers it: the left side is
three counters mutated at stop, **`main.c` guards the decrement while incrementing
unconditionally — an asymmetry that inflates the sum if a counter is already zero** — and
`prv_awake_redecide` is a second pass with the same pattern. **It further registers that P-MINS
is the only registered detector of that class of fault.** **`construction-sweep-2026-08-24.md` §7
presented it as newly found; `baseline-condition-correction-2026-08-24.md` §11 corrects that.**

---

## 3. The fix

**AT BOTH SITES, THE DECREMENT AND THE INCREMENT BECOME ATOMIC.**

Replacement text, identical at both:

```
    if (s_mins[rec.stage] > 0) {
      s_mins[rec.stage]--;
      s_mins[ns_stage]++;
    }
```

**THE MOVE NOW HAPPENS IN FULL OR NOT AT ALL.** **The move can no longer be the cause of a
mismatch between `sum(s_mins)` and the stored-epoch count.**

**WHY THIS FORM AND NOT THE ALTERNATIVE.** The other candidate fix is to remove the decrement's
guard, making the pair unconditional and letting the counter underflow. **That is rejected:
`s_mins` is `uint16_t` (`main.c:260`), so an underflow wraps to 65535 rather than going negative,
and a wrapped counter is a worse failure than an inflated one — it is unbounded and it corrupts
every stage line at once.** **The guard stays; the increment joins it.**

**ON EVERY REACHABLE PATH THIS IS A NO-OP.** Reaching the guard's false branch requires
`s_mins[rec.stage]` to be zero while an epoch is still credited to `rec.stage`, which means
`s_mins` has ALREADY diverged from storage before the move. **No reachable state produces that
divergence** — the credit at `main.c:383` and the write at `main.c:384` sit on the same
unconditional path, and `storage_epoch_update`'s silent-return path is unreachable
(`construction-sweep-2026-08-24.md` §THE STORAGE FAULT PATHS ARE UNREACHABLE). **The change is
made because the asymmetry is wrong, not because it has been observed firing. It has not.**

---

## 4. WHAT THIS DOES TO GATE 7, STATED PLAINLY

**GATE 7 IS P-MINS.** `baseline-condition-correction-2026-08-24.md` §8 names its two break
states: **storage saturation past 672 epochs**, and **the unguarded increment firing on an
already-zero bucket.**

**THIS FIX REMOVES THE SECOND ONE.**

**THE REMAINING BREAK STATE REQUIRES A RECORDING LONGER THAN 672 MINUTES — 11.2 HOURS**
(`storage.c:35-36`, `storage.h:4-5`). **NO NIGHT IN THIS SERIES APPROACHES IT.** **So after this
change, gate 7 is a gate that cannot fail on any night this project is likely to produce.**

**THAT IS REGISTERED HERE AND NOT GLOSSED.** **It is the same condition — a gate whose failing
reading is unreachable in practice — that `baseline-condition-correction-2026-08-24.md` was
written to remove from gate 1.**

**GATE 7 IS NOT DROPPED BY THIS FILE, AND THE DIFFERENCE FROM GATE 1 IS REAL:**

- **Gate 1's four checks were VACUOUS BY CONSTRUCTION.** No reading of any length, any duration or
  any state could make them fail. **They tested nothing at any time.**
- **P-MINS remains ABLE to fail.** Its break state is reachable by the running program, at a
  stated recording length, on a path read from source. **It is unreachable in practice, not in
  principle.** **A gate that catches a real fault at an unusual duration is not the same as a
  gate that catches nothing ever.**
- **AND P-MINS IS THE ONLY REGISTERED DETECTOR OF ITS FAULT CLASS**, per `a5bfee5`. **Dropping it
  would leave that class unwatched.** The fix prevents one instance of the class; it does not
  make the class impossible in future code.

**WHAT IS REGISTERED, SO A LATER SESSION DOES NOT MISREAD A GATE-7 PASS:** **after this change, a
P-MINS pass on a night under 11.2 hours is NOT evidence that the stage counters are sound.** It is
consistent with soundness and with nothing having been tested. **The same asymmetry-of-evidence
note `physiological-plausibility-spec-v1` §7 makes for marker 3 applies here: a pass is not
confirmation; a FAIL would still be conclusive and would still end a run.**

**WHETHER GATE 7 SHOULD BE REPLACED BY A CHECK THAT CAN FAIL AT REALISTIC DURATIONS IS NOT
DECIDED HERE AND MUST NOT BE DECIDED BY THE SESSION THAT FINDS IT CONVENIENT.** It requires its
own dated file stating what the replacement would catch.

---

## 5. Versioning — VERIFIED AFTER, NEVER ASSERTED BEFORE

**NO DECISION CHANGES AND NO REACHABLE OUTPUT CHANGES.** The guard's false branch is unreachable,
so no stage line, no total, no band, no label and no persisted value differs on any night the
program can produce.

**`CLASSIFIER_SERIES` STAYS 14 AND `NIGHT_SUMMARY_VERSION` STAYS 3.** **BOTH ARE TO BE VERIFIED
FROM `src/c/storage.h` AFTER THE EDIT IS APPLIED — NOT ASSERTED HERE.** This is the obligation
`hrv-cadence-readout-spec-v1` §7 and `movement-gate-readout-spec-v1` state for every
non-output-changing change, and it is adopted unchanged.

**NO COMPARABILITY GROUP OPENS.** **N31 REMAINS COMPARABLE TO N26 THROUGH N30 ON EVERY STAGE
LINE.**

**NO PERSISTED STRUCTURE CHANGES.** `EpochRecord` and `NightSummary` are untouched.

**THE BUILD MUST BE CONFIRMED AND THE BINARY MUST BE FLASHED BEFORE N31 RUNS.** **A night recorded
on a binary that does not match the source is not comparable to anything.**

---

## 6. RULE 2 check

**NO CONSTANT IS INTRODUCED, MOVED, WIDENED, NARROWED OR RETIRED.** The change adds two braces and
moves one existing statement inside an existing conditional. **672 and 960 appear in §4 as read
values from `storage.h:4-5` and `main.c:191`, not as anything this file chooses.**

---

## 7. RULE 3 check

**FOR THE CHANGE ITSELF:** it is a code fix, not a scored criterion, so the vacuity forms apply to
what it does to P-MINS rather than to the fix.

**P-MINS AFTER THIS CHANGE — CAN IT PASS:** yes, on every night where `sum(s_mins)` equals `Ep`,
which is every night recorded to date. **CAN IT FAIL:** yes, on a recording past 672 epochs, where
`storage_epoch_write` returns early without incrementing while `main.c:383` has already credited
the minute. **DOES ITS FAILING READING MEAN WHAT IT APPEARS TO MEAN:** yes — a mismatch means
minutes were credited that storage did not accept, which is exactly what P-MINS is registered to
detect.

**THE THIRD FORM IS CLEAN.** **What is NOT clean, and is recorded in §4 rather than hidden, is
that the failing reading is now unreachable at realistic durations.** **That is a statement about
the gate's practical power, not about its meaning.**

---

## 8. RULE 19 self-check — performed on this file, recorded in it

**Form 1 — one quantity described twice.** 672 appears in §4 and §7 as the storage ceiling, cited
to `storage.c:35-36` and `storage.h:4-5` both times; 11.2 hours appears in §4 twice as its
duration equivalent. `main.c:939-940` and `main.c:1064-1065` are cited in §2, §3 and §4 with the
same content each time. **No disagreement found.**

**Form 2 — quantitative and prose forms, substituted at BOTH extremes.** The claim "on every
reachable path this is a no-op" was substituted at both ends. **At the low extreme** (a night with
one epoch): `s_mins` holds 1 and the bucket is non-zero at any move, so the guard is true and the
behaviour is unchanged. **At the high extreme** (a night at the 672-epoch storage ceiling):
`s_mins` totals 672 spread across buckets; any bucket a stored epoch is credited to is non-zero,
so the guard is true and the behaviour is unchanged. **Between them the same holds for the same
reason.** **The no-op claim is true at both extremes and throughout.**

**Form 3 — a decision rule against the measurement it reads.** §7 does this for P-MINS, and §4
states the consequence rather than burying it.

**Form 4 — sections serving different readers.** **A session applying the change:** §2 and §3 give
the exact text at both sites and the anchoring warning. **A session scoring a night:** §4 and §5
state what gate 7 now means and that the versions are verified, not assumed. **A later session
asking why gate 7 survived when gate 1 did not:** §4 answers it directly with the
principle-versus-practice distinction. **No section contradicts another.**

---

## 9. What this does NOT change

- **No band, no threshold, no parameter, no decision.**
- **No persisted structure.** `CLASSIFIER_SERIES` 14 and `NIGHT_SUMMARY_VERSION` 3, to be
  VERIFIED after the edit.
- **No night file is edited and no night is rescored.**
- **The baseline condition's eight gates are UNCHANGED in wording and in membership.** **Only what
  a gate-7 pass is EVIDENCE OF changes, and §4 states it.**
- **`hrv_buf_add` IS NOT TOUCHED**, per the standing prohibition.
- **No frozen spec is edited.** `construction-sweep-2026-08-24.md` and
  `baseline-condition-correction-2026-08-24.md` stand as committed; this file is the further dated
  file where they could not speak.
- **NO COMMIT FREEZE IS ARMED BY THIS FILE.** A freeze arms when a night passes the condition.

---

## 10. Status

**Frozen on commit. The code change is applied in a SEPARATE, LATER commit, so this spec exists
before the change it specifies.**

**`CLASSIFIER_SERIES` AND `NIGHT_SUMMARY_VERSION` ARE VERIFIED FROM SOURCE AFTER THE EDIT. THE
BUILD IS CONFIRMED AND THE BINARY FLASHED BEFORE N31 RUNS.**
