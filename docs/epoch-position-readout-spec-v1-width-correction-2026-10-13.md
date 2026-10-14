# epoch-position-readout-spec-v1 — width figure correction

**Dated 2026-10-13. SUPERSEDES ONE FIGURE in `epoch-position-readout-spec-v1.md` (`c46a45d`), which is
NOT edited. Nothing else in that file changes.**

## §1 — The defect

`epoch-position-readout-spec-v1` §6 states that the longest possible run line is "three items of
`959+959`, 23 characters", and §14 FORM 2 repeats "23-character". **§6 itself specifies two spaces
between items.** Three items of seven characters with two two-space separators are
`7 × 3 + 2 × 2 = 25` characters. **23 counts single spaces and is wrong.** The RULE 19 check before
freezing compared the figure across sections but did not recompute it from the format string.

Found while testing the implementation on the host: the worst-case string rendered by the
implemented format (`"%u+%u"` then `"  %u+%u"`) measured 25 characters.

## §2 — The correction

**The worst-case run line is 25 characters.** Wherever `epoch-position-readout-spec-v1` §6 and §14
say 23, read 25.

## §3 — What does not change

The format, the two-space separator, three items per line, the 24-run page, the nine-line height,
the width check of §12 step 5 and the two-per-line fallback all stand. **The width check is made on
the watch, so it was never resting on the figure;** the figure only described how hard the check is,
and the check is now known to be two characters harder than stated. No code, no constant, no check.
`CLASSIFIER_SERIES` and `NIGHT_SUMMARY_VERSION` are untouched.

## §4 — Rule checks

RULE 2: no constant. RULE 3: no criterion. RULE 6: `7 × 3 + 2 × 2 = 25`, and the host test printed
`25` for the literal string. RULE 7: no subject value. RULE 19: the corrected figure is stated once,
in §2.

## Status

Frozen on commit. A change is a superseding dated file, never an edit.
