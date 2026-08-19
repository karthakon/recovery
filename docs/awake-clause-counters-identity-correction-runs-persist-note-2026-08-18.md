# awake-clause-counters-identity-correction-runs-persist-note-2026-08-18

**Dated 2026-08-18. Qualifies ONE claim in
`awake-clause-counters-spec-v1-identity-correction-2026-08-18.md` section 4.
It supersedes NOTHING and changes NO code.** The corrected identity in section
2, the derivation in section 3, and everything in sections 5, 6 and 7 stand
unchanged.

## 1. The claim being qualified

Section 4 states:

> **RUNS READS PERSIST.** Unlike DIAG 2, which is destroyed at app exit, the
> conditioning check can be re-read at IDLE after the app closes. **This is a
> strict improvement in the recoverability of the check** and is the reason RUNS
> is the correct screen for it rather than DIAG 2.

**THE FIRST SENTENCE IS TRUE OF RUNS GENERALLY AND FALSE OF `AwO`
SPECIFICALLY.**

## 2. What was observed

**OBSERVED DIRECTLY, NOT INFERRED.** After the implementing commit was
installed, RUNS was opened at IDLE against a night whose epochs were still
stored. Every other value on the screen read unchanged - the REM run
statistics, the epoch count, the REM offset, both transition counter pairs, and
both onset values. **`AwO` read `--`.**

## 3. Why - read from source

`has_awo` is keyed on `s_onset_epoch_idx`, which is a **RAM static, reset to -1
on app launch** (`main.c` 328 and 1491). **The stored epochs survive an app
exit; the live immobility onset index does not.**

**`AwO` THEREFORE PRINTS `--` ON ANY READ AFTER AN APP RESTART, EVEN THOUGH THE
EPOCHS IT WOULD COUNT ARE STILL PRESENT.** The `--` is CORRECT behaviour under
section 4's own undefined rule - it is not a bug in the render and it must
never be read as a zero. **What is wrong is section 4's claim about
recoverability, not the code's behaviour.**

## 4. What this does and does not cost

**IT COSTS NOTHING AT SCORING TIME.** `AwO` is captured at stop in capture 6,
while the app is still running and `s_onset_epoch_idx` is live and correct.
**The identity is scored from that capture, which is when it was always going
to be scored.**

**WHAT IS LOST IS THE SECOND CHANCE.** If capture 6 is missed, `AwO` cannot be
recovered by reopening RUNS, and the identity becomes NOT PERFORMED for that
night. **`AwO` must therefore be treated as a VOLATILE value on a persistent
screen** - captured with the same discipline as DIAG 2, not the relaxed
discipline the rest of RUNS permits.

**CAPTURE 6 NOW CARRIES A VALUE THAT CANNOT BE REDONE. CHECK FOR `AwO`
EXPLICITLY.**

## 5. Why it is not being fixed tonight

`st->onset_label` is a persist-derived onset computed from `EpochRecord.reserved`
in the same function and would survive a restart. **Keying `AwO` on it instead
would change WHICH SPAN IS COUNTED**, because `onset_label` and
`s_onset_epoch_idx` are different quantities by `classifier-spec-v3` section
3.4 - and the six counters branch on `s_onset_epoch_idx`. **An identity whose
two sides use different onsets is not an identity.**

**A fallback is therefore a SPEC question and not a code tidy-up**, and it is
deferred rather than rushed on the same evening as the correction it would
amend. **Registered as an open item; NOT scheduled.**

## 6. Status

**A note, not a correction. Nothing is superseded and no constant is
introduced.** A change to `AwO`'s span definition is a further dated correction
with its own argument, never an edit.
