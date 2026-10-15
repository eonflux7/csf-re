# Mission world-loading boundary (2026-10-15)

## Finding

The interrupted patch is resumed: **24 new address-slot definitions**, covering
23 of the 25 distinct requested addresses (one render callback is partial).
`RpWorldStreamRead` now returns a game-readable CPU BSP, with world/sector game
plugin callbacks, materials, mesh facades, and rollback. This is **not evidence
that FR01 loads or renders**. World rendering remains unfinished.

Contrary to the task's premise, **this librw checkout does not read BSP worlds**:
`work/tools/librw/src/world.cpp` implements dynamic membership only, and
`rw::World` has no BSP/sector/material-list members or `streamRead`. The boundary
therefore owns the BSP envelope and plane/sector facades, using unmodified MIT
librw for material/texture reading, sector geometry ownership and mesh building.
No SDK sources/headers, db/export changes, or Windows execution were used.

## Per-slot disposition

| Address | Disposition / contract |
|---|---|
| `0059FEA0` | Implemented for non-native CPU BSPs. Caller already consumed WORLD header; preserves following parent chunk. Original version range 3.5.0–3.7.0.2, both root kinds, old triangle order, old/new planes. Native-sector payloads are rejected. |
| `006329C0` | Implemented: stores argument in original static-frame-plugin cell and returns it. Not a registry resize/lock operation. Original frame-list reader uses it to choose child attachment/root updates; native hierarchy/update timing is not original-byte verified. |
| `007FF0E0` | Implemented: aligned 96-byte x86 visibility proxy, original descriptor identity, scalar defaults, object/kind, original default callback identity. That callback (`007FF600`) still traps if executed. |
| `007FDA30` | Implemented: first vacant dictionary entry, high-water cursor, capacity failure, parallel value array. |
| `007FDA80` | Implemented: clears matching entry; decrements cursor only for last slot. |
| `005870B0` | Implemented: borrowed UserData array name pointer. |
| `005863D0` | Implemented: publishes current world, returns first PVS extension word. Despite inventory name, not an arbitrary callback dispatcher. |
| `0056EFF0` | Implemented: render-style word getter. |
| `0056F2B0` | Implemented: unique sector/tied-atomic lightmap textures, low filter byte only, then style word; null world supported. Does not decode lightmap stream extensions. |
| `0056B9B0` | Implemented: BSP line/point/sphere/box/atomic-sphere sector queries, original coarse traversal and stop contract. |
| `0063BF20` | Implemented: versioned STRUCT matrix, optional output allocation, zero-filled short struct, flags and orthonormalization. Oversized struct rejected safely. |
| `0056F380` | Implemented: distinct sector/tied-atomic lightmap count. |
| `0063C9B0` | Implemented with librw BBox point expansion. |
| `0056BAF0` | Implemented CPU world triangle intersections (line/sphere/box/atomic-sphere), facade callback identity and stop. Linear leaf triangle scan, not collision-plugin acceleration order. |
| `007DA4E0` | Implemented: original forward-ray sphere test, projected centre distance divided by segment length; intentionally not a clamped segment entry distance. |
| `00620640` | Implemented: one-sided segment/triangle test with original tolerances and failure-output writes. |
| `0056BDF0` | Implemented CPU atomic triangle intersections: local-space query, morph interpolation, line/sphere/atomic-sphere. No acceleration-tree ordering guarantee. |
| `0058E680` | **Partial**: exposes existing facade default renderer on librw's generic GPU pipeline. Returns null on NULL backend or game Rx pipeline, never passes game pipeline to librw. Native fixture verifies rejection, not a GPU draw. |
| `0063C8B0` | Implemented with librw BBox calculation. |
| `00620840` | Implemented independent sphere/triangle closest-point test, original normal/plane-distance outputs and quick axial rejection. |
| `0056BBD0` | Implemented tied-atomic broadphase, frame synchronization, collision flag/self exclusion, deduplication and callback distances. |
| `0058DE50` | Implemented sector atomic tie-ring visitor with temporary cursor; callback may remove current/next ties. |
| `0058DEF0` | Implemented cached-camera-sector visitor. **Cache population/culling is not implemented**; fixture seeds the cache, not a visibility producer. |
| `0058CBB0` | **Not implemented; trap retained.** Requires camera visibility, per-frame atomic render bookkeeping/culling and working sector renderer; unconditional native clump traversal would not implement this contract. |
| `005903F0` | **Not implemented; trap retained.** Original tail-dispatches engine world-plugin renderer. No facade sector renderer/GPU integration or game Rx pipeline bridge exists. |

Additional `007FF120` clears proxy ownership for descriptor-driven destruction.
The factory necessarily introduces a source reference to `007FF600`; this is
not a fabricated success callback.

## Design and original evidence

Read the requested originals under `export/funcs`, supporting world readers
`0059F440`/`0059F990`, mesh initialization, collision traversal helpers and their
binary disassembly (`work/rwworld2/original.asm`). Callers include `006BEC60`
(stream/header ownership), `006BDB50` (direct sector box writes), `006BCE70`
(sphere query), `006BD830` (PVS/lightmap state), `006D6EE0` (proxy payload and
callback replacement), and collision consumers under `006E*`.

World has original +1C root / +24 clump count / +50 bounds; sector has +48 cell
box / +60 tight box / +78 mesh / +80 material base. x86 layout assertions check
the headers. Native tests widen pointers rather than truncating owning handles.
Plane type is a byte offset (0/4/8), not an axis ordinal.

The decoder bounds chunk envelopes and payload streams before graph publication.
Materials/textures use the existing clump transaction adapters and identity
maps. World/sector constructors and stream readers receive facade identities
and ABI registry offsets. Sector callbacks run after arrays are installed;
world callbacks see the tree/material list before synthesized meshes. Unknown
extension chunks are skipped. UserData uses librw's decoder with borrowed POD
headers in the sector tail. It does not copy native owning plugin headers.

Each leaf holds a private native Geometry. Vertices/UVs/colors borrow compatible
native PODs; packed normals and original base-relative triangle records are
facade-owned. Native mesh material pointers become facade material pointers;
unused native material groups are omitted. Empty streamed leaves publish an
empty mesh header, as original `0059EEF0` does. Native indices retain their owner.
These are regenerated meshes, not an exact original BinMesh strip round trip.

Failure tears down constructed sector/world plugins while identities/materials
remain discoverable, then releases native geometry/material holds and facade
storage. Allocation journals bound unchecked native construction/mesh/UserData
allocations (including must-allocate paths). Callback failure restores the
parent stream cursor to the failing payload. Malformed-input read side effects
and arbitrary callback mutations outside the graph are not reproduced/reversed.
Reentrant world reads are rejected.

The resumed audit added a world-destructor graph-inspection regression. It
exposed a real **ASan heap-use-after-free** in the interrupted patch: sectors
were freed before the world destructor. Teardown is now two-phase: sector plugin
destruction, world plugin destruction with sector headers/arrays still alive,
then storage release. Both ordinary destruction and read rollback use it.
Evidence: `resume/destructor-before.log` and the final native fixture log.

Dynamic atomics have genuine dual facade tie links into sector and atomic rings.
Add/remove-clump and frame synchronization maintain those ties; no vector of
clump members is substituted for sector ties. Static collision queries scan CPU
triangles, preserving callback triangle indices and original query pointers.
The accelerated collision tree's ordering, performance, and unusual reentrant
callback behavior have not been reproduced.

## Verified commands and evidence

The **RW section** of `tools/check_all.sh --no-walk` was executed verbatim as
separate configure/build/test commands (13 targets), without running the script's
Windows dry run. ASan/UBSan configuration is in `work/rwcheck-asan`; commands and
logs are under `work/rwworld2/resume/`. Environment:
`ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1`. All 13 executables
passed. LeakSanitizer is not claimed.

The new world fixture covers two leaves, overlapping and legacy planes, both root
kinds, four stream versions, empty world/mesh, duplicate material references,
two UV sets/colors/packed normals, material/sector/world callbacks, unknown
extensions, UserData, ties/frame moves, cached camera visitor, CPU collisions,
proxy/dictionary/math helpers, following-stream preservation and destruction.
It exercises 3 stream-callback failures, 5 constructor failures and **28 engine
allocation-failure points**, with allocator accounting and retry/teardown checks.
The existing clump fixture also still passes its 75/93 allocation-failure sweeps.

```sh
.venv/bin/python tests/rw_world_math_original.py
.venv/bin/python tools/linktest.py -j 8 --out work/rwworld2/resume/final.json
cmake --build build --target csf_standalone -j 8
git diff --check
git -C work/tools/librw status --short
```

The math harness compares original bytes with compiled x86 COFF: **209 cases**
(17 fixed + 192 fixed-seed random) plus **6 original BSP traversal cases**.
Scalar square-root dependencies are test doubles; not a bit-exact fast-sqrt or
end-to-end collision proof. Native fixture tests the corresponding traversal.

For the before inventory, six changed boundary sources and their changed headers
were compared byte-for-byte with `git show HEAD:PATH`, rebuilt using
`linktest.flags()` with `-DRW_D3D9`, then substituted into
`work/rwworld2/baseline/objects.txt` and inventoried with `linktest.py --objects`.
This reconstructed baseline has RW **162**, not an assumed historical 156/158.
The resumed source inventory is RW **140**: 23 previously unresolved requested
addresses disappear, new `007FF600` reference appears, and `007FF120` gains its
definition (previously not a source unresolved). Final log/JSON give the
authoritative totals. No removed slot definitions, duplicate definitions, raw
original-address hits, or compile failures were found.

Final source inventory (`work/rwworld2/resume/final-link.log`):

```text
compiled 14908/14908 files standalone; 15822 functions implemented
unresolved functions: netz 154, png 1, rw 140
data to generate: 2885 globals, 1154 vtables; raw image addresses in code: 0 in 0 files; asm thunks with original addresses: 0 files; other undefined symbols: 50
```

The final World COFF timestamp was also checked newer than the last streamer
edit before running the math harness and comparing inventories; no stale-object
math result is being used.

`csf_standalone` build completed successfully after the teardown fix (log:
`resume/standalone-final.log`), producing a PE32 i386 executable. Its separate
generated-data/vtable-inclusive inventory has **299 RW traps**. An assertion
against `build/cutover/stubs.json` verifies exactly the two listed requested
addresses remain traps, `007FF120` is defined and `007FF600` remains trapped.
This count is not interchangeable with the 140 source-call unresolved slots.

## Not verified / still open

* No game, smoke, Windows executable or GPU was run. The complete
  `check_all.sh --no-walk` is not claimed: it invokes Windows even with that flag.
* `find /mnt/e/dev/csf-paks/GlobalEK -iname '*.bsp'` found **no files**. FR01's BSP
  and accompanying textures/clumps need extraction from mission-specific paks
  before an asset test is possible. The new fixture is synthetic, not FR01.
* Native/platform-sector streaming, lightmap/PVS/MatFX owning stream extensions,
  collision acceleration payloads, camera visible-sector production, static
  sector drawing and custom game Rx pipelines are incomplete. Existing plugin
  attachment being storage-only is not converted into a decoder by these APIs.
* First-mission load/render success is **not concluded**. In addition to these
  limits, the new proxy preserves a still-trapped default callback `007FF600`.
* PLAN/JOURNAL, db, queue and export are unchanged. Files changed are the RW
  world/stream/query boundary, clump material-reader seam, CPU/toolkit/object/
  UserData slots and headers, RW CMake target, four native fixture sources plus
  the original-byte math harness, and this report.

Exact paths: `src/boundary/rw/{CMakeLists.txt,clump.cpp,cpu.cpp,object.cpp,object.h,
toolkit9.cpp,toolkit9.h,userdata.cpp,world.cpp,world.h,world_queries.inc,
world_stream.inc}`, `tests/{rw_globals.cpp,rw_toolkit9.cpp,rw_world_stream.cpp,
rw_world_userdata.cpp,rw_world_math_original.py}`, and `docs/RW_WORLD2_REPORT.md`.
