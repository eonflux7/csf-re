# Decision paper: game functions under six bytes

All 166 tiny todo game functions (736 bytes) are mechanically implementable and have room for the existing five-byte entry hook. They are excluded by `select(min_size=6)`, not by insufficient patch space. This tree contains 166 generated candidates; all 166 passed the gate and returned `same` under its emulator check. No hook header change is needed for this set.

The current snapshot is HEAD `b908139c`. Its other todo game functions outside STL and queue number **29**, not the brief's 27. There are 195 leftovers in these two groups. The journal's “192 = 166 + 27” was itself arithmetically inconsistent. This report uses the actual CSV, exports and queue directory, unchanged.

## Classification and references

The selection is `region=game`, `status=todo`, CSV `size<6`. Every size agrees with `export/functions.jsonl`. Bytes are read from `work/pc/CommXPC.exe` using its PE mapping, and decoded with Capstone. Direct call/jump candidates come from the exported xrefs and are verified by disassembling their caller bytes. Vtable locations come from `db/vtables.json`; all 645 selected slot values were independently compared with the original image. Other pointer occurrences are scanned in the image, with instruction operand checks and exclusion of known strings.

| Original shape | Functions | Bytes | Direct, no known vtable | Vtable, no direct edge | Both | Other pointer users |
|---|---:|---:|---:|---:|---:|---:|
| `jmp target` | 38 | 190 | 9 | 24 | 3 | 2 |
| `mov al, [ecx+N]; ret` | 27 | 108 | 1 | 16 | 8 | 6 |
| `mov byte [ecx+N], imm; ret` | 23 | 114 | 14 | 8 | 1 | 0 |
| `lea eax, [ecx+N]; ret` | 21 | 84 | 3 | 9 | 9 | 4 |
| `mov eax, [ecx+N]; ret` | 20 | 80 | 3 | 3 | 14 | 2 |
| `fld [ecx+N]; ret` | 17 | 68 | 0 | 13 | 4 | 3 |
| `lea eax, [ecx-N]; ret` | 5 | 20 | 0 | 2 | 0 | 4 |
| `and dword [ecx+N], imm; ret` | 3 | 15 | 1 | 1 | 1 | 0 |
| `movzx eax, [ecx+N]; ret` | 3 | 15 | 0 | 3 | 0 | 0 |
| `xor al, al; ret N` | 3 | 15 | 0 | 1 | 2 | 2 |
| `mov eax, [ecx]; jmp [eax+N]` | 2 | 10 | 0 | 2 | 0 | 0 |
| `inc dword [ecx+N]; ret` | 2 | 8 | 2 | 0 | 0 | 0 |
| `xor ax, ax; ret` | 1 | 4 | 0 | 1 | 0 | 0 |
| `xor eax, eax; ret N` | 1 | 5 | 0 | 1 | 0 | 0 |

“Direct” includes direct calls and tail jumps; the columns count functions, not call sites. The last column overlaps the first three. Totals: **33 direct/no-vtable, 84 vtable/no-direct, 42 both, seven other-pointer-only, zero unreferenced**. The seven pointer-only entries are `00493810`, `004ef500`, `0050a4e0`, `0050a500`, `006b35b0`, `006f3b20`, `00709800`. These are other aligned data pointers, not proven dynamic invocations.

There are 849 caller-function/callee pairs using calls, 32 using direct jumps, 645 vtable slots across 387 tables, and 222 other pointer locations (all aligned, all in `.rdata`). The 775 unique direct callers include 628 with CSV status done. Full caller addresses, vtable addresses/slot indexes and pointer locations are in `work/tiny-functions.json`; one line per function, including original bytes, is in `work/tiny-functions.txt`.

A naive address-byte search produced 16 false pointers to `00752520`: its little-endian representation is the format-string suffix ` %u` plus NUL. The analyzer excludes these verified string occurrences, including four aligned ones. Its JSON records the excluded locations separately.

There are no plain `ret`, plain `ret N`, `xor eax,eax; ret`, or `mov al,1; ret` entries in the selected todo set. Six game functions smaller than six bytes were already done: one one-byte function, four three-byte functions, and one four-byte function. The generator recognizes additional simple patterns, but only the 14 observed patterns were exercised here. `movzx` entries are two word loads and one byte load; the zero-return pop counts are 4, 8, 12 (AL) and 16 (EAX).

## Installation and cutover

The actual header is `hook/hook.h` (there is no `hook/hooks.h`). `HOOK`/`HOOK_METHOD` register `Kind::Jmp`; `hook/main.cpp::install` writes `E9 rel32` across five bytes, through a counting stub. It does not check original size or patch room. `HOOK_TRAMP` delegates prologue relocation to MinHook. `HOOK_VSLOT` writes a four-byte pointer at one specified slot: this does not cover other slots or direct callers, and its standalone expansion defines no function-address slot.

The smallest normal JMP-hookable **body** can therefore be one byte (`ret`) if four additional bytes are safe padding. The smallest necessary writable span is five bytes. `src/auto/00506ac0.cpp` already implements a one-byte original body using `HOOK_KEEP_ECX`. A four-byte function is not automatically unhookable. This selected set has 94 four-byte bodies followed immediately by `CC`, and 72 five-byte bodies. The next function is +16 for 155 entries, +32 for eight, +48 for two and +64 for one. No selected entry needs a short-jump scheme, hotpatch scheme, or vtable-only installation.

Today `CSF_FUNC` in the hook build always calls the original entry address. A done caller of an unimplemented tiny function executes its original bytes; a virtual caller uses the original vtable. Once the tiny hook is enabled, either path reaches the entry JMP and replacement. Thus the tiny todo statuses do not leave missing runtime implementations in the hybrid build. This follows from the source; no game installation was run here.

Standalone `HOOK` defines `csf::fn_slot<EA>::p`; `CSF_FUNC`/`CSF_ADDR_FN` reference that address-keyed symbol. Globals and vtables use separate generated `gv`/`vt` storage. Original executable bytes cannot supply a fallback. Before generation, 66 tiny addresses had 715 bindings in source, but only **65** appeared as unresolved object references in linktest. All 166 definitions will also be available to future generated vtable/callback data. Linktest currently counts references emitted by compiled source, not every pointer in the original image, so closing its 65 tiny unresolved slots is not proof that cutover data is complete.

## Proposal and implementation

Generate this set without model calls and keep it as candidates for orchestrator approval and normal installation/evidence handling. Do not lower the lane threshold merely to spend model calls on byte-defined functions.

* **126 leaves:** return/load/store/field-address expressions from the decoded instructions; explicit scalar return width; ECX-relative methods use `__fastcall(self, dummy_edx)`; `ret N` supplies N/4 unused stack dwords. Byte getters return `uint8_t`, not normalized bool. Field offsets, store values and signed address adjustments come from bytes, not pseudocode names.
* **Five of those leaves** (`00471540`, `0052d5c0`, `006b6af0`, `006f2860`, `00711d70`) use existing `HOOK_KEEP_ECX`, based on `regargs.needs_keep_ecx`. No new preservation wrapper was added.
* **38 direct aliases:** a naked `jmp dword ptr [tiny_target]`, with `tiny_target = CSF_ADDR_FN(target)`. It forwards all registers, original stack arguments, return address and result ABI without guessing the target signature or creating a new C++ call. There are 38 distinct targets: 34 done game targets, one other generated tiny todo target, two RW targets and one NetZ target. The three library targets remain boundary bindings, not reimplementations.
* **Two virtual forwarders:** copy the decoded `mov eax,[ecx]; jmp [eax+slot]` into the gate's already permitted naked-tail-thunk form. Existing `HOOK_TAIL` is unnecessary here, and its helper would introduce register clobbers absent from this simple forwarder.

`tools/tinygen.py` accepts summary, `--list`, `--others`, `--json FILE`, and `--write [EA ...]`. It only generates recognized shapes smaller than six bytes with safe five-byte patch room, never overwrites an existing candidate, and never writes the database. `--write --gate` validates each newly created candidate and removes its own failures. The generator currently uses existing `HOOK` and `HOOK_KEEP_ECX`; no `hook.h`/`globals.h` support is needed. Future functions without five safe bytes would need another strategy, but there are **zero** in this set.

There is no immediate hybrid-runtime requirement to replace any of the originals. For cutover, the 65 unresolved tiny symbols need definitions now; the other 101 are not unresolved in the current object graph and can wait until generated data or other callers require them. Since generating all 166 is small and deterministic, this patch supplies all rather than maintaining two implementations of the selection policy. Leave all CSV rows todo until the orchestrator chooses the production/evidence status.

## The other 29

Source checks identify six hard exclusions: two hand-owned hooks, one missing pseudocode export, three without a module. Twenty more entries are selectable in a module-specific lane but their modules currently rank below 60. Rolling `admit()` calls `rank_modules(db, 60)` **before** filtering `seen`; its top 60 includes queued and tiny todo entries that selection will reject. Once those top modules have been visited, it can report ranking exhausted while lower-ranked selectable modules remain. Recompute across all modules or filter for selectable work before truncation; do not infer completion from that message.

Three entries within the window were previously produced, then returned to todo; “no lane took it” is false for them. `git show 4651dcec -- db/functions.csv` shows the resets of `004958c0` and `004d31e0`, while `7a256dbf` documents `0069ae30` returning from queue. The worktree contains no historical grunt/wave logs, so snapshot ranking explains a present exclusion, not the exact historical order of every leftover. Clone grouping is not a permanent exclusion: completed siblings provide examples, and all-todo clusters offer one member at a time.

* `0045aa70` (114 B): module rank 83, outside rolling admit()'s top-60 window
* `004958c0` (220 B): Selectable now; previously done, reset to todo by emulator sweep commit `4651dcec` (current module rank 55).
* `004b2990` (111 B): module rank 72, outside rolling admit()'s top-60 window
* `004b2a40` (74 B): module rank 72, outside rolling admit()'s top-60 window; clone cluster of 2 ({'todo': 1, 'done': 1})
* `004b9660` (78 B): module rank 88, outside rolling admit()'s top-60 window
* `004d31e0` (384 B): Selectable now; previously done, reset to todo by emulator sweep commit `4651dcec` (current module rank 54).
* `004db5f0` (43 B): module rank 69, outside rolling admit()'s top-60 window
* `004db620` (41 B): module rank 69, outside rolling admit()'s top-60 window
* `004fb8f0` (47 B): Hand-owned hook in `src/core/devcmd.cpp`; select skips it; CSV still todo (also rank 101).
* `00506cf0` (127 B): no pseudocode export/funcs/<ea>.c (select() requires it)
* `00521bc0` (34 B): module rank 104, outside rolling admit()'s top-60 window
* `005364d0` (46 B): Hand-owned hook in `src/core/devcmd.cpp`; select skips it; CSV still todo.
* `00692390` (16 B): module rank 79, outside rolling admit()'s top-60 window; clone cluster of 10 ({'done': 6, 'lib': 3, 'todo': 1})
* `006923c0` (18 B): module rank 79, outside rolling admit()'s top-60 window
* `006923e0` (34 B): module rank 79, outside rolling admit()'s top-60 window; clone cluster of 59 ({'done': 58, 'todo': 1})
* `0069ae30` (62 B): Selectable now (rank 53); returned from queue by `7a256dbf` after the stack-probe emulator fix.
* `006b6fe0` (62 B): module rank 96, outside rolling admit()'s top-60 window; clone cluster of 7 ({'done': 5, 'queue': 1, 'todo': 1})
* `006d0dd0` (24 B): module rank 66, outside rolling admit()'s top-60 window
* `006f5510` (34 B): module rank 76, outside rolling admit()'s top-60 window; clone cluster of 3 ({'todo': 3})
* `006f5540` (38 B): module rank 76, outside rolling admit()'s top-60 window; clone cluster of 3 ({'todo': 3})
* `006f5570` (34 B): module rank 76, outside rolling admit()'s top-60 window; clone cluster of 3 ({'todo': 3})
* `006f55a0` (38 B): module rank 76, outside rolling admit()'s top-60 window; clone cluster of 3 ({'todo': 3})
* `006f55d0` (34 B): module rank 76, outside rolling admit()'s top-60 window; clone cluster of 3 ({'todo': 3})
* `006f5600` (38 B): module rank 76, outside rolling admit()'s top-60 window; clone cluster of 3 ({'todo': 3})
* `0075ffe0` (80 B): module rank 93, outside rolling admit()'s top-60 window; clone cluster of 2 ({'done': 1, 'todo': 1})
* `007613e0` (15 B): module rank 93, outside rolling admit()'s top-60 window
* `007a3270` (18 B): no module (module waves and the ranking never see it); clone cluster of 2 ({'todo': 2})
* `007a32c0` (18 B): no module (module waves and the ranking never see it); clone cluster of 2 ({'todo': 2})
* `0082a670` (10 B): no module (module waves and the ranking never see it)

## Verification

Ran the executable analyzer, its per-function listing, other-function report, JSON export and byte assertions against all 645 catalogued vtable slots. Recomputed the module ranking formula from the CSV and exported callers without importing or running grunt. Inspected `select`, `rank_modules`, rolling `admit`, hook installation, address-binding macros, linktest and the two historical reset commits.

The first ten were generated and gated with:

```sh
.venv/bin/python tools/tinygen.py --write --gate 004053f0 00405820 00409460 004206c0 00471540 0047b680 00481820 004924f0 004cfa10 006dba30
```

All ten passed. Generated the rest with `.venv/bin/python tools/tinygen.py --write`. Four additional representatives passed `gate.gate`, completing all 14 shape families; the other 152 then passed `gate.gate` with one shared DB (every verdict is logged in `work/tiny-all-gate.log`, machine results in `work/tiny-gate-rest.json`). Total: **166/166 gated**. Gate compiles a fresh temporary object from each source; no stale build object was used. Its emudiff gate was on and logged **166 unique `same` verdicts**, 164 with 8/8 profiles and the two virtual forwarders with 4/8 profiles. The latter have four inconclusive profiles each, not eight completed comparisons. This proves the tested emulator states, not in-game installation or every possible caller ABI. Registers/flags outside the modeled behavioral contract are not claimed byte-identical for C++ leaves.

Also ran `.venv/bin/python tools/emudiff.py 004e5df0 --src src/auto/004e5df0.cpp` independently: `same`, 4/8 profiles, using a fresh source compilation. Compared all 166 existing files with regenerated text: exact match. Unknown-shape and library-region rejection assertions passed.

Ran `.venv/bin/python tools/linktest.py -j 4` before and after generation. Both exit 1 because of the same pre-existing failure in `src/auto/006910f0.cpp`: explicit specialization of `p` after instantiation. Neither has duplicate address definitions.

| Standalone metric | Before | After |
|---|---:|---:|
| Files compiled / total | 13,712 / 13,713 | 13,878 / 13,879 |
| Functions defined | 13,717 | 13,883 |
| Unresolved game functions | 410 | 345 |
| Selected tiny unresolved functions | 65 | 0 |
| Compile failures | 1 | 1 |

Unchanged unresolved boundaries: CRT 49, NetZ 319, not-a-function 5, PNG 1, RW 398, RWA 40. Unchanged data requirements: 2,655 globals and 1,077 vtables. Unchanged old issues: two raw addresses in one file and original addresses in 15 asm-thunk files. The generated forwarding thunks bind their target addresses via `CSF_ADDR_FN`, so they add no raw-address issues. Snapshots: `work/tiny-link-before.json`, `work/tiny-link-after.json`.

`git diff --check` passed. The changes are this paper, `tools/tinygen.py`, and 166 new `src/auto/<ea>.cpp` files. `docs/PLAN.md`, `docs/JOURNAL.md`, `db/`, `queue/` and `export/` were not changed. No commits, network, game, smokes, verifier, grunt execution, or IDA writes were attempted.

## Still open / not verified

The orchestrator decides whether to take/install the candidates and update statuses; no live hook or smoke evidence exists from this job. Future standalone vtable/callback generation must replace all original function pointers. The existing allocator standalone compile failure, other unresolved functions and library boundaries remain. The rolling top-60 selection bug and missing-module/export/owned-hook status decisions are findings for separate changes; this job changes neither production scheduling nor the database.
