# movement-gate-readout-spec-v1

**Dated 2026-08-19. An INSTRUMENT ONLY. Frozen before implementation.**

**Reads with `movement-spec-v1` (`33f678b`), `classifier-spec-v3` (`2ae829a`)
sections 3.1 and 4.2, and `awake-clause-counters-spec-v1` (`d9cfd5a`).
CITED, NOT RESTATED.**

## 1. Why this exists

**The Awake decision's movement clause has not fired on any minute of the last
two recorded nights, and the instrument that would say WHY does not exist.**

**THE CLAUSE IS NOT STRICT. IT IS PERMISSIVE, AND THIS WAS READ FROM SOURCE
THIS SESSION AFTER BEING CARRIED INCORRECTLY IN A CONTINUITY DOCUMENT.**
`main.c` 76 defines `AW_MOVED_MIN` as **3**, and the comment at `main.c` 74
states it is an unweighted **3-of-5** majority. A grep across `src/` returns
exactly four sites - the comment, the define, a stale use-site comment reading
"of 5", and the test itself. **There is no redefinition anywhere.** The clause
requires 3 MOVED minutes in a centred 5-minute window, NOT 5 of 5.

**A permissive clause that never fires is a different finding from a strict
clause that declines to fire, and it points at the layer below it.**

**THERE ARE TWO GATES AND THEY COMPOUND. BOTH READ FROM SOURCE:**

- **SAMPLE LEVEL** (`main.c` 53): a sample counts as moved when
  `mag2 < 722500 || mag2 > 1322500` - a BAND around 1 g, roughly 0.85 to 1.15 g.
  **A movement that preserves magnitude near 1 g is not counted at all**, which
  `movement-spec-v1` already records as a known and unfixed blind spot.
- **EPOCH LEVEL** (`main.c` 192 to 194): the minute is MOVED only when
  `s_mv_min_moved * 100 >= s_mv_min_samples * MV_MOVED_PCT`, with
  `MV_MOVED_PCT` defined as **10** at `main.c` 28. **Ten percent of the
  minute's samples must be out of band.**

**Under the subscribed accelerometer the sample rate is on the order of
hundreds per minute, so the epoch gate demands out-of-band acceleration across
a substantial fraction of a sixty-second minute.** **NEITHER THRESHOLD HAS A
SURVIVING DERIVATION IN ANY FROZEN SPEC** - `movement-spec-v1` section 3
records the magnitude test as carried UNCHANGED across the subscription swap,
and `MV_MOVED_PCT` is not derived anywhere.

**WHAT IS NOT KNOWN, AND IS THE ENTIRE POINT OF THIS SPEC: whether the clause
does not fire because the wearer did not move, or because the gate cannot see
the movement.** `classifier-spec-v3` section 3.1's three-valued state
distinguishes UNKNOWN from STILL and does NOT distinguish "still" from
"unseen". **The existing `Unk` counter proves samples ARRIVED. It establishes
nothing about whether they were classified correctly**, and a reading of zero
on consecutive nights has been mistaken for evidence that movement detection is
working.

**NOTHING ELSE RENDERS ANY MOVEMENT QUANTITY.** `s_epoch_still` and
`s_epoch_mv_known` are RAM-only bitmaps with no persist and no render site, and
`EpochRecord` carries no movement field - so **no past night can be
re-examined** and the instrument must exist before a night, exactly as the
clause counters did.

## 2. What is measured

**THE PER-MINUTE MOVED FRACTION, IN HUNDREDTHS OF A PERCENT.**

For each closed minute, `s_mv_min_moved * 10000 / s_mv_min_samples`, stored as
a `uint16_t`. **Range 0 to 10000. Units are hundredths of a percent, chosen
because the epoch gate sits at exactly 1000 in them**, so the threshold and the
data render on one scale and a reading may be compared to the gate by eye.

**Whole percent was considered and REJECTED**: a distribution lying well below
the gate would render as all zeros and the instrument would report nothing,
which is the failure mode this spec exists to avoid.

**MINUTES WITH NO SAMPLES ARE EXCLUDED, NOT STORED AS ZERO.** A minute where
`s_mv_min_samples == 0` is UNKNOWN per `classifier-spec-v3` section 3.1, and a
zero would be indistinguishable from a genuinely motionless minute. **The
existing `s_epoch_mv_known` bit is the exclusion test - no sentinel value is
introduced.**

**A SEPARATE COUNT OF MOVED MINUTES** is accumulated at minute close from the
same `movement` boolean the state assignment uses, **before any branch**, so it
cannot disagree with the decision - the same construction guarantee
`awake-clause-counters-spec-v1` section 4 relies on.

## 3. What is rendered

**TWO LINES APPENDED TO DIAG 2, AS THE EIGHTH AND NINTH:**

```
Mv <p50> <p90> <p99>
Mvx <max>  MvM <n>  n <n>
```

- **`p50`, `p90`, `p99`** - percentiles of the moved fraction over minutes with
  a defined value, in hundredths of a percent.
- **`Mvx`** - the maximum over the same set. **THE SINGLE MOST DECISIVE VALUE
  ON THE LINE: if `Mvx` is below 1000, NO minute in the night could have been
  marked MOVED under any circumstances**, and the epoch gate is unreachable
  rather than unmet.
- **`MvM`** - the count of minutes marked MOVED. **This is c1's input.**
- **`n`** - the count of minutes with a defined moved fraction.

**WHERE IT FITS, AND THIS WAS CHECKED RATHER THAN ASSUMED.** `prv_draw_diag`
draws a header at `y = 2` with a 26-pixel advance and then nine 18-pixel lines,
ending at `y = 190`, and that screen is observed rendering all nine.
`prv_draw_diag2` currently ends at `y = 166` with seven. **Two further lines
bring DIAG 2 to 190, which DIAG 1 already demonstrates fits.** The overflow
recorded in the DIAG-split comment occurs near eleven lines. **After this spec
DIAG 2 has ONE line of margin remaining and the next addition must re-check.**

**THE PERCENTILE SET DIFFERS DELIBERATELY FROM THE `Fd` READOUT'S**
(p50/p75/p90/p95). The question here is whether the distribution's UPPER TAIL
approaches the gate, so p99 and the maximum earn their place and p75 does not.
**Recorded so the difference is not read as an inconsistency.**

## 4. What does NOT change

**NO DECISION, NO THRESHOLD, NO WINDOW SHAPE, NO CONSTANT AND NO STORED
FIELD.**

- **`MV_MOVED_PCT` is NOT changed.** Neither is the magnitude band, `AW_MOVED_MIN`,
  the three-valued state, or the known-bit convention.
- c1, c2 and the disjunction are byte-identical. The six clause counters are
  untouched.
- No `EpochRecord` field, no `NightSummary` field, nothing persisted.
- **`CLASSIFIER_SERIES` is NOT bumped and `NIGHT_SUMMARY_VERSION` is NOT
  bumped.** Same treatment as `a7d6939`, `4236e5f` and `b304e1f`. **The bump
  rule is OUTPUT CHANGES and this changes no output.**
- **This spec opens NO comparability group.**

**THE STALE USE-SITE COMMENT AT `main.c` 537 IS CORRECTED IN THE SAME COMMIT
AS A COMMENT ONLY.** It reads "AW_MOVED_MIN of 5" and the define is 3. **A
wrong comment about a threshold is how the wrong value reached a continuity
document, so it is fixed where it sits.**

**VERIFICATION REQUIRED BEFORE COMMIT, NOT ASSERTED HERE:** read
`CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION` from `storage.h` after the
edit, and confirm `git diff --stat` names `src/c/main.c` alone.

## 5. Cost and safety

**RAM ONLY. NO FLASH I/O IS ADDED.** The per-minute value is computed at minute
close from counters already maintained, and the percentile pass reads a RAM
array. **No `storage_epoch_read` call is added anywhere**, so review finding 10's
unverified stop-time watchdog concern is not engaged by this change.

**One `uint16_t` per epoch of static RAM**, sized by the existing
`EPOCH_VAR_MAX`, plus one counter.

**THE PERCENTILE PASS SORTS THE ARRAY IN PLACE AND THEREBY DESTROYS THE
PER-MINUTE ORDERING.** This is acceptable **only because nothing reads the
per-minute values** - the array has exactly one consumer, this readout.
**IF ANY FUTURE CHANGE READS THE PER-MINUTE SERIES, THE IN-PLACE SORT MUST GO
FIRST.** Registered here so it cannot be discovered later.

**Defined values are compacted to the front before sorting** and the count is
the `n` that renders.

## 6. What this CANNOT establish

**WHICH OF THE TWO GATES IS WRONG, IF EITHER.** The readout describes the
distribution the epoch gate sees. **It says nothing about what the magnitude
band discarded before that**, because a sample rejected as in-band is never
counted. **A distribution lying far below the gate is consistent BOTH with a
band that misses real movement AND with a genuinely motionless wearer**, and
this instrument cannot separate them. **Separating them needs a sample-level
magnitude distribution, which this spec does NOT build.**

**WHETHER THE WEARER MOVED.** There is no ground truth and none is claimed.

**WHETHER ANY THRESHOLD SHOULD MOVE.** A reading is not a licence. **Any change
to `MV_MOVED_PCT`, the magnitude band or `AW_MOVED_MIN` is a
`classifier-spec-v5` with its own argument from structure or physiology, frozen
before implementation.**

**ANYTHING ABOUT A PAST NIGHT.**

**ANYTHING FROM ONE NIGHT.**

## 7. Rule 2 check

**NO CONSTANT ENTERS ANY DECISION.** The stored value is a measured fraction.
`Mvx`, `MvM` and `n` are a maximum and two counts, with no band and no expected
value.

**THE PERCENTILE CHOICE - p50, p90, p99 - IS A READOUT CHOICE AND NOTHING
BRANCHES ON IT.** Moving p99 to p95 would change which number is REPORTED and
would change no stage, no total and no decision. **The narrow exemption applies
and is registered WITH ITS EXPIRY: the moment any decision reads one of these
percentiles, the exemption stops applying and the choice must be derived.**
Same standing as `AWC_EARLY_MIN`.

**THE 10000 SCALE FACTOR IS A UNIT, NOT A THRESHOLD.**

## 8. Rule 3 check

**THIS SPEC REGISTERS NO CRITERION AND MAKES NO PREDICTION.** It is an
instrument. **A pre-registration may register a criterion against these values
once they have read at least once**; registering a band before any reading
exists would be inventing one from nothing.

**THE RENDER-SITE CHECK:** every value above is rendered by DIAG 2, a screen
every night captures. **No quantity here is invisible to the scoring pass**,
which is the defect that made an earlier registered identity unevaluable.

**DIAG 2 RENDERS FROM RAM STATICS AND DOES NOT SURVIVE APP EXIT.** These values
carry the same capture discipline and the same consequence as the clause
counters: **if capture 4 is missed they are gone, and no re-read recovers
them.**

## 9. Status

**Frozen on commit. Implemented in a SEPARATE commit. No frozen spec is edited
by this file and this file supersedes nothing.**
