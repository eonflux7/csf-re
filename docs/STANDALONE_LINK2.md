# Second standalone link — 2026-10-14

**The target links:** `build/csf_standalone.exe`, **8,666,624 bytes**.
Both the initial successful link and the post-edit incremental link returned 0.
`linkreport.py` reports zero errors in every category. No executable was run;
Wine is absent and was not installed.

**Validation exception:** all 31 edited auto files preserve their exact pre-edit
emudiff verdict lines, but only **28 gates print `ok`**. Three already had DIFF
verdicts. With the emudiff gate enabled, preserving those DIFF lines and obtaining
`ok` are mutually incompatible. No tool, waiver, database or gate policy was
changed to hide that conflict. These are not claimed as passing functions.

## What was wrong / changes

* The target compiled `rw/cpu.cpp` but did not link its librw providers: the actual
  first attempt had **32 ordinary undefined librw symbols**, besides the 24 data
  symbols. `cmake/librw.cmake` now references the unmodified MIT checkout with
  the NULL platform and `EXCLUDE_FROM_ALL`; standalone links `librw::librw`.
  The optional CPU probe includes the same guarded helper, avoiding duplicate
  targets. Existing narrowing/libm build adaptations are retained without any
  checkout patch or source copy. The hooked target stays independent of librw.
* All 24 rejected addresses were incorrect data bindings, spread over **31
  translation units**, not 24 files. Active standalone references now contain
  no rejected globals/vtables. Strict datagen replaces diagnostic
  `--allow-rejected`; it emits **2,659 global / 1,094 vtable aliases** into the
  original **622,592-byte non-executable data image**. No fake data or .text
  image was supplied.
* The source glob and regenerated actual-object inventory already include the
  new NetZ and four RWA translation units. No separate audio/NetZ import library
  was necessary. The actual exe imports only KERNEL32, USER32 and ole32; /MT CRT
  and librw supply ordinary static-library dependencies.
* Initializer-block functions were not implemented in this step. Fresh
  inventories regenerate exactly the unresolved function-slot traps, including
  required initializers and WinMain. The 393 new catalog rows are not all
  currently demanded slots: implementing wrappers will introduce further
  destructor/callee references, which must then get real providers or traps.

### Global mistakes

All paths below are under `src/auto/`.

| Rejected address | File | Cause and correction |
|---|---|---|
| `0x718` | `004b13c0.cpp` | Null-object field access, not an image global. Use `CSF_AT` with offset 0x718; preserve the original faulting path instead of inventing low-memory storage. |
| `0x4D62B4` | `004d6100.cpp` | Text switch-index bytes. Encode their verified mapping as switch logic: 0/11/14/38 → 0/1/2/3; other in-range commands → 4. |
| `0x53C9A0` | `0053c8d0.cpp` | Unused text jump-table binding; the existing selection logic already implements the dispatch. Remove the phantom binding. |
| `0x560BB8` | `00560960.cpp` | Unnecessary volatile text jump-table read preceding an existing switch. Remove binding/read, not the switch. |
| `0x800000` | `004ef4b0.cpp` | Binary stores an immediate bit pattern, not memory at this address. Standalone stores `1u << 23`. The legacy hook-only read remains explicitly guarded to preserve its requested old DIFF verdict; fixing that hooked behaviour remains open. |

The standalone object for `004ef4b0` was disassembled: it contains
`mov dword ptr [eax + 0x30], 0x800000` and has no rejected data reference.
This verifies emitted instructions, not execution of the constructor.

### Nineteen fake vtables

Each target is a CSV-listed game function, not non-executable data. Packet
annotations used as values now use `CSF_ADDR_FN` (with a pointer cast where
needed to retain the original expression type). Real calls retain their
existing `CSF_FUNC` bindings. For `0x692B10`, four erroneous scratch-buffer
stores were removed: the existing initializer function call constructs the
non-polymorphic file buffer; storing the function address is not construction.

| Function address | Referring edited files |
|---|---|
| `0x402350` | `006b8b10.cpp` |
| `0x41E5F0` | `00475800.cpp`, `006c62f0.cpp`, `006e0230.cpp`, `006fe840.cpp` |
| `0x4276C0` | `00428300.cpp` |
| `0x457AF0` | `0045b870.cpp` |
| `0x4686A0` | `0045ee70.cpp` |
| `0x47EA40` | `007321d0.cpp` |
| `0x48C9F0` | `004b80d0.cpp`, `004d8860.cpp` |
| `0x4AA310` | `004aee30.cpp` |
| `0x4C2380` | `004c2590.cpp` |
| `0x4DE550` | `004de700.cpp` |
| `0x4F1210` | `004f17b0.cpp` |
| `0x536940` | `00536b70.cpp` |
| `0x692B10` | `0050bee0.cpp`, `00517710.cpp`, `00756740.cpp`, `00783340.cpp` |
| `0x6A30A0` | `006a3110.cpp` |
| `0x6A7520` | `006aafe0.cpp` |
| `0x6AC7B0` | `006aca20.cpp` |
| `0x743570` | `0074fbe0.cpp` |
| `0x771B80` | `004a6cf0.cpp` |
| `0x77D4B0` | `0077d560.cpp` |

### Preserved failures, not passes

The before/after direct emudiff verdict lines are identical:

```
00428300 DIFF         1/8 profiles; profile 2: returns 3f800000 vs 0
004ef4b0 DIFF         8/8 profiles; profile 0: 1 dword(s) of memory differ, first at 2a000130: 00800000 vs d9c1de38
00560960 DIFF         1/2 profiles; profile 2: call 6 to f52f27a1: arguments ['2c020101'] vs ['cccccccc']
```

Original gate runs also failed; `00560960` originally failed the volatile-policy
check before reaching emudiff. After deleting the redundant read, its old
emudiff mismatch is still present. Return-value/flag-word behaviour and the
hook-only constructor bug need a separate behaviour-fix review, with a changed
emudiff baseline. The other 28 edited files print `ok` and retain `same` lines
(often fewer than eight usable profiles; no runtime certification is inferred).

## Measured inventories and traps

Fresh source-only command, before and after:
`.venv/bin/python tools/linktest.py -j 8`.

```
compiled 14389/14389 files standalone; 14657 functions implemented
unresolved functions: crt 2, game 95, netz 185, not-a-function 7, png 1, rw 352
```

Before: `2664 globals, 1113 vtables`; after: `2659 globals, 1094 vtables`.
Both: raw addresses 0, original-address asm 0, duplicates 0, compile failures 0.
The capped 50 ordinary undefined references in this source inventory are not
final link errors: linked CRT/Windows/librw providers resolve them.

The full real **slot-object** inventory adds three startup objects (14,392
objects). The ordinary librw archive is linked separately and defines no CSF
address slots. Actual fatal traps:

| Region | Traps |
|---|---:|
| game | 306 |
| NetZ | 817 |
| RW | 428 |
| RWA | 102 |
| CRT | 20 |
| D3DX9 | 321 |
| PNG | 2 |
| not in functions.csv | 1,713 |
| **Total** | **3,709** |

Unknown/code-interior/EH targets are not claimed to be newly identified
functions. Traps abort and return no fabricated result. Source-only RWA demand
is resolved, but data-only/vtable-only RWA demand still produces 102 traps.

## Linux-side dry-run input test

`tools/standalone_check.py` and `tests/test_standalone_check.py` do not run the PE.
They check generated initial bytes against the original data, actual COFF image
bytes and alias definitions, unique validated fixups, defined real/trap targets,
fresh COFF inventory and exactly-once compiled trap definitions. Original table
words/order (including nulls) and generated initializer call order agree; the
entry-source contract places data/init calls before the dry-run exit/WinMain.

Findings from `--require-ready --out build/cutover/dryrun.json`:

* **Structural consistency: ok.** 3,753 aliases, 36,023 fixups (31,587 to code
  slots), **zero undefined fixup targets**. No rejected data remains.
* **Original initializer order: exact.** 282 table slots, 201 non-null calls;
  actual coverage is **0**, all 201 are fatal traps. Listed: 195 game / three
  CRT; three unlisted. Eleven directly annotated game helpers are implemented,
  which does not implement the wrapper slots. If startup reaches the table,
  its first non-null entry `xi[1]`, `0x7AEDE3`, traps.
* A bounded original-game direct-call/basic-block scan visits **296** callees.
  It finds **zero exact and zero conservative local-table candidate vtable
  calls into traps**, but **nine computed indirect calls remain unanalysed**.
  **53 library callees** are deliberately not reverse-implemented or declared
  safe by this scan. This is not an interprocedural alias proof and does not
  certify all initializer virtual calls.
* **Startup ready: false; CLI exit 2 (expected).** Stripped relocation
  completeness, copied IAT/import state, allocator order, old CRT compatibility,
  registered teardown callbacks and runtime pointer completeness remain open.
  The structural regression suite passes **13 tests**. A transient audit during
  an incremental build correctly rejected stale COFF inventory; run this audit
  only with the relevant generated/slot objects quiescent.

## Verification/evidence paths

* `cmake --build build --target csf_standalone -j 8`: successful links, executable
  inspected as i386 PE32, Windows subsystem, actual size above.
* `tools/linkreport.py` on `build/cutover/link.log` and `link-incremental.log`:
  all error categories zero; exit-status files contain 0.
* Final sequential `link-final.log` / `hook-final.log`: standalone target and
  `ninja -C build -j 8` both return 0. Hooked `build/dinput8.dll` is **8,249,856
  bytes**; its build never links librw or standalone boundary objects. Final
  `link-errors-final.json` again has zero errors in every category.
* Per-file `tools/emudiff.py EA --src src/auto/EA.cpp` before/after and
  `tools/gate.py EA src/auto/EA.cpp`: `build/cutover/binding-audit/`, including
  `verdicts.json`, original bindings/hashes and both source-only inventories.
* `.venv/bin/python tests/test_stubgen.py`: six tests pass.
* `.venv/bin/python tests/test_datagen.py`: six tests pass.
* `.venv/bin/python tests/test_standalone_check.py`: 13 tests pass.
* Separate native build with `CSF_RW_CPU_SLICE=ON`, clang-21/clang++-21,
  target `csf_rw_cpu_link`: configures/builds and its Linux CPU probe passes
  all 36 slot references and vector/matrix/frame fixtures. This tests the
  shared librw CMake helper/probe, not Windows startup or game behaviour.
* `git diff --check`: clean. No PLAN/JOURNAL, db, queue or export edits; no commit.

The default DLL rebuild/final incremental build is recorded alongside these
artifacts. Use one Ninja invocation at a time in a build directory (parallel
jobs within it are fine). Ninja reported a premature dependency-log EOF and
performed extra rebuilds, including on a sequential retry. Initial concurrent
invocations are a possible contributor, not a verified explanation for the
persistent warning. Dependency-log/incremental-build performance remains open;
the actual build/link commands succeed. Do not interpret those as game execution.

## Next steps, in dependency order (not execution-verified)

1. Review the three preserved gate failures separately; correcting hooked
   `004ef4b0` necessarily changes its old DIFF verdict. Do not add waivers to
   disguise that as the requested unchanged-baseline mechanical edit.
2. Establish old-CRT initializer/exit compatibility, heap/mutex startup and IAT
   rebinding before game initializers can consume copied data or allocate.
   Audit the nine indirect sites and the required library startup adapters.
3. Implement the 195 listed game initializer wrappers and associated teardown
   callbacks; retain table order, handle the three CRT and three unlisted entries
   explicitly, and regenerate traps for newly exposed callees. No initializer
   may be silently skipped. Replace required boundary traps, not all unused
   catalog rows indiscriminately.
4. Implement the parked `0x00509250` WinMain and the boundary dependencies of its
   actual startup path. Audit unclassified code/EH fixups and pointer completeness.
5. Re-run host structural checks; then obtain a real `CSF_DRYRUN=1` run on an
   already available Windows/Wine environment before any window-opening game
   test. No environment installation or runtime certification was done here.
