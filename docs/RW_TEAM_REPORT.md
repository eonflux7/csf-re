# Rwp physics cluster (historical job name: RW_TEAM)

## Finding / completion

**Partial: 23 of the 33 requested slots implemented; 10 remain traps. This is
RenderWare Physics (Rwp), not RpTeam soldier rendering or Rt2d HUD rendering.**
Replacing the missing operations with atomic rendering or im2d would silently
disable collision detection, activation and integration, not supply the API.

This identification is stronger than a name/neighbor guess: the original PE
contains its own `$Id` strings for `//Physics/Rwp37Active/.../RwpContext.c`,
`RwpBatch.c`, `RwpMgr.c`, `RwpTrilist.c`, `RwpAggChildren.c`, `RwpAggregate.c`,
`RwpDyn.c`, `RwpObj.c`, and the volume and solver modules. E.g. the RwpNull ID
is at 0x00884FD8, immediately preceding the descriptor at 0x00885020 used by
0x007FA5B0. The strings are binary provenance, **not SDK source**. Only original
exports, original image bytes and the MIT librw checkout were used.

No db/csv, export, PLAN, JOURNAL, queue, world.cpp, world_stream.inc or world
render/stream-write code was edited. There is no claim that mission loading or
physics simulation now works. The unimplemented manager constructor is still a
load-path blocker independently of rwworld3.

## Slot inventory and proposed corrected names

Names below are **descriptive Rwp boundary names**, not recovered public symbol
spellings. Module identity and field operations are established; exact SDK
capitalization/visibility is not. In particular neither `RpTeamDictSetFlags` nor
`_rpTeamPlayerSelect` is substantiated by the binary.

Caller column lists original-address filenames in src/auto (hex without `.cpp`).
Every requested slot was grepped in src/auto and src/boundary and its export read.

| Slot | Corrected descriptive identity | Status / contract | Callers |
|---|---|---|---|
| 0x007FA460 | RwpVolumeConfigureMassProperties | Implemented: density/mass, center, inertia tensor/quaternion or spherical inertia, ratio/FLT_MAX return | 006b23e0 |
| 0x007FA5B0 | RwpNullInitialize | Implemented: null volume descriptor, identity basis, scalar defaults, preserve payload/padding | 006b23e0 |
| 0x007FAFD0 | RwpContextSetDeltaTime | Implemented: store float at +0 | 006d6800 |
| 0x007FB060 | RwpContextRefreshCollisionPairs | **Open A**: activation bitsets, spatial overlap pairs, contact-pair refresh/pruning | 006d6800 |
| 0x007FB800 | RwpContextPrepareIslandSelfContacts | **Open A**: island/compound-body contact generation | 006d6800 |
| 0x007FB970 | RwpContextProcessOptionalPipeline | **Open A**: conditionally invokes 0x0080A610; not an optimization no-op | 006d6800 |
| 0x007FB990 | RwpContextPartitionIslands | **Open A**: calls 0x00805360 and updates 40-byte island records | 006d6800 |
| 0x007FD880 | RwpContextStep | **Open A**: contact/constraint solver, integration and post-step phases | 006d6800 |
| 0x007FD8D0 | RwpMgrCreate | **Open B**: owns a batch/spatial tree, context arenas and auxiliary arrays | 006d6fc0 |
| 0x007FDAD0 | RwpMgrGetBatch | Implemented: first handle word, not a dictionary schema | 006d6fc0 |
| 0x007FDB00 | RwpBitMatrixCreate | Implemented: engine calloc of 3-word header plus ceil(rows*columns/32) words; NULL on allocation failure | 006d6fc0 |
| 0x007FDB40 | RwpBitMatrixDestroy | Implemented: engine free, returns 1 | 006d6760 |
| 0x007FE6B0 | RwpBatchAddObject | **Open C**: allocate ID, callbacks, multiple bitsets and spatial insertion; not a flag setter | 006ff680 |
| 0x007FE950 | RwpBatchRemoveObject | **Open C**: deactivation, bitsets, spatial removal and ID reset | 006ff710 |
| 0x007FEDC0 | RwpBatchGetObjectBounds | Implemented: bounded-membership gate, expands tree-stored bounds by margin; preserves padded words | 006ff9a0 |
| 0x007FEE10 | RwpBatchSetObjectEnabled | Implemented: per-ID +0x5c bitset, returns batch | 006ffca0, 0070a6a0, 006ff9f0, 0070ab10, 006ff680, 00700a80 |
| 0x007FEE60 | RwpBatchActivateObject | Implemented: owner membership, capacity, cdecl +0x10 activation callback, count, error latch | 006ffca0, 006ff9f0, 006ff680, 0070ab10, 006d6800, 00700a80 |
| 0x007FEF10 | RwpBatchDeactivateObject | Implemented: cdecl +0x14 callback, clears passed batch bit and decrements count, returns object | 006ffca0 |
| 0x007FEFA0 | RwpBatchSetObjectCollisionGroup | **Open C**: spatial group tag and bounds remove/reinsert | 006d6800, 0070a860 |
| 0x007FF0A0 | RwpBatchSetCollisionMatrix | Implemented: spatial tail pointer at +49176, returns batch | 006d6fc0 |
| 0x007FF0C0 | RwpObjectGetContactList | Implemented: +0x2c pointer | 006ff600 |
| 0x007FF600 | RwpTrilistGatherTriangles | **Open D**: world/sector/atomic/geometry collision-query adapter, 76-byte output triangle records and material masks | toolkit9.cpp callback stored by createVisibilityProxy |
| 0x007FFB90 | RwpParametersInitialize | Implemented: first 16 defaults only, later words untouched | 006d6fc0 |
| 0x007FFBF0 | RwpParametersSetContactLimits | Implemented: four args to +52,+48,+56,+60, not the 16 args in current caller prototype | 006d6fc0 |
| 0x007FFC20 | RwpParametersSetPairCapacities | Implemented: same integer to +40/+44 | 006d6fc0 |
| 0x008006D0 | RwpBoundsScaleAboutCenter | Implemented: 3D endpoints at +0/+16, untouched padding, not 2D | 006d6fc0 |
| 0x008013C0 | RwpMgrGetContext / RwpAggChildrenGetCount | Implemented: shared untyped second-word getter; callers use BOTH pointer and count | 006d6cd0, 00700630, 006d6800, 006d6fc0, 006ff7e0, 006d6760, 006d6280 |
| 0x008013D0 | RwpAggChildrenGetVolume | Implemented: inline array element with original 96-byte stride | 00700630, 006ff7e0 |
| 0x00801520 | RwpVolumeGetPayloadObject | Implemented: volume +0x40 pointer; compound-child container in type 8 | 00700630, 006ff7e0 |
| 0x00802410 | RwpDynAddForceAtPoint | Implemented: accumulate force and (point-center) cross force torque, not player translation | 006ff9f0 |
| 0x00802650 | RwpCylinderGetRadius | Implemented: x87 float-to-double getter at volume +0x44; 00700250 uses it for the transverse extents | 00700250 |
| 0x00802660 | RwpAxialVolumeGetHalfLength | Implemented: shared capsule/cylinder +0x40 getter; 00700250 adds capsule radius for the axial extent | 00700250 |
| 0x00803770 | RwpObjectSetPartTransform | Implemented: active gate, copy part matrix, local translation, quaternion update; unusual inactive return preserved | 0070a460, 0070a6a0, 00700e10, 0070a860 |

Open dependencies are substantive, not uncertainty about the old spelling:

* **A:** Rwp pipeline/partition/solver and contact arenas have no librw counterpart.
  These functions do not render. Their callees include the integrator, constraint
  graph and collision narrow phase; no placeholder hook was installed.
* **B:** `sub_7FD900` allocates the batch via 0x007FE5C0 and 1060-byte context via
  0x007FA610. The existing destroyHandle releases two aligned context arenas and
  aligned workspace. A 32-byte shell alone is not a valid replacement.
* **C:** Object membership mutates the opaque spatial tree (0x00815610,
  0x00815BF0, 0x00815F00) as well as public bitsets and game callbacks. Merely
  filling body IDs would falsely advertise successful collision registration.
* **D:** The callback gathers collision triangles (with capacity, surface masks,
  source identity and local/world flags), not rendered atomics. It needs an
  independently designed collision traversal bridge, including the sector and
  geometry plugin masks at globals 0x008AF2D4 / 0x008AF2D8. The old native-test
  abort sentinel remains; production remains unresolved at this address.

## ABI mapping

All new entry points are cdecl. POD math goes through MIT librw; owning physics
records are independent, game-owned ABI structures, **never native owning casts**.
The original-address macros remain the only address carriers in production code.

* ToolkitObject remains 96 bytes on x86; descriptor at +92 and payload at +64.
  Compound children are inline ToolkitObjects, not librw scenes. Native tests
  expand pointers/stride consistently; x86 offsets have static assertions.
* PhysicsBody exposes callback table +0, part count +12, part list +16, ID WORD
  +32, owner +36, contact list +44. Callback +16 returns int; +20 returns void.
  Callbacks receive the same game-facing pointer. No native atomic is involved.
* PhysicsDictionary (RwpBatch descriptive facade) preserves capacity +68, active
  limit/count +72/+76, body table +80 and five bitsets +88..+104.
* PhysicsMass exposes center +4, mass +16, principal inertia +20, quaternion +32,
  spherical inertia +48 and mode flags +72. Shape descriptor mass callback is
  at descriptor +12. Negative mass means density times volume.
* PhysicsState preserves matrices +0, rotation table +16, forces +48; elements
  point through aggregate +16. Each force/torque pair occupies 32 bytes and
  preserves its padding. Part triples are 12 bytes on x86.
* Bounds use two padded V3d, not `rw::BBox`. Bounds query preserves +12/+28.
  Spatial tree is otherwise opaque; its published bounds table, margin +49172
  and collision-matrix pointer +49176 are read/written without assuming librw
  ownership. New code does not create or destroy that tree.
* RwMatrix cached IDENTITY can be stale after direct game writes. Part-transform
  translation therefore uses the matrix basis/position, clears IDENTITY and
  computes the quaternion, rather than librw's identity-shortcut multiplication.
* createBitMatrix improves original allocation failure from null dereference to
  NULL return; valid allocation layout and engine allocator dispatch are retained.

## Verification

Native tests are wired into check_all.sh via the normal `csf_rw_*_link` CMake
target discovery; original-byte fixtures are an explicit offline check.

* `cmake --build work/rwcheck-asan --target csf_rw_team_link csf_rw_toolkit9_link -j 8`
  followed by both binaries with `ASAN_OPTIONS=detect_leaks=0
  UBSAN_OPTIONS=halt_on_error=1`: passed after fixing a callback-type UBSan
  finding and a stale-IDENTITY translation bug caught by the new tests.
* `tests/rw_team.cpp`: all 23 slots, callbacks, bit index 33, activation
  success/failure/full/idempotence, error latch, alloc/free layout, defaults,
  untouched bytes, density/mass/inertia, bounds, force/torque, inactive/null-owner
  returns. Real serialized frame matrices from the first FR01.rws clump are
  exercised when that extracted asset is available (not a world-read test).
* `.venv/bin/python tests/rw_team_original.py`: passed against original PE bytes;
  independent checks for defaults, padded bounds, null-volume initialization,
  mass/inertia, activation/deactivation callbacks and force/torque. Checks ESP
  and nonvolatile registers; mass-properties callback is a controlled double
  and x87 sqrt substitutes for the uninitialized engine lookup table. This is
  **not** a claimed complete original/COFF differential test.
* Full final check_all/linktest/standalone results are recorded below after the
  runs finish. The first check_all run caught the translation assertion; do not
  count that initial run as passing.

No Windows, mission dryrun, smoke, GPU or complete physics-world test was run.
Actual Ransom.phd exists, but its physics definition streaming is outside the
implemented subset. No game data was copied into the repository.

## Files changed

`src/boundary/rw/toolkit9.h`, `toolkit9.cpp`, new `toolkit_physics.inc`,
`src/boundary/rw/CMakeLists.txt`, new `tests/rw_team.cpp`,
`tests/rw_globals.cpp`, new `tests/rw_team_original.py`, `tools/check_all.sh`,
and this report. Test globals include two pre-existing missing plugin offset
cells (0x008B1404/0x008B1408), needed to link the native suite on this checkout.

## Final tool results

`.venv/bin/python tools/linktest.py -j 8` exited 0:

```
compiled 14926/14926 files standalone; 15918 functions implemented
unresolved functions: netz 122, png 1, rw 78
data to generate: 2891 globals, 1154 vtables; raw image addresses in code: 0 in 0 files; asm thunks with original addresses: 0 files; other undefined symbols: 50
```

Zero compile failures and duplicate definitions. Compiled-slot inventory confirms
exactly 23/33 requested definitions; the ten absent slots are precisely the open
rows above. No fresh pre-edit inventory was taken, so no inferred baseline count
is presented as a measured run. Build acceleration reused upstream COFF cache
entries only for byte-identical src/auto sources after comparing all headers
(only toolkit9.h differed, not included by these files), compiler commands
(normalized checkout path) and upstream object freshness. All changed RW code
was compiled locally.

Final `tools/check_all.sh --no-walk --offline` native test phase: all 15 target
invocations exit 0, including the new ASan/UBSan physics target. Both real FR01
frame matrices passed. Its linktest and NetZ checks also passed (1044 emulated
calls). Full log: `work/rwteam-check-final.log`. The whole script is **not claimed
passing**: the cold hook DLL build has not completed at report time.

`cmake --build build --target csf_standalone -j 8` exited **0**, produced
`csf_standalone.exe`; log `work/rwteam-standalone.log`. Its additional full-link
inventory (including generated data/address-taken callbacks, NOT the source-only
78-RW inventory above) reported:

```
compiled 14929/14929 files standalone; 15918 functions implemented
unresolved functions: crt 21, d3dx9 321, netz 770, not-a-function 1420, png 4, rw 237, rwa 105
```

Those remaining full-link slots are emitted as fatal traps; a successful build
is not a claim that physics or all those callbacks work.

Final edge-case follow-up: original-byte fixtures confirmed that inertia scaling
uses **strictly greater than FLT_MIN**, not greater-or-equal. Corrected that
predicate and added native/original equality tests. After this last edit, rebuilt
and ran all 15 native targets again (all exit 0, log
`work/rwteam-native-final.log`) and explicitly recompiled toolkit9.cpp with
`linktest.compile_one(linktest.flags(), source, inc_mtime)`: MSVC x86 ABI
assertions passed and all 23 new COFF slot definitions were verified. Relinked
the final exe using the **unchanged CMake-generated link command**, extracted
with `ninja -C build -t compdb -x CXX_EXECUTABLE_LINKER__csf_standalone_Debug`;
exit **0**, log `work/rwteam-relink-final.log`. No interface, slot or data binding
changed in this predicate-only follow-up. The complete cold hook build/full
check_all script still has no completion result; do not count it as a pass.

Separately ran `.venv/bin/python` on `tests/wmrest_offline.py`,
`tests/rw_particles_original.py`, `tests/rw_team_original.py`,
`tests/audio1_offline.py`, `tests/test_datagen_text.py`: all passed (log
`work/rwteam-offline.log`). `git diff --check` passed. No fresh baseline run or
Windows runtime result is claimed.
