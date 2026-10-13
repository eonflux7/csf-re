# First standalone link (diagnostic, not a runnable game)

Configure the existing hooked build, then request the **opt-in** target:

```
cmake -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/clang-cl-x86.cmake
cmake --build build --target csf_standalone -j 8
```

`ninja -C build` still selects `csf_hook` / `dinput8.dll`, not the executable.
No hooked objects or compiler options are repurposed. Standalone src objects
are compiled by `linktest.py` using its audited `flags()` into the separate
`build/standalone/` cache; generated startup/traps have their own CMake objects.
The target can live in another build directory, but `build/` must first be
configured: that is the existing linktest flags/cache contract.
The private cache is invalidated when flags change and tracks all src/hook
headers, not just a fixed short header list.

## Generated inputs and explicit gaps

`build/cutover/` contains:

* `base.json`: un-stubbed src inventory (all auto/core/types/boundary sources).
* `data/data.cpp`, `data/datagen*.json`: original **data** image and validated
pointer fixups; no original executable section is copied or executed.
* `initializers.cpp`, `initializers.json`: original table order and every entry,
  including null slots, database names/regions, actual slot coverage, and
  direct callee annotations for compiler-generated thunks absent from the DB.
  `initializers.tsv` lists every non-null entry for review;
  `startup-disassembly.txt` preserves the binary evidence for table discovery.
* `objects.txt`, `linktest.json`: exact un-stubbed linker inputs, including
  generated data/startup. This inventory is larger than the src-only test.
* `stubs.cpp`, `stubs.json`: exactly those unresolved **function** slots.
  `stubgen.py` independently checks actual COFF inputs against the inventory
  and refuses existing definitions, stale inventories, or duplicates.
  Failed COFF inspection also stops generation; it never treats an unreadable
  object as having no definitions.

Every trap records the original address and database name in inspectable
variables, then the one `csf::unresolved(address)` prints, flushes stderr and
aborts. It never returns a fabricated result. Extra stack/register arguments
do not require a function-specific ABI because the trap cannot return.
Adding a real definition regenerates a smaller trap list automatically.
Addresses absent from `functions.csv` stay explicitly `not-a-function`; they
are not silently recategorized as implemented game/library functions.

Datagen's `--allow-rejected` is **diagnostic only**: it warns and emits its
valid subset so the final linker can enumerate rejected gv/vt references.
It does not invent arrays, vtables, code-image storage or function definitions.
Default datagen behavior still returns 2 for rejected references.
Data-pointer inventory completeness, imported IAT pointers, text-embedded
data and remaining raw-address assembly are not solved by linking traps.
The original image is needed **at generation time only**, not loaded as a
runtime fallback. Copied resource bytes are not automatically a new PE resource
directory, and the original copied IAT is not the new executable's import table.

Hook-only `src/core/devcmd.cpp` is empty under CSF_STANDALONE. Its original
IAT patches, smoke instrumentation and RW wrap-and-call-original trampolines
cannot work in an exe; their game/RW slots remain explicit unresolved work,
not wrappers with null original pointers. Other core sources are retained.

## Entry and original initialization

`standalone/main.cpp` uses normal modern /MT `WinMainCRTStartup` with the
Windows subsystem; no custom CRT bypass, `/FORCE`, or undefined-symbol waiver.
Host CRT/C++ initialization happens first (including HOOK_METHOD bindings).
WinMain initializes the generated image, invokes ordered original initializer
**slots**, checks C initializer errors, then calls `_WinMain@16` at
`CSF_FUNC(..., 0x00509250, ...)`. The game WinMain is parked in queue/ and
therefore currently receives a fatal trap, not a substitute game entry.

Binary evidence: PE entry 0x007B08CF calls `__cinit` 0x007B2922 before game
WinMain. `initgen.py` locates these by PE entry/CSV names and decodes the two
inlined table loops (it validates the register/range pattern and sentinels):

| table | begin | end sentinel (exclusive) | traversed slots | non-null |
|---|---|---|---:|---:|
| `__xi_a..__xi_z` | 0x00898454 | 0x0089846C | 6 | 5 |
| `__xc_a..__xc_z` | 0x00898000 | 0x00898450 | 276 | 196 |

The 201 initializer entries are all retained, C first then C++, increasing
table-address order, ignoring nulls. C entries return int and terminate
startup on a nonzero result; C++ entries return void. Missing entries trap
at their exact address and do not get silently skipped, including original
CRT/library initialization entries. Thus a dry run is not promised to finish.
The original CRT's non-table startup/teardown internals are not replayed over
modern CRT state; exit/atexit compatibility remains separate boundary work.
In particular, original `__cinit` also calls the original 0x008AA024 callback
(initial value 0x007AEA3A, with argument 1) before the tables and registers
0x007B9019 with `_atexit` between them. Those **CRT-internal**, non-table steps
are not executed against copied original CRT state. This is not a complete
MSVC 7.1 CRT-startup compatibility layer.

Most initializer wrappers are absent from `functions.csv`: the coarse NetZ
tail range is mixed and is **not** evidence all these wrappers are NetZ.
The generated JSON separates listed regions from unlisted thunks and records
their direct callees/coverage, without claiming underlying implemented
constructors are implemented wrapper slots.
Annotations stop at a return or unconditional jump and are not a reconstructed
call graph; table-entry coverage itself is measured from real COFF definitions.

`CSF_DRYRUN=1` performs data initialization and the same initializer sequence,
then exits before the game WinMain. Do not run the exe without this flag in
offline work. Even with it, unresolved initializers may abort and the copied
data/allocator startup has not been runtime-certified. Use Wine only if
already installed; do not install it or run a window-opening game test.

## Reporting a final link

Save CMake output, then group the actual lld-link diagnostics:

```
cmake --build build --target csf_standalone -j 8 > build/cutover/link.log 2>&1
.venv/bin/python tools/linkreport.py build/cutover/link.log --out build/cutover/link-errors.json
.venv/bin/python tools/linktest.py -j 8
.venv/bin/python tests/test_stubgen.py
.venv/bin/python tools/gate.py 00401000 src/auto/00401000.cpp
```

`linkreport.py` counts unique errors by function slots, globals, vtables,
other undefineds, duplicates, sections, imports, missing import libraries,
and remaining linker errors. It suppresses nothing and does not treat an
empty error list as proof an executable was produced.

## Measured first link (2026-10-14)

**The exe does not link yet.** The actual lld-link attempt resolves every
function slot using fatal traps, and stops at precisely **24 data symbols**:
five `gv` and nineteen `vt`. All remaining kinds are zero: other undefineds,
function slots, duplicates, sections, imports, missing libraries, other errors.
No `/FORCE` or fake data definitions are used. Existing Windows import libraries,
/MT CRT and Windows subsystem already satisfy the mechanical link requirements.
The final command's exit status and grouped errors are saved in
`build/cutover/link-exit-status.txt` (255), `link.log`, `link-errors.json` and
`link-errors.txt`. An incremental retry repeats only the link and the same gaps.

The five rejected global addresses are `0x718`, `0x4D62B4`, `0x53C9A0`,
`0x560BB8`, `0x800000`; the first is unmapped, the rest lie in executable .text.
All nineteen rejected vtable addresses lie in .text too. The full addresses,
reasons and referring source locations are in `data/datagen-rejected.json`
and the linker log. Resolving these requires reviewing the caller's binding
kind or establishing safe extents for genuine text-embedded data, not inventing
arrays or copying executable sections.

The fresh source-only inventory compiles **14,363/14,363** files and defines
**14,510** real function slots, with no duplicates, failures, raw image addresses
or original-address assembly. These are newer counts than the task's baseline.
Generated data and startup make substantially more dependencies explicit:

| kind | src-only unresolved | all real inputs / fatal traps |
|---|---:|---:|
| game | 99 | 127 |
| NetZ | 222 | 857 |
| RW | 402 | 478 |
| RWA | 40 | 142 |
| PNG | 1 | 2 |
| CRT | 0 | 18 |
| D3DX9 | 0 | 321 |
| not in functions.csv | 7 | 1,905 |
| **total** | **771** | **3,850** |

The latter includes pointer/vtable-only dependencies and initializers, not just
calls from maintained source. In particular, the 1,905 unknown targets are not
claimed to be newly discovered functions: code-interior/EH pointers and unlisted
initializer thunks need classification. Traps make those slots link-resolvable,
not behaviorally implemented. Datagen emits 2,638 global / 1,094 vtable aliases
into its 622,592-byte data image and applies 36,023 validated pointer fixups.

Both original initializer tables together contain **201 non-null entries;
covered 0, missing 201**. Three are CSV-listed CRT entries and three CSV-listed
NetZ entries; 195 are unlisted. There are 118 entries with direct game-region
callees, reaching eleven distinct game helpers, all eleven covered. This does
not implement those 118 initializer wrappers: their table slots remain missing.
The generated TSV/JSON lists every entry and its actual slot coverage separately
from these callee annotations.

Verified commands: `cmake --build build --target csf_standalone -j 8` (link
failure above), `tools/linkreport.py` on its log, `tools/linktest.py -j 8`,
`tests/test_stubgen.py` (six tests), `tests/test_datagen.py` (six tests), and
`tools/gate.py 00401000 src/auto/00401000.cpp` (`ok`), all Python commands via
`.venv/bin/python`. `ninja -C build -j 8` succeeds with no work to do; the existing
`dinput8.dll` SHA-256 remains
`440062aae698c125439953481ba4edcc82f242b9f60ef40a9d23bfb7cf033a88`.
Wine is absent from PATH; no executable or game was run.
An independent COFF check of the final CMake objects confirms exactly all 3,850
trap slots, no overlap with real definitions, and exactly-once definitions of
all 3,732 accepted data aliases. The final error addresses equal the rejected
data manifest exactly.

Next, in order: (1) audit the 24 rejected data bindings; (2) classify unlisted
pointer targets and implement initializer wrappers / establish a modern-CRT
compatibility policy and allocator startup order; (3) implement the parked game
WinMain and replace remaining boundary traps; (4) obtain a successful link,
then a dry run on an already available Windows/Wine environment. A successful
diagnostic link alone would not certify pointer completeness or game startup.
