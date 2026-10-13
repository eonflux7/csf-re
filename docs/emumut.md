# emudiff mutation benchmark

`tools/emumut.py` measures the differences emudiff detects, alongside its verdicts
on unchanged controls. It never installs a hook or changes `src/auto`, the ledger,
or emudiff. It uses `gate.compile_cmd()` and fresh objects, including in a worker
tree with no built auto-function objects.

## Reviewing an emudiff change

```sh
.venv/bin/python tools/emumut.py run --n 400 --seed 1 --jobs 8 --out work/emumut-baseline.json
.venv/bin/python tools/emumut.py run --replay work/emumut-baseline.json --emudiff /path/to/job/tools/emudiff.py --jobs 8 --out work/emumut-job.json
.venv/bin/python tools/emumut.py compare work/emumut-baseline.json work/emumut-job.json
```

Replay embeds the original sources and exact edits from A. Prefer it to independent
sampling when reviewing a change. The comparison reports class totals, lost catches
(including catches changed to inconclusive/error), and changed original verdicts.
It also reports unpaired mutations and refuses to compare paired mutations whose
compiled objects changed. Input hashes identify changes to the original image,
API, function inventory or call metadata. A changed original control can confound
a mutant DIFF; the table explicitly warns about those cases.

The selected Python implementation runs against this benchmark checkout's image
and metadata, not the variant's build objects. Each emulation has its own process;
only eight processes run at once with `--jobs 8`. An individual process timeout is
an **error**, never a catch or a silent pass.

## Corpus and exclusions

The generator first filters the ledger for done, installed, zero-mismatch functions
with in-game differential evidence. Each selected original is then compiled fresh;
its `verify.code_hash()` must still put it in `verify.tiers()`'s `diffed` tier.
New corpora exclude originals for which this emudiff says DIFF, inconclusive, or
error, while recording their controls. Replay deliberately retains the frozen
corpus if the variant's original verdict changes.

Sampling is deterministic for the same sources, metadata, compiler and seed.
Mutation classes are scheduled round-robin, preferring new functions. Compilation
failures, stale evidence, unsuitable controls and equivalent objects are recorded
in `excluded`; they do not contribute to the miss count. Class totals need not be
equal: some classes are scarce or have more exclusions. An incomplete corpus is
written with `complete: false` and a nonzero exit status.

Each accepted record contains `(ea, class, line, before, after)`, source offsets,
a stable mutation ID, object fingerprint, emudiff verdict and detail. The JSON
also contains unchanged controls and the original sources; temporary objects are
deleted after the run. Originals compile once per function; mutants once each.

The equivalent control compares COFF function code with relocation bytes masked,
retaining relocation targets/addends and referenced data. Registration stubs and
debug information are omitted: HOOK registration contains line numbers, and its
global initializer contains the source filename. Retaining relocated constants
avoids incorrectly discarding changes to float literals with unchanged load
instructions. Parenthesis-only edits, including edits that move HOOK's line,
exercise this control in the tests.

## Mutation scope

The fifteen classes are field offset (+4/-4), adjacent call-argument swap, dropped
call argument, constant (+1), loop strictness (off by one), inverted if condition,
dropped if condition, dropped standalone call, dropped memory store, neighbouring
callee, signed/unsigned compare, byte/dword store width, stale value across an
out-argument call, dropped return value, and adjacent virtual slot.

Dropped arguments really alter the function-pointer signature and remove an
argument expression; they do not merely pass zero. Register arguments are retained
for fastcall targets. Dropped stores retain RHS evaluation, and dropped returns
retain the expression's side effects. Stale-value mutants snapshot an out-argument
before a call and pass the snapshot to the next call, with no intervening write to
that variable. Wrong-callee mutations change the address inside CSF_FUNC, retaining
the prototype. Typed fields use CSF_AT with their existing decltype for offset
mutations. Typed vSlot wrappers can be expanded into dispatch to the next slot.

This is a conservative lexical generator, not a C++ AST transformation. It ignores
comments, strings, address macros and declarations where appropriate, then relies
on actual compilation to reject edits that cannot type-check. It primarily targets
CSF_FUNC calls, CSF_AT/simple typed fields and vtbl/vSlot accesses. It does not cover
every syntax variant, switch-case coverage, ABI wrappers, all loop forms, or changes
inside templates/headers. It does not reimplement library regions.

Code-different mutants are not automatically proven semantically different for
every possible input: redundant stores, dead results and unused callee arguments
can remain equivalent beyond the byte-equivalence control. In-game equality of the
unchanged function does not prove a particular edit is non-equivalent. The audited
misses below distinguish observed differences from input-coverage limitations.

## Baseline and miss audit

Seed 1, 400 mutants, 8 jobs, current emudiff (SHA-256
`f67db170da1245cadfd466c3d84ca986561ccbfc2f898dd8d0c05a94561595d0`):

| Class | Caught | Missed | Inconclusive | Kill/all |
|---|---:|---:|---:|---:|
| field_offset | 22 | 10 | 0 | 68.8% |
| swapped_argument | 20 | 7 | 0 | 74.1% |
| dropped_argument | 21 | 14 | 0 | 60.0% |
| constant | 15 | 19 | 0 | 44.1% |
| loop_bound | 10 | 20 | 0 | 33.3% |
| inverted_condition | 25 | 7 | 0 | 78.1% |
| dropped_condition | 19 | 16 | 0 | 54.3% |
| dropped_call | 28 | 5 | 0 | 84.8% |
| dropped_store | 29 | 5 | 0 | 85.3% |
| wrong_callee | 30 | 4 | 0 | 88.2% |
| signed_unsigned | 2 | 11 | 0 | 15.4% |
| store_width | 31 | 2 | 0 | 93.9% |
| stale_after_call | 0 | 4 | 0 | 0.0% |
| dropped_return | 19 | 5 | 0 | 79.2% |
| virtual_slot | 0 | 0 | 0 | not sampled |
| **Total** | **271** | **129** | **0** | **67.8%** |

397 mutated functions, 451 unchanged controls. Controls: 437 same, 13 inconclusive,
1 DIFF. No mutant errors. Exclusions: 26 identical objects, 14 compilation failures,
13 inconclusive controls, 1 DIFF control. Runtime of the saved baseline: 553.4 s
(9.2 minutes), including originals and rejected candidates. JSON:
`work/emumut-baseline.json`; fresh reproduction: `work/emumut-baseline-current.json`.
The fresh run took 500.0 s (8.3 minutes); all 400 mutation fingerprints and
verdicts and all 451 control verdicts match the saved baseline. Compare output:
`work/emumut-baseline-reproduction-compare.txt`.
Kill/all counts inconclusive/error as not caught, never as a miss. The observed
zero inconclusive mutants does **not** mean eight useful runs each: `same 1/8`
is a miss even if the other seven profiles fault. Controls with no useful run
are excluded, so this measures detection on the usable subset, not all done code.
68 of the 129 misses have fewer than eight usable profiles; 14 have only one.

### Twenty misses read

This is a purposive blind-spot audit, not a random estimate of all 129 misses.
Read the embedded original/edit, both freshly compiled objects' CodeView line
tables, reached source lines, raw memory/eax and call events for each usable
profile. Scratch evidence is in `work/emumut-audits/<id>.json` (all twenty rerun
identically in `work/emumut-audits-current/`), generated by
`work/emumut_audit.py work/emumut-baseline.json ID OUT.json`. Optimized line tables
are supporting evidence, not exact statement coverage; the path conclusions below
also use the surrounding tests and call traces. Addresses and line numbers below
identify the exact records in the baseline; two edits in 004DE230 are distinct.

**Masked by a comparison rule (5):**

| EA:line / class | Edit | Evidence / exact rule |
|---|---|---|
| 00500A80:29 constant | store +0x64: 0 -> 1 | raw dword 0 vs 1 at 0x2A000164; `compare`: `abs(wo-wm)<64 and close(fo,fm)` treats integer bits as float last-bit rounding |
| 004E86B0:20 constant | field_0050: 0 -> 1 | raw dword 0 vs 1 at 0x2A000150; same float-rounding rule |
| 005237C0:24 field_offset | +0x3C -> +0x38 | raw field +0x3C 0 vs 6 (profile 1), 0 vs 3 (profile 2); same float-rounding rule |
| 004C03A0:22 constant | valueA initializer 0 -> 1 | dispatch argument original 0xCCCCCCCC vs mutant 1; `same_args` masks every original 0xCC byte; mocks never fill debug triple |
| 00431780:40 dropped_return | render-state call result -> zero | path reached in profile 7, eax 0x2C0E0101 vs 0; `eax_meaningless(ea)` skips return comparison (may correctly model an unused return) |

**Memory/pointee contents not observed (3):**

| EA:line / class | Edit | Evidence |
|---|---|---|
| 004AA310:41 dropped_store | omit probePos[1] initialization | profiles 6/7 reach probe construction and sub_7707E0; events compare only normalized `stack` pointer, not the float array; `on_write` never records stack writes |
| 004B7DD0:19 store_width | target handle +4: int -> byte | profiles 2/6/7 reach mutation and copy-target call; pointee upper bytes differ but stack object's contents are not recorded/consumed by mock |
| 004FBBA0:18 dropped_store | omit flag clear before acquire | intervening acquire mock cannot inspect flag; later store overwrites it, so final memory agrees (transient memory invisible) |

**Out-parameter dependency not modelled (2):**

| EA:line / class | Edit | Evidence |
|---|---|---|
| 004DE230:31 stale_after_call | snapshot maxY before ComputeFloatExtents | all profiles reach call; mock never writes extents, so pre/post reads agree; original forwarded 0xCC bytes are additionally masked |
| 004DE230:31 stale_after_call | snapshot minX before ComputeFloatExtents | same missing mock writes, at first ApplyExtentToObject rather than second |

**Mutated path not reached in any usable profile (6):**

| EA:line / class | Edit | Evidence |
|---|---|---|
| 004DF2E0:99 dropped_store | omit edgeEnd[2]=0 | only profiles 0/6 return; stop before crossing-edge initialization; five faults and one cap skipped |
| 00691120:29 dropped_return | special-tag Mem_AllocDebug result -> zero | other allocator branches taken; this special-tag branch never reached |
| 00431620:8 wrong_callee | GetSceneMenuContext 44FC60 -> 44FC80 | all usable profiles stop before that call; only GetGlobalValue event |
| 00733520:26 field_offset | field_00EC -> +0xE8 | selected currentIndex==0/negative-step branch never reached, although other branches run |
| 004E4D10:28 dropped_call | omit sub_4E4350 | conditional branch never taken; original and mutant both have one other call |
| 0048D0F0:25 dropped_argument | channel 2 argument removed | only channel 1 dispatched in usable profiles; no event for channel 2 |

**Path reached, distinguishing input/boundary absent (3):**

| EA:line / class | Edit | Evidence |
|---|---|---|
| 00492B10:12 signed_unsigned | signed value>=0 -> unsigned>=0 | every generated field value is nonnegative; no negative input exercises distinction |
| 00713920:26 dropped_return | return result -> zero | selected failure return is reached only with zero low byte; success goes through the other return |
| 0069B320:21 loop_bound | index<count -> index<=count | only profile 6 usable; an existing range record matches and returns before the changed final iteration; seven original faults excluded |

**Likely semantic equivalent beyond byte-equivalence control (1):**

004619C0:16 `store_width`: widen parentRecord[1] store at +128 from byte to
dword. Later stores explicitly reset +129, +130 and +131 before any outside call;
the extra bytes are overwritten. Raw final memory, calls and return agree. This
is not evidence that emudiff missed a real bug; the compiler still emits different
code, so the stipulated byte-equivalence control cannot remove it. A kill rate is
a sensitivity proxy over code-different edits, not a proven true-difference rate.

### Not covered

No virtual-dispatch site matched the eligible diffed corpus; `virtual_slot` is
implemented and compile-tested on fixtures, but has **no baseline measurement**.
The eligible set is dominated by straight-line code: in-game record/replay often
does not admit virtual calls. The four stale-value mutants come from one function.
The generator does not model contiguous stack-object layout corruption (JOURNAL's
004C4890), arbitrary ABI changes, switch-case forcing (004D9170), library internals,
or callee behaviour. These and semantic-equivalent mutants remain open limitations.
