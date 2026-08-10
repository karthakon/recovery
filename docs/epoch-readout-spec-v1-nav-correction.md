# Recovery - Epoch Readout Spec v1, NAVIGATION CORRECTION

**DATED 2026-08-10. Supersedes section 4 NAVIGATION ONLY of
docs/epoch-readout-spec-v1.md (413e850).**

**413e850 is NOT edited.** Its section 4 stands as written and as committed.
This file records that its navigation was defective, why, and what replaces it.
Superseding in a new dated file rather than editing the frozen one is the
standing rule for every frozen document in this project.

**Nothing else in 413e850 changes.** Sections 0, 1, 2, 3, 5, 6, 7 and 8 all
stand unamended. No statistic changes. No threshold moves. No stage decision
changes. EpochRecord and NightSummary remain untouched. The section 2
registered identity - pre-smoother REM total MUST equal the night
v_over_gate_count, 15 on N16 - is unaffected and still governs.

---

## 0. The defect

413e850 section 4 placed RUNS after DIAG, reached from DIAG by DOWN.

**RESULTS, HYPNO and DIAG are reachable only in the session-results flow that
follows a recording, and they are driven by RAM statics that do not survive app
exit.** Once BACK returns to IDLE they are gone, and after an app install there
has been no session in that launch at all. **RUNS as specified was therefore
unreachable on a fresh launch - which is precisely and only when it is needed,
because the whole purpose of the screen is to read epochs stored by a session
that ended before the install.**

This was avoidable. The same fact was already recorded in 0670f72 and is
visible in prv_draw_diag - DIAG prints dashes when its RAM statics are empty on
relaunch. The consequence for RUNS navigation was not followed through.

**The computation was never affected.** prv_compute_runs reads persist storage
through storage_epoch_read, not RAM statics, so it returns correct values on a
cold launch. **Only the route to the screen was wrong.**

---

## 1. The replacement navigation

**RUNS is reached from IDLE by DOWN. UP or BACK returns to IDLE.**

DOWN is unused on IDLE - UP goes to HISTORY, BACK exits the app, and a long
SELECT starts a recording - so nothing is displaced and no existing route
changes.

**RESULTS, HYPNO and DIAG are unchanged**, including DIAG DOWN, which no longer
leads anywhere and is removed. Every existing recorded-value convention and
every prior night screen-reading procedure still applies.

RUNS remains diagnostic-only and remains under queued item 9 with IDLE and
DIAG. It is not a release screen.

---

## 2. Why this is a superseding file and not an edit

docs/measurement-spec-v1.md section 3.6 states that button wiring is an
implementation decision, recorded in the commit message rather than fixed in
the spec. **413e850 section 4 overstepped that precedent by fixing navigation
in the spec at all.** Having fixed it there, changing it silently would be
a quiet divergence from a frozen document, and that habit is what makes the
other frozen specs less trustworthy. Hence a dated correction.

**Navigation in any future spec of this series should follow 0670f72 section
3.6 and be left to the commit message.**

---

## 3. Ordering - UNCHANGED and still mechanical

413e850 section 6 stands in full. **storage_session_start zeroes epoch_count
(storage.c line 18), and storage_epoch_read refuses any index at or above the
count. Starting ONE recording makes N16 epochs unreadable**, and only one night
of epochs exists at a time because KEY_EPOCH_BASE is a single overwritten
buffer.

1. Install the build.
2. **Open RUNS from IDLE by DOWN and read N16 numbers BEFORE starting any
   session.**
3. Only then pre-register and record N17.

**If step 2 is skipped, N16 adjacency answer is destroyed and cannot be
recovered.** The correction makes step 2 possible; under 413e850 section 4 as
written it was not.
