# Slice 9: partial toolkit boundary

## Finding

**21 new slots implemented and exercised; six allocated slots already belong
to RWA; one constructor remains unimplemented.** Not a completed 28-slot
toolkit or usable graphics cutover. No duplicate RWA definitions were added.

The allocation is an address partition, not a coherent 2D toolkit. Callers
006b2000/006b2180/006b2240 construct 3D box/sphere/capsule bounds. Callers
006d6fc0/006d6cd0 configure visibility/workspace storage, not images/toon inks.
Five dictionary slots and 007F1810 already live in boundary/rwa/adapter.cpp.

## Address/name audit

Names describe established behavior, NOT recovered formal SDK names. No
database/export renames. All new definitions are cdecl/plain RET.

| Address | Inventory name → caller-confirmed contract | Status |
|---|---|---|
| 007DBD20 | RtDictFindNamedEntry → audio dictionary named lookup | Existing RWA |
| 007DBD70 | RtDictGetEntry → audio dictionary indexed lookup | Existing RWA |
| 007DBD80 | RtDictGetNumEntries → audio dictionary count | Existing RWA |
| 007DBE70 | RpMTEffectDictGetCurrent → audio DEFAULT dictionary | Existing RWA; corrected |
| 007DBE80 | RpMTEffectDictSetCurrent → audio set-current, returning previous | Existing RWA; corrected |
| 007F1810 | unnamed → three audio initialization callback lists, 4/9/2 callbacks | Existing RWA initialize; equivalence NOT verified |
| 007E4970 | unnamed → exact no-op | New; formal identity unknown |
| 007F7900 | Rt2dObjectDestroy → descriptor destroy + aligned release | New; 2D identity rejected |
| 007F7BF0 | GfxTeam_GetObjectBoundsData → transform/bounds dispatch | New; team identity rejected |
| 007F8250 | Rt2dObjectSetMTM → matrix setter, returning descriptor | New; 2D identity rejected |
| 007F8320 | Rt2dObjectSetRenderCallBack → callback setter at +48 | New; 2D identity rejected |
| 007F8330 | Rt2dObjectGetRenderCallBack → callback getter at +48 | New; 2D identity rejected |
| 007F8340 | Rt2dObjectSetColorOffset → recursively propagated scalar offset | New; RGBA/2D identity unestablished |
| 007F8390 | Rt2dObjectSetColorMultiplier → recursively propagated scalar multiplier | New; RGBA/2D identity unestablished |
| 007F83E0 | Rt2dObjectSetPosition → initialize box half-extents | New; corrected |
| 007F8410 | Rt2dObjectGetPosition → primitive dimensions at +40 | New; corrected |
| 007F9860 | Rt2dObjectSetBoundingRadius → initialize sphere radius | New; corrected |
| 007F9880 | Rt2dObjectStringGetHeight → primitive radius at +4C | New; corrected; x87 float load/double return |
| 007F9B40 | GfxScene_Set2dObjectSize → initialize capsule radius/axial half-length | New; corrected |
| 007FAFE0 | Gfx2d_SetSceneVector → configuration vector at +4C | New; corrected |
| 007FB000 | RwImageSetHeight → configuration integer at +8 | New; corrected |
| 007FB010 | RwImageSetWidth → configuration integer at +4 | New; corrected |
| 007FB020 | RpToonInkSetPerspectiveScale → configuration float at +C | New; corrected |
| 007FB030 | RpToonInkSetNearDistance → configuration float at +14 | New; corrected |
| 007FB040 | RpToonInkSetFarScale → configuration float at +10 | New; corrected |
| 007FB050 | RpToonInkSetFarPerspectiveScale → configuration float at +18 | New; corrected |
| 007FD8D0 | Gfx2d_CreateRenderHandle → composite spatial/visibility workspace constructor | OPEN; no success/null stub |
| 007FD9C0 | Rt2dMaestroDestroy → composite spatial/visibility handle release | New; corrected |

Original 007F1810 does NOT register dictionary descriptors: helper 007DC050
invokes each function pointer in three arrays until failure. Existing RWA
initialize simply returns success; preservation of these effects is unverified.
Native schema/string/dictionary tests were NOT run for the six inherited
x86/Windows-only RWA slots. Configuration integer fields appear to be update
budgets (changed entries double them); exact physical meanings of the float
fields are unknown and no ToonInk names are claimed.

## Layout, ownership, dependencies

ToolkitObject is exactly 96 bytes on x86: matrix +0, payload +40, multiplier/
offset +50/+54, controls +58/+5A, descriptor +5C. Type 8 points to an inline
array/count record; x86 child stride is 96. Native fixtures widen pointers and
use sizeof(ToolkitObject). Descriptor destructor +4, bounds callback +10,
flags +40. Static x86 assertions compile.

Reset preserves matrix padding/unrelated payload words, ORs flags 0x20003,
uses scalar 0.2f defaults, selects original descriptor data via CSF_ADDR_DATA:
00884E38/00884EF8/00884F88 (image type IDs 5/2/3). Generated standalone data must
translate their callbacks; this slice does NOT implement all descriptor methods.
Native fixtures supply synthetic descriptor cells/callbacks and do NOT prove
all real primitive operations are callable in the standalone exe.

Object release calls the descriptor with the game pointer then uses the byte
before the aligned pointer as base displacement. Handle release frees the two
configuration arenas (+40C/+418), configuration, aligned workspace, entries,
scratch, nodes and handle, in original order. Count clears before node release.
Allocator comes from engine pointer-table index 67 (+10C on x86), NOT CRT free.

**Slice 4/7 assumptions:** none about atomic/geometry/material/native device
layouts for the 21 implemented slots. Descriptor callbacks retain game
pointers. Matrix multiplication uses slice 1; engine allocator dispatch uses
slice 2's layout (fixture checked). No librw owning-object casts or fictitious
device success. No SDK sources/headers, and librw unchanged.

007FD8D0 cannot safely become a 32-byte allocation: 007FD900 builds spatial
storage through 007FE5C0/007FE050 and configuration through 007FA610/007FA9C0,
with many nested spatial-index, bit-matrix, object-array and arena builders.
The last two alone contain hundreds of exported lines. These have no librw
counterpart; empty substitute arrays would invalidate later game accesses.
Constructor and its builders remain unresolved for a dedicated job.

## Verified

Evidence/logs: work/rw9-check/ (all 28 target packets, caller source audit,
representative original caller packets, compile/build/test and emudiff logs).
No export/db/queue/PLAN/JOURNAL changes.

Native ASan/UBSan with instrumented librw:

```sh
cmake -S . -B /tmp/csf-rw9-asan -G Ninja -DCSF_RW_CPU_SLICE=ON \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g' \
  '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined'
cmake --build /tmp/csf-rw9-asan --target csf_rw_toolkit9_link \
  csf_rw_image_link csf_rw_life_link csf_rw_cpu_link -j 8
for t in toolkit9 image life cpu; do
  ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 \
    /tmp/csf-rw9-asan/src/boundary/rw/csf_rw_${t}_link
done
```

All four pass. Toolkit fixture exercises every NEW slot: matrix copy/no-copy,
parent composition/bypass, callback identity, primitive reset/float returns,
nested/empty child traversal, configuration setters and exact allocation-base
release order. Slices 1–3 pass. LeakSanitizer disabled as in earlier slices;
no general heap-leak-free claim. No Windows execution/game/device test.

Standalone-flags compilation passes. emudiff --src alone compiles hook flags
and sees an empty standalone-only translation unit. A freshly compiled object
with those flags plus CSF_STANDALONE is accepted via --obj with --src:

```sh
.venv/bin/python tools/emudiff.py <addresses> \
  --src src/boundary/rw/toolkit9.cpp --obj work/rw9-check/toolkit9.obj
```

15 slots SAME on accepted profiles: 007E4970, 007F8250, 007F8320, 007F8330,
007F8340, 007F8390, 007F8410, 007F9880, 007FAFE0 and six 007FB0xx setters.
Accepted profiles range 2–8/8; excluded profiles are not passes. Six slots with
inlined original helpers/standalone slot calls (007F7900, 007F7BF0, 007F83E0,
007F9860, 007F9B40, 007FD9C0) report DIFF call events: emudiff mocks the
original helper instead of executing it, or cannot resolve the standalone
indirect matrix slot. These are NOT differential passes; native fixtures
validate behavior, not original runtime equivalence.

## Full-build verification

Full compiled-slot audit:

```text
before: compiled 14780/14780 files standalone; 15118 functions implemented
unresolved functions: crt 2, game 95, netz 229, not-a-function 7, png 1, rw 280
after:  compiled 14781/14781 files standalone; 15139 functions implemented
unresolved functions: crt 2, game 95, netz 229, not-a-function 7, png 1, rw 259
```

Both `.venv/bin/python tools/linktest.py -j 8 --out
work/rw9-check/{before,after}.json` runs exit 0. Empty failures/duplicates/raw/
asm maps. Set-check proves exactly the 21 new slots are the removed RW slots,
with no newly unresolved RW functions. The initial source enumeration was
completed before toolkit9.cpp existed; later new-header recompilation in the
after run is harmless. Existing RWA slots are inherited, not counted as new.
The exact-set/coverage/RET audit also passes (audit.json, set-check.log).

Win32 original-layout assertions, actual librw linking, hooked DLL and
standalone exe builds all pass (exit 0):

```sh
cmake -S . -B build -DCSF_RW_CPU_SLICE=ON
cmake --build build --target csf_rw_toolkit9_link csf_rw_image_link \
  csf_rw_life_link csf_rw_cpu_link csf_hook -j 8
cmake --build build --target csf_standalone -j 8
```

The final separate probe build recompiles final toolkit9.cpp and relinks all
four probes, after the full hooked build finishes. No concurrent Ninja jobs
share the build directory. `file` identifies dinput8.dll, the new probe and
csf_standalone.exe as PE32 i386. The standalone's expanded data/initializer
inventory still needs **335 RW fatal traps** (different set from source-only
linktest's 259): linking does NOT prove this is runnable. Nothing was executed
on Windows. `git diff --check` passes; librw's git status is empty.

## Still open / not verified

007FD8D0 and its spatial/visibility builders; real descriptor-method callback
implementations; inherited RWA registration/schema semantics and native
dictionary/string fixtures; real assets/game/device/Windows execution; malformed
handles and arbitrary reentrant mutation/destruction. The 15 emudiff SAME
results are limited to accepted profiles; six helper-boundary DIFF results
remain non-equivalence evidence, explained above rather than claimed as passes.

## Files

Added boundary/rw/toolkit9.cpp, toolkit9.h, tests/rw_toolkit9.cpp and this
report. Appended integration to boundary/rw/CMakeLists.txt. abi.h untouched;
toolkit records live in the new header.
