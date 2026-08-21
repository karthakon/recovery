# hrv-cadence-readout-spec-v1-recording-gate-correction-2026-08-20

**SUPERSEDES §3 OF `hrv-cadence-readout-spec-v1` (`2d0cc15`) AND NOTHING ELSE.** §1, §2, §4, §5, §6, §6.1, §7 and §8 stand exactly as frozen. The registered identity `He == Hn + Hx` is unchanged and still holds.

**Written BEFORE the implementation commit.** The defect was found by reading the implementation back against the spec, before any build was committed and before any night was recorded under it.

## §1 — The defect in the frozen §3

§3 names four increment sites and does not condition any of them on `s_recording`.

**The HRV subscription is APP-LIFETIME.** `prv_set_hrv(true)` is called in `prv_init`, and `prv_health_handler` tests `s_recording` at one place only — gating the `s_minute_buf` and `s_night_buf` adds. Read from source at `2ae34bb`. The handler therefore continues to fire, and the four counters continue to increment, **after `prv_stop_recording` has run and for as long as the app remains open.**

**This is not a marginal window.** The stop-time ordering is SEVEN CAPTURES. Minutes of wall-clock elapse between the recording stopping and DIAG 3 being read, and every one of those minutes adds events that were not part of the night.

**THE BIAS IS DIRECTIONAL AND IT FAVOURS THE WRONG CONCLUSION.** §6.1 derives `(Hn - Hd) / session seconds`, where session seconds comes from `rec_duration_min` — a duration that EXCLUDES exactly the post-stop period inflating the numerator. The effective unique sample rate would come out biased HIGH: the direction that makes RSA look MORE viable than it is, against a Nyquist margin §6.1 records as only 1.5-2x.

**"Session-scoped" in §2 names the SCOPE. §3 names the SITES, and the code follows the sites.** Zeroing at session start does not make a counter session-scoped if nothing stops it at session end. The two must agree, and §3 is the half that must move.

## §2 — What replaces §3

§3's four sites stand as written, **with every increment additionally conditioned on `s_recording`**, matching the condition the buffer adds in the same handler already carry.

- `He` increments at the top of the HRV branch, **OUTSIDE** the `ppi > 0` guard and **INSIDE** a `s_recording` test.
- `Hx` increments on the guard's FALSE path, inside the same test.
- `Hn` increments and the `Hd` comparison runs on the TRUE path, **BEFORE** the assignment to `s_last_ppi`, inside the same test.
- **The `Hd` reference remains NEW and SESSION-SCOPED, NEVER `s_last_ppi`** — unchanged from the frozen §3 and restated here because it is the site most likely to be got wrong on a re-read.

**`s_hd_prev` IS ALSO GATED.** If the reference updated outside recording, the first comparison of a session would run against a post-stop value from the previous session — reintroducing, in a new place, exactly the defect the frozen §3 avoided by refusing `s_last_ppi`.

## §3 — What this does NOT change

- **`s_hrv_events` is untouched.** It remains app-lifetime, remains ungated, and remains rendered on IDLE's `ev` line. It is a different quantity (RULE 9) and this correction does not alter it.
- **No classifier input, no stage, no total, no stored field.** Still an instrument only.
- **`CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION`** are to be VERIFIED UNCHANGED after implementation, not asserted — unchanged obligation from §7.
- **The undefined guard in §5 is unaffected.** It keys on `s_session_start`, not on `s_recording`.

## §4 — What is LOST by gating

**Events arriving between app launch and recording start are no longer counted anywhere session-scoped.** That is deliberate. They belong to no session, `rec_duration_min` does not cover them, and including them would make the §6.1 denominator wrong in the other direction.

**`s_hrv_events` still counts them app-lifetime**, so nothing becomes unobservable — it moves to a counter whose scope actually matches what it measures.

## §5 — Why this is a correction and not an edit

`hrv-cadence-readout-spec-v1` is frozen at `2d0cc15`. **The frozen text is not edited.** PRECEDENT: `64bbe19`, `62288a1`, `244e64d` and `b40186c` each superseded one element of a frozen file by a dated file filed beside it.

**This correction is filed in `~/recovery/docs/` beside the spec it supersedes**, because the spec is a SPEC and specs live there under the standing filing rule. The `predictions/` exception applies to pre-registration corrections, which this is not.
