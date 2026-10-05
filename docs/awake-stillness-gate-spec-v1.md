# awake-stillness-gate-spec-v1

Dated 2026-10-05. Implements the single classifier change frozen in
N36-preregistration.md (recovery-nights, 4edab93). Opens classifier_series 15.
FROZEN AND PUSHED BEFORE THE BUILD. Read at pin 8cd243b before writing (RULE 0,
RULE 21); every site below was read from source.

## s1. The change
A stillness gate is added to the heart-rate Awake clause (c2) in
prv_awake_redecide, at the STAGE-DECISION SITE ONLY. For a post-onset minute i,
define c2_in_run = (s_epoch_in_run bit i set) and c2_eff = c2 && !c2_in_run. A
c2 minute sitting inside a stillness run of >= STILL_RUN_MIN (5) does NOT
contribute to StageAwake. The movement clause c1 is unchanged.

## s2. The two sites, and only these two
(1) ns_stage construction (main.c ~966): (c1 || c2) -> (c1 || c2_eff).
(2) clear-to-Light fall-through (main.c ~1002): !(c1 || c2) -> !(c1 || c2_eff).
c2_in_run reads s_epoch_in_run, already built at the top of prv_awake_redecide
before the decision loop. STILL_RUN_MIN (5) is reused, not redefined. No new
constant, no new static, no new storage field.

## s3. Instruments read the UNGATED c2 -- the gate stays visible
s_c2_total / s_c2_still (C2n/C2s), s_ab_l / s_ab_w, and the six awc counters
continue to read c2, not c2_eff. The gate changes the STAGE, never the
instrument, so C2s/C2n keeps measuring the clause the gate acts on and a future
night can still see how many c2 minutes were caught. Confirmed from source: the
counter sites at main.c 974-991 all branch on c2.

## s4. Disposition and the REM guard it relies on
A gated minute is still (in a stillness run), so it is never MV_MOVED; its live
stage is Light or REM, not Awake. The fall-through leaves it for
prv_base_redecide, which decides Light vs REM by T1/T2/T3. REM there requires
t3 = still && known (main.c 1107-1108), so motion can never become REM and
mid-night REM remains reachable. The gate therefore does NOT force Light -- it
returns the minute to the existing decision rather than overriding it.

## s5. Versioning, verified from source
CLASSIFIER_SERIES 14 -> 15 (storage.h:36): the output changes, comparability
breaks, and it is stamped into every record at storage.c:82. NIGHT_SUMMARY_VERSION
STAYS 3 (storage.h:17): no stored field is added or reordered; the struct layout
and the v1/v2/v3 tail sizes are unchanged. The two defines are independent --
series is the OUTPUT version, summary-version is the LAYOUT version. Confirmed by
reading storage.h:17,36 and storage.c:81-82,164.

## s6. P-ONSET-REM is a reading, not code -- precise definition
No instrument is added. prv_compute_runs already computes onset_idx (the first
epoch of the first run of RUNS_ONSET_RUN consecutive non-Awake minutes --
LABEL-onset, NOT the live immobility onset s_onset_epoch_idx) and first_rem (the
first pre-smoother REM epoch), and renders Off = first_rem - onset_idx on RUNS
(first_off / has_off, main.c 2204-2207). P-ONSET-REM is scored at reading time
off Off: first REM arriving only a few minutes after label-onset -- far below
normal REM latency (first REM is normally well over an hour into sleep) -- is a
sleep-onset-REM fault, a classifier error. Off is defined against LABEL-onset;
this is stated so the reader knows which onset. The check reads a rendered value,
never the hypnogram (which is a sample).

## s7. Expected effect on Off -- stated in advance
The gate recovers still minutes out of Awake into the Light/REM decision. Early
in the night a still minute with elevated HF variance can become REM there, so
the gate may LOWER Off (make onset-REM more frequent), not raise it. This is
expected and is NOT a reason to alter the gate: onset-REM at N35 read Off 1 on
the UNGATED classifier, which points at the absence of a Deep class -- early
low-variance still minutes have nowhere to go but Light or REM. Deep is HELD;
this spec does not address it. Off is watched on N36 and the next change, if any,
is argued from N36 plus source.

## s8. Rule checks
RULE 0 / RULE 21: every site read from source at 8cd243b before writing.
RULE 2: no new constant; STILL_RUN_MIN reused from a frozen spec where it is
derived. RULE 11: no new or changed format string, so no ELF string check is
owed -- stated so its absence is deliberate, not skipped. RULE 6: no derived
value is typed in this file.

## s9. Build and verify (performed after this file is frozen and pushed)
pebble build clean; confirm footprint and that the only warnings are the
pre-existing unused-function and RWX-LOAD-segment ones with NO new warning;
flash; verify RUNS (RemN, Off), DIAG 2 and DIAG 3 render with C2s/C2n present.
Reinstall required: app code changed. No night is recorded until the build is on
the watch and its screens are verified.
