# RenderWare Graphics → librw: cutover survey and CPU slice

## Finding and scope

Use **an RW 3.x game-facing ABI façade with librw sidecars** for objects with
hierarchies, ownership, callbacks or plugin payloads. Keep compatible mathematical
value types native. Do not replace library instructions, import SDK declarations,
or point the game at arbitrary librw objects merely because an API name matches.
The MIT checkout is `work/tools/librw`, commit
`18532c2e13efbc1aa43f0b00b4459831f9414de0`; it is the only RW reference used here.

The address inventory below is the **pre-slice compiled linktest inventory**,
rerun on this worktree on 2026-10-14. It contains **400**, rather than the task's
cached 399. The interrupted attempt had only 392 rows (source-derived) despite
claiming a compiled 399 inventory. This resumed audit replaces that incomplete
table with the actual compiled-slot set, including address-taken callbacks.
“References” means distinct compiled caller files (not raw xrefs or total calls).
The exported `boundary_api.json` has 416 RW-region entries: that is a different
set from the live standalone slots. Never use a text
grep as the authoritative unresolved-slot count.

Fit classes describe API correspondence, **not verification of original-library
behavior**. “Same semantics” is the normal valid-input mathematical operation or
value getter; it permits last-bit floating-point differences. “Differs” means an
equivalent facility exists but needs ABI/return/callback/layout/format adaptation.
“Missing” means no exact operation in this checkout: an independently designed
boundary or composition of available primitives is needed. In particular, a
missing public method is not necessarily a missing mathematical primitive.

The offset audit is deliberately qualified: [RW_PORT_OFFSETS.md](RW_PORT_OFFSETS.md)
gives offsets and distinct file counts; [RW_PORT_OFFSETS.csv](RW_PORT_OFFSETS.csv)
records the expressions and carriers. Boundary-local evidence and spelling-only
grep candidates are separate. **This is not a complete interprocedural alias or
reaching-definition proof**; casts, wrapper fields, array subscripts and callback
arguments still require manual layout audits before those later slices land.
`src/types/auto_classes.h` mainly describes game wrappers, not RW-native objects.

## Architecture by structure

All numeric layout comparisons here are for 32-bit pointers. The CPU target has
compile-time assertions for its actual matrix and frame ABI. Native layouts for
20 corresponding types were also compiled with `linktest.flags()` / clang's
record-layout dump: [RW_PORT_LIBRW_LAYOUTS.txt](RW_PORT_LIBRW_LAYOUTS.txt).
Original-layout comparisons outside matrix/frame are source/layout reasoning plus
game-access evidence, not completed shims.
Costs are estimates in jobs, not measured implementation effort. **(b) is rejected**
for owning objects: it would fork an upstream project across its allocation,
plugin, streaming and backend code, and the requested build must use it as-is.

| Game structure | Evidence / important offsets | librw counterpart and mismatch | Choice and estimated cost |
|---|---|---|---|
| RwMatrix | basis rows 0/0x10/0x20, flags 0x0C, translation 0x30; counted accesses in offset audit | `rw::Matrix`, `rwbase.h`: 64 bytes, same rows/flags/translation/padding; CombineOp 0/1/2 matches | Native value type; no layout patch/rewrite. Low; slice 1 |
| RwV2d/RwV3d, RGBA/RGBAReal, sphere, bbox, quaternion | POD values and arrays passed to matrix, camera, lighting and intersection APIs | `V2d`, `V3d`, `RGBA`, `RGBAf`, `Sphere`, `BBox`, `Quat` in `rwbase.h`; quaternion x/y/z/w; bbox sup then inf | Native values after individual size/order assertions. Low; boundary signatures still matter |
| RwFrame | matrices +0x10/+0x50, object list +0x90, child/next/root +0x98/+0x9C/+0xA0; game creates frames then writes local matrices | `rw::Frame`, `rwobjects.h`: object list **before** matrices, matrices +0x18/+0x58, list +0x10; child/next/root still +0x98/+0x9C/+0xA0 | **(a)**, façade prefix 0xA4 + future plugin tail. Sidecar lookup and translated hierarchy/callbacks. Medium; CPU portion implemented, plugin portion slice 2/5 |
| RwObject / RwObjectHasFrame | flags bytes 0..3, parent/frame pointer +4; list links at +8 and sync callback following | `rw::Object`, `ObjectWithFrame`: compatible prefix, but parent pointer must target an ABI Frame, not native Frame | **(a)** pointer/list bridges; low-medium, shared by object slices |
| RpAtomic | geometry +0x18, render callback +0x48, other world/clump/pipeline accesses in audit | `rw::Atomic`: geometry +0x14, bounding spheres/list/pipeline arrangement differs; render callback happens to remain +0x48. No old interpolator/resource-entry layout | **(a)**. Synchronize geometry/spheres/flags, render callback and frame ownership. High, 1–2 jobs shared with geometry/clump |
| RpClump | frame pointer +4; callbacks/counts via boundary and plugin offsets; spelling-only +0x9C..0xB0 matches can be game wrappers | `rw::Clump`: object and atomic/light/camera lists, world field; old world/plugin/render-callback tails are not interchangeable | **(a)**, list and clone/stream/callback translation. Medium, 1 job with atomic |
| RpGeometry / RpMorphTarget / mesh header / mesh | +0x20 material-list access, triangle/vertex counts, +0x30/+0x34/+0x5C and material/morph arrays; audit separates uncertain carriers | `rw::Geometry`: +0x20 is **triangles**, +0x24 colors, +0x28 texCoords; matList is after morphTargets. Refcount width/order and instance data differ. Mesh/triangle payloads need their own assertions | **(a)**; façade arrays can share verified POD storage, never reinterpret the header. High, ~2 jobs including native data |
| RpMaterial | texture/color/surface properties/plugin access; numerous name-only offsets belong to game wrappers | `rw::Material`: texture at 0, color at 4, surfaceProps at 8, pipeline at 0x14, 32-bit refcount at 0x18; original order differs | **(a)**. Small header mirror, texture handle, plugin tail. Medium, with material/user-data slice |
| RwTexture | raster pointer +0, name/filter/refcount prefix; counted +0x54 accesses; custom extensions may use registry-derived offsets | `rw::Texture`: RW-like raster/dictionary/list/name/mask/filter/refcount prefix, **extra global-list link after refCount** before plugins | **(a)** for owning/plugin object, share native raster sidecar. Low-medium, texture slice; could narrow to native-prefix (c) only after extension audit |
| RwTexDictionary | list/dictionary relationships, global enumeration, streaming | `rw::TexDictionary`: compatible-looking object/list prefix but native Texture pointers and extra global links | **(a)** list/handle/callback conversion. Low-medium with textures |
| RwRaster | width +0x0C, height +0x10, depth +0x14 confirmed in `007281f0.cpp`; flags +0x23 candidates | `rw::Raster`: platform/type/flags/privateFlags/format are full int fields **before** dimensions (width +0x14), parent at end; original compact flags/offsets and pointer order differ | **(a)**. Lock/unlock pixels, subraster parent/rectangle, D3D native extensions. High, raster slice + driver slice |
| RwImage | width +4, height +8, stride +0x10, pixels +0x14 (capture path) | `rw::Image`: inserted `bpp` at +0x10, stride +0x14, pixels +0x18 | **(c)** is economical for the few confirmed local consumers: typed helper accessors, not mass constant substitution. Low, image slice; use (a) if callbacks expose old layout elsewhere |
| RwCamera | frame parent +4; framebuffer +0x60/zbuffer +0x64 candidates, projection and view/frustum fields | `rw::Camera`: viewWindow/viewOffset/clip fields **before** viewMatrix (+0x3C); buffers after frustum data and additional native devView/devProj matrices | **(a)**. Sync framebuffer, near/far/projection/view, frame and device hooks. High, camera slice |
| RpLight | compatible-looking flags/frame/radius/color prefix; world/clump extensions accessed indirectly | `rw::Light`: core prefix similar but list/plugin layout and originalSync/world/clump tails differ | **(a)**, core property mirror plus frame/world lists. Medium, light/object slice |
| RpWorld / RpWorldSector / BSP sectors | direct sector/material/mesh accesses, sector callbacks, WorldForAllWorldSectors and world stream APIs | `rw::World`: simplified Object + light lists + clump list, **no sector/BSP/material-world implementation** in this checkout | **(a)**, independently implement the game-facing world/sector boundary; convert sector geometry to librw atomics/geometry. Very high, ≥2 jobs; not just a layout shim |
| RtAnimAnimation / interpolation scheme / keyframes | keyframes +0x10 confirmed in two files | `rw::Animation`: interpInfo/numFrames/flags/duration/keyframes/customData prefix matches the observed +0x10; scheme callbacks and streamed representation still need ABI checks | Native animation header + **(c)** typed callback/accessor bridges if full prefix is asserted. Medium, animation slice; compressed keyframes are absent |
| RtAnimInterpolator | currentAnim/time prefix; hierarchy embeds/addresses interpolation state and game installs callbacks | `rw::AnimInterpolator`: currentAnim/time/nextFrame/size/node count + cached callbacks, explicitly TODO parent/sub and other callbacks; interpolation frames follow the reduced header | **(a)**, missing copy/blend/set-time/subinterpolator semantics independently supplied. High, animation slice |
| RpHAnimHierarchy / node info / frame extension | matrices/get-ID via APIs; hierarchy +0x20 and other accesses in audit | `rw::HAnimHierarchy`: flags/nodes/matrix/nodeInfo/frame pointers, **pointer to** interpolator at +0x1C, not original embedded interpolator/tail. Subhierarchy largely unimplemented | **(a)**, node/frame/matrix ownership and embedded-interpolator façade. High, 1–2 animation jobs |
| RpSkin / split data | skin obtained by plugin getter; direct offset proof is weak (name-only +0xF0 may be a wrapper) | `rw::Skin`: int bone/weight counts, pointers, split/RLE storage; backend platformData differs. Original full layout **not established** here | **(a)** until direct-access audit proves a narrower solution; share matrix/weights/index buffers with checked strides. High, skin/native-data jobs |
| RwStream / chunk info | IO calls, callbacks and ownership, memory/file streams | `rw::Stream` is a virtual C++ class; original C stream struct/type union is not compatible. `ChunkHeaderInfo` values can be copied | **(a)** opaque façade + StreamFile/StreamMemory sidecars and exact return/close policy. Medium, slices 2/6 |
| plugin registry / user-data / MatFX / particle/PTank / 2D/team/import data | game takes registry offsets and directly walks plugin payloads; see candidate offsets | `PluginList`, `UserDataArray`, `MatFX` provide some counterparts; particle/PTank/2D/team/import packages largely absent | **(a)** ABI registry and payloads; stream/construct/copy/destroy callbacks always see game pointers and game offsets. Costs vary from small user-data getters to multiple independent toolkit jobs |
| RW engine globals/device dispatch | `CSF_GLOBAL(..., 0x008B68C0)` in **44 files**, not counted as function slots | `rw::Engine` has a different layout. Game reads/writes render-state/device dispatch tables directly (+0x20/+0x24 seen) | **(a)** engine-global façade and thunk table, never publish `rw::engine` as this pointer. High; lifecycle/device slices |

### Mirroring discipline

Façade pointers are stable, own their ABI plugin tail and remain the only pointers
published to the game. Native sidecars are private. Inward conversion imports
mutable game fields/arrays; outward conversion exports results and translated
parent/list/object pointers. Constructor/copy/destructor/stream/render callbacks
must get the façade and **ABI offsets**, not native plugin offsets. Clone and
destruction need a handle map, rollback on allocation failure and identity tests.
Do not mirror an entire native object by `memcpy`; only compatible POD payloads.

## Names and unnamed slots: evidence, not renames

No files under db/ or export/ were changed. The existing database is not a promise
that a toolkit name is right (PLAN already records held-back naming conflicts).

| Address | Finding | Confidence / evidence |
|---|---|---|
| 0x007E4970 | unnamed no-op; exact toolkit API unknown | **High** behavior: one-byte RET, void return, no reads/writes. `006d6760.cpp` passes a child count. Do not invent a public name |
| 0x007F1810 | unnamed dictionary/schema registration for type IDs 4, 9, 2 | **High** code shape, **low** formal identity. Calls one registration helper with three static descriptors and tests success. `0070f2d0.cpp` is audio-facing; do not assume a Graphics plugin |
| 0x007FEDC0 | unnamed membership-gated team/player output query | **Medium** identity: word ID at player +0x20 selects bit in owner +0x68 array; calls output helper; `006ff9a0.cpp` consumes six dwords as position/heading. Exact `RpTeam...` name unknown |
| 0x005915C0 | database `RpClumpRegisterPluginStream` actually registers against the **material** registry | **High**: same registry operand as named MaterialRegisterPlugin at 0x00591590 |
| 0x0058F0E0 | database `RpClumpGetWorld` is suspect; likely light→clump getter | **Medium**: plugin-offset getter, light destruction callers use returned object's frame/list; candidate replacement `Light::clump` |
| 0x005802A0 | database `RpMultiTextureSetEffect` is suspect; emitter particle-class/flags setter | **High** shape, **medium** formal name: emitter +0x18 class ownership, +0x14 flags, old class release/new class refcount. Particle caller, not a texture operation |
| 0x00626AC0 | database `Skin_DispatchType4Handler` does not describe its body | **High** shape: stores one argument in an engine-adjacent global and returns it; formal API unresolved |

For all unnamed live slots the per-address table also records this evidence.
Additional newly observed slots must not be named by nearest-neighbor guesses.
The resumed compiled inventory adds five particle callbacks (0x0057CFE0,
0x0057DBA0, 0x0057EDA0, 0x0057EF90, 0x0057F050), one atomic pipeline callback
(0x0058E680), and BMP/PNG read/write callbacks (0x007C5680, 0x007C5CA0,
0x007C8D30, 0x007C9050). Evidence and confidence are in their rows; these remain
named "unnamed" because db/ is untouched. The image callbacks have concrete
librw counterparts; the particle and Rx callbacks do not.

Two named slots from the interrupted inventory are not live in the fresh
baseline: RwEngineGetVideoModeInfo (0x00626BE0; then 3 caller files), mapped to
`engine.cpp: rw::Engine::getVideoModeInfo` (differs: copy video-mode output ABI),
and RwCameraEndUpdate (0x00628E40; then 7 caller files), mapped to
`camera.cpp: rw::Camera::endUpdate` (differs: facade camera/device ownership).
They remain planned in slices 2 and 4 if restored, but are not counted in the
400 live-slot totals. Their historic references are not current link evidence.

## D3D9 and direct device use

The inventory's **D3D9 driver** rows identify every named driver-specific slot,
including shaders/declarations/streams/indices/draw, transform/state/texture,
caps/device getters, video/multisampling/refresh/fullscreen policy, DDS loading,
vertex-shader world/composed-matrix helpers, and all-in-one pipeline callback.
The region also contains `D3DXAssembleShader` and `D3DXCompileShader`: these are
**D3DX boundaries**, not functions to implement from RW code or librw.

`grep IDirect3DDevice9 src/auto` finds **zero** files because game code uses erased
pointer types and raw vtable calls. Following the cached device at renderer +0x5C
and grepping those calls confirms:

| Game source | Device / COM method | Cutover impact |
|---|---|---|
| `006dc290.cpp` | caches RwD3D9GetCurrentD3DDevice; writes caps +0xEC bit 0x80 | Return librw's real D3D9 device, preserve mutable caps policy |
| `006dc2c0.cpp` | device +0x5C CreateTexture, +0x84 GetFrontBufferData; texture +0x48 GetSurfaceLevel; surface +0x34 LockRect/+0x38 UnlockRect, +8 Release | Screenshot path needs real D3D9 resources and BGRA→RGBA conversion, not an emulated vtable |
| `006dc560.cpp` | device +0x54 SetGammaRamp | Real device call; the game binding uses register-context adapters (not a new COM ABI) |
| `006dc750.cpp` | device +0x1C GetDeviceCaps; +0xC4/+0xCC caps members consumed | Same device/caps contract |
| `006e4e90.cpp`, `006f61b0.cpp` | device +0xE4 SetRenderState, state **195** (depth bias), then reset to zero | Direct writes bypass caches. Both calls reset to zero, but a stale cached value can still suppress later writes; bridge/cache invalidation required |
| `006f61b0.cpp` and many renderer files | original engine dispatch +0x20/+0x24 render-state functions | Not a fn_slot reference! Provide compatible engine thunk table; similarly audit immediate-mode dispatch |

The grep appendix contains additional “device” matches. They are **not all D3D**:
`006dc9b0.cpp` calls a game render object via fastcall; audio uses other COM devices
in `00527630.cpp`, `00527dc0.cpp`, `00712f80.cpp`. Do not map those vtable indices
against IDirect3DDevice9 just because the local is named `device`.

librw D3D9 sources (`d3ddevice.cpp`, `d3d9*.cpp`, `d3dimmed.cpp`, `d3drender.cpp`)
cover device lifecycle, modes/multisampling, native raster locking, native texture
streaming, shader/declaration/index/stream state, immediate drawing and default/
skin/MatFX pipelines. They do **not** supply the original Rx graph API, sector
renderer, PTank/PrtStd/PrtAdv, team/toon/2D toolkits, compressed animation or DDS
reader. A direct COM call can supply some missing low-level operations; it does
not reproduce RW cache or callback semantics automatically. D3D9 was inspected,
**not built or run** in this job; first slice always uses NULL.

## Dependency-ordered jobs and offline acceptance

The generated table allocates **every live slot once** to a numbered slice.
Counts are printed beside it; allocations are reproducible in `tools/rw_survey.py`.
No “success” stub is an implementation of a missing toolkit.

| Slice | Scope | Dependencies and acceptance without the game |
|---|---|---|
| 1 | CPU vectors/math, all matrix slots, frame hierarchy/list/matrix slots, dirty sync, freelist purge (36) | Unchanged librw NULL compile; Win32 compile-time ABI assertions; executable references every slot; native unit tests for transforms, inverse, zero normalize, orthonormalization, supplied cosine/sine rotation, parent/root/LTM, early callback stop, sync-once and destruction |
| 2 | Explicit engine lifecycle + global dispatch façade, registry/file/stream primitives, allocator policy, random and remaining core helpers | 1. Replace lazy NULL bootstrap; registry ordering, callback identity, allocation-failure cleanup, memory/file short-read/seek/close tests; compile+link all slots. Keep graphics and audio-facing region misclassifications separate |
| 3 | Raster/texture/dictionary/image façade and image-access rewrites | 2. NULL image/texture dictionary ownership, refcounts, pixel stride, mask/filter/name tests; palette/mipmap/subraster mock tests. Backend-native fixtures go with 7 |
| 4 | Atomic/clump/geometry, camera and lights/material property boundaries | 1–3. Construct/clone/destroy/list iteration tests, geometry counts and material ownership, matrices/sphere/frustum tests, callback façade identities. Win32 link probe includes all allocated slots |
| 5 | User data, MatFX/plugin payloads, HAnim/skin/animation and remaining optional-plugin policies | 2/4. Hand-built keyframe/hierarchy/skin fixtures; attach/detach/update/copy/blend/set-time and plugin streams. Missing compressed schemes require independent format work, not opaque forwarding |
| 6 | Asset serialization, world/sector/BSP façade, collision/bounds | 2–5. Synthetic chunk/unknown-plugin/EOF fixtures, serialized-size round trips, static-sector→atomic conversion, known line/sphere/triangle hits. Validate version/build handling separately from “can parse” |
| 7 | D3D9 driver/device and immediate mode; Rx boundary policy and misregion imports | 2–6. D3D9 compile+link with SDK; fake device records calls/state invalidation and vertex conversion; no hardware test claimed. Define supported pipeline callback contract, do not reimplement Rx graph internals |
| 8 | PrtStd/PrtAdv/PTank independent game-facing boundary | 3–7. Property-offset/class/refcount/batch ownership and deterministic CPU particle fixtures, PTank lock/unlock stride tests. Output goes through librw atomic/immediate backend |
| 9 | First half of remaining toolkit slots, by address: dictionaries/2D primitives and ancillary handles | 2–7. Ownership/schema/string/transform/child traversal fixtures. Names must be confirmed against callers before implementation; table gives exact address allocation |
| 10 | Second half of toolkit slots: team/player/import/water/scene contracts | 4–9. Selection/membership/transform/material-slot/scene hierarchy fixtures; CPU generated geometry tests. Address ordering is a workload partition, not proof all formal toolkit identities are right |

Slices 5, 6 and 8 are particularly high-risk; split a job further if missing-format
work cannot fit one job, even if that means fewer than 20 functions. Acceptance is
compile **and real library link of all allocated slots**, not just linktest's
unresolved-slot reduction. Null/device-mock tests prove only their tested CPU
contract. Full standalone/game rendering remains outside offline acceptance.

## First slice implementation, limitations and build

`src/boundary/rw/` defines 36 address-keyed standalone slots via HOOK, with
original cdecl / zero-argument stdcall entry conventions. It is excluded from
`csf_hook` exactly like CRT boundaries. `CSF_RW_CPU_SLICE=ON` adds the external
checkout with `add_subdirectory`, a static CPU boundary and a standalone link
probe; the default hooked target never links librw. librw sources are not copied
or modified. The build removes upstream's Unix `m` dependency for clang-cl and
disables narrowing diagnostics for upstream's unsigned `{-1}` Xbox tables.

Matrix memory uses librw's allocator and initializer separately (its create API
does not guard allocation failure); Frame façades use CRT allocation and keep
native Frame sidecars. No free-list pool exists, so Purge returns zero reclaimed
bytes. `RwFreeListForAllFreeLists` and resource-arena policy are **not installed**.
Facade base size is 0xA4; **custom Frame plugin tails/callbacks are not implemented
yet**, so this is not a complete usable graphics runtime. Matrix create/rotate/
transform use librw. OrthoNormalize and RotateOneMinusCosineSine compose librw
vector/matrix primitives using its mathematical representation because it lacks
those exact public APIs; neither was translated from original-library code.

The lazy lifecycle starts a NULL engine for CPU frames (Init/Open/Start, including
module construction). Slice 2 must remove this bootstrap before mapping explicit
game lifecycle and before any D3D9 cutover. No CPU boundary should silently open a
real device. Sync callbacks currently use façade object lists; later owning-object
slots must bridge those lists and native sidecars. Parent destruction explicitly
detaches children first to avoid librw leaving dangling hierarchy roots.

Vector lengths use librw sqrtf; scalar sqrt/reciprocal sqrt use std::sqrt,
rather than the original lookup-table approximations.
Zero normalization returns zero vector/length instead of invoking RW error state.
There is no original error registry yet. Invert/singular and degenerate
orthonormalization, custom allocators, plugin tail offsets, reentrant hierarchy
mutation/destruction inside callbacks, native streaming and rendering are open.
Creation failure is tested by injecting a failing native allocator, but arbitrary
custom allocator lifetime/alignment is not established. Full-map matrix import/
state export is currently conservative and its large-scene performance is unmeasured.
The CPU facade is single-threaded, matching the intended graphics entry path;
thread safety has not been established for arbitrary concurrent calls.

Reproduce Win32 build/link (not execution):

```sh
cmake -S . -B build -DCSF_RW_CPU_SLICE=ON
cmake --build build --target csf_rw_cpu_link -j 8
.venv/bin/python tools/linktest.py -j 8
cmake --build build --target csf_hook -j 8
```

Reproduce native CPU tests (not a 32-bit ABI execution test):

```sh
cmake -S . -B /tmp/csf-rw-test -G Ninja -DCSF_RW_CPU_SLICE=ON \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cmake --build /tmp/csf-rw-test --target csf_rw_cpu_link -j 8
/tmp/csf-rw-test/src/boundary/rw/csf_rw_cpu_link
```

## Inventory and audit appendices

### Offline checks rerun on 2026-10-14

The CPU source was temporarily excluded for the baseline and restored before
the after run. `.venv/bin/python tools/linktest.py -j 8` reports:

```text
before: compiled 14363/14363 files standalone; 14515 functions implemented
unresolved functions: game 99, netz 222, not-a-function 7, png 1, rw 400, rwa 40
after:  compiled 14364/14364 files standalone; 14551 functions implemented
unresolved functions: game 99, netz 222, not-a-function 7, png 1, rw 364, rwa 40
```

Both runs have no duplicate slots or standalone compile failures; after has no
raw image-address hits. Set comparison verified that the 36 removed RW slots
are exactly the 36 HOOK definitions and that all 400 baseline slots appear
exactly once in the inventory and slice allocation (all slices have 20–60 slots).
Logs and snapshots are in `work/rw-port-check/` (scratch, not committed).

`tools/rw_survey.py work/rw-port-check/before.json --layouts --check-cpu-abi`
compiled 20 Win32 native record layouts and checked all 36 conventions/argument
counts against export/abi.json. This does not check behavioral or return-type
equivalence. The Win32 `csf_rw_cpu_link` executable linked against unchanged
librw, and `csf_hook` built (PE32 x86 DLL). The Windows binary was not executed.
Native CPU execution passed all 36 slot references and vector/matrix/frame
fixtures, including object type and a four-level hierarchy after fixing the
facade initialization and upstream nested-subtree-root behavior in the adapter.
Facade object callbacks also follow the native dirty-root/child preorder rather
than hash-map iteration; a three-object ordering fixture verifies this.
The same tests passed with AddressSanitizer + UndefinedBehaviorSanitizer;
leak detection was disabled because engine teardown is not part of this slice.
No game, device, D3D9-backend or original-library differential test is claimed.

The following generated appendices give fit totals, exact slice counts and every
pre-slice slot, followed by offset/file-count evidence. Machine-readable slots:
[RW_PORT_SLOTS.json](RW_PORT_SLOTS.json). Regenerate from a **pre-slice** linktest
snapshot; do not overwrite the original inventory with the reduced unresolved set.

# RW slot inventory

Generated with `tools/rw_survey.py` from the pre-slice linktest snapshot. References are distinct compiled caller files, not call-site counts. Paths below are relative to `work/tools/librw/src/`. Read [RW_PORT.md](RW_PORT.md) for qualifications and architecture.

## Totals

| Subsystem | Same semantics | Differs | Missing | Total |
|---|---:|---:|---:|---:|
| D3D9 driver | 0 | 10 | 16 | 26 |
| camera | 0 | 12 | 1 | 13 |
| collision/bounds | 0 | 3 | 9 | 12 |
| core/frame/matrix | 14 | 22 | 2 | 38 |
| engine | 0 | 10 | 2 | 12 |
| file system | 0 | 1 | 1 | 2 |
| geometry/atomic/clump | 0 | 12 | 1 | 13 |
| hanim/skin/animation | 0 | 18 | 11 | 29 |
| immediate mode | 0 | 4 | 0 | 4 |
| light | 0 | 13 | 0 | 13 |
| material/matfx | 0 | 8 | 1 | 9 |
| memory/freelist | 0 | 0 | 3 | 3 |
| other boundary/pipeline | 0 | 0 | 17 | 17 |
| other plugins | 0 | 0 | 7 | 7 |
| particles/ptank | 0 | 0 | 33 | 33 |
| plugin registry | 0 | 6 | 1 | 7 |
| random | 0 | 0 | 2 | 2 |
| raster/texture/image | 0 | 44 | 6 | 50 |
| stream/chunk I/O | 0 | 23 | 6 | 29 |
| toolkit/team | 0 | 0 | 1 | 1 |
| toolkit/unknown | 0 | 0 | 2 | 2 |
| toolkits | 0 | 0 | 48 | 48 |
| user data | 3 | 9 | 1 | 13 |
| world/sector | 0 | 11 | 6 | 17 |
| **Total** | **17** | **206** | **177** | **400** |

## Slice allocation

Each slot is allocated exactly once. Slice 1 is implemented; 2–10 are a plan, not stubs.

| Slice | Slots |
|---|---:|
| 1 | 36 |
| 2 | 39 |
| 3 | 50 |
| 4 | 41 |
| 5 | 56 |
| 6 | 47 |
| 7 | 47 |
| 8 | 33 |
| 9 | 26 |
| 10 | 25 |

## All slots

Candidate mappings are source-level correspondences, NOT completed adapters or proven behavioral equivalence. `none` includes absent exact APIs and missing toolkits. Database names (especially toolkit names) remain provisional; see corrections in RW_PORT.md.

| Address | Name | Refs | Subsystem | Slice | librw file / function or field | Fit / adaptation |
|---|---|---:|---|---:|---|---|
| `0x566520` | RpAnisotGetMaxSupportedMaxAnisotropy | 2 | raster/texture/image | 3 | texture.cpp: rw::getMaxSupportedMaxAnisotropy | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x566530` | RpAnisotTextureSetMaxAnisotropy | 1 | raster/texture/image | 3 | texture.cpp: rw::Texture::setMaxAnisotropy | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x56B9B0` | RpWorldForAllWorldSectorIntersections | 1 | collision/bounds | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x56BAF0` | RpCollisionWorldForAllIntersections | 5 | collision/bounds | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x56BBD0` | RpWorldForAllAtomicIntersections | 1 | collision/bounds | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x56BDF0` | RpAtomicForAllIntersections | 4 | collision/bounds | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x56CD30` | RpHAnimHierarchyDestroy | 3 | hanim/skin/animation | 5 | hanim.cpp: rw::HAnimHierarchy::destroy | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x56CDB0` | RpHAnimHierarchyCreateSubHierarchy | 1 | hanim/skin/animation | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x56CED0` | RpHAnimHierarchyCreateFromHierarchy | 1 | hanim/skin/animation | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x56CF50` | RpHAnimHierarchyDetach | 2 | hanim/skin/animation | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x56CF80` | RpHAnimHierarchyGetMatrixArray | 8 | hanim/skin/animation | 5 | rwplugins.h: rw::HAnimHierarchy::matrices [field] | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x56CF90` | RpHAnimFrameGetHierarchy | 2 | hanim/skin/animation | 5 | hanim.cpp: rw::HAnimHierarchy::get | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x56CFA0` | RpHAnimFrameGetID | 3 | hanim/skin/animation | 5 | rwplugins.h: rw::HAnimData::id [field] | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x56D3F0` | RpHAnimHierarchyUpdateMatrices | 8 | hanim/skin/animation | 5 | hanim.cpp: rw::HAnimHierarchy::updateMatrices | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x56DB50` | RpHAnimHierarchyAttach | 2 | hanim/skin/animation | 5 | hanim.cpp: rw::HAnimHierarchy::attach | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x56EFF0` | RpLtMapGetRenderStyle | 1 | other plugins | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x56F2B0` | RpLtMapSetRenderStyle | 1 | other plugins | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x56F380` | RpLtMapWorldLightMapsQuery | 1 | other plugins | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x571270` | RpMatFXAtomicEnableEffects | 2 | material/matfx | 5 | matfx.cpp: rw::MatFX::enableEffects | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x5712A0` | RpMatFXAtomicQueryEffects | 4 | material/matfx | 5 | matfx.cpp: rw::MatFX::getEffects(Atomic*) | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x5712B0` | RpMatFXMaterialGetEffects | 1 | material/matfx | 5 | matfx.cpp: rw::MatFX::getEffects(Material*) | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x5715D0` | RpMatFXMaterialSetEnvMapFrame | 1 | material/matfx | 5 | matfx.cpp: rw::MatFX::setEnvFrame | **differs**: Obtain MatFX extension then setEnvFrame; translate Frame to sidecar and return original Material pointer. |
| `0x571610` | RpMatFXMaterialSetEnvMapFrameBufferAlpha | 1 | material/matfx | 5 | matfx.cpp: rw::MatFX::setEnvFBAlpha | **differs**: Obtain MatFX extension then setEnvFBAlpha; native method returns void, original returns Material handle. |
| `0x571920` | RpMatFXMaterialSetUVTransformMatrices | 5 | material/matfx | 5 | matfx.cpp: rw::MatFX::setUVTransformMatrices | **differs**: Extension method versus Material API; matrices may share the compatible POD layout, but extension offsets/ownership do not. |
| `0x571A80` | RpMatFXMaterialSetEffects | 2 | material/matfx | 5 | matfx.cpp: rw::MatFX::setEffects | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x5774B0` | RpMorphAtomicAddTime | 1 | other plugins | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5784E0` | RpPrtAdvEmitterClassCreate | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5785B0` | RpPrtAdvEClassStdCreate | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57BBD0` | RpPrtAdvEmtSphereEmitCB | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57C220` | RpPrtStdAtomicIsEmitter | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57C290` | RpPrtStdAtomicGetEmitter | 11 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57C2D0` | RpPrtStdAtomicUpdate | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57C5D0` | RpPrtStdAtomicCreate | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57C770` | RpPrtStdPropTabDestroy | 3 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57C8B0` | RpPrtStdPropTabGetPropOffset | 14 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57C9A0` | RpPrtStdPropTabCreate | 3 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57CD60` | RpPrtStdEmitterClassDestroy | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57CE10` | RpPrtStdEmitterClassSetCallBack | 2 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57CE90` | RpPrtStdEmitterClassSetPropTab | 3 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57CED0` | RpPrtStdEmitterClassGetPropTab | 3 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57CFE0` | unnamed | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. High confidence particle batch update callback: caller installs in particle class; dt argument, property offsets and PTank arrays. Formal PrtStd callback name uncertain. |
| `0x57DBA0` | unnamed | 2 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. High confidence emitter emission callback: class callback slot 0, emitter property tables, randomized particle initialization and PTank output. Formal PrtStd name uncertain. |
| `0x57EDA0` | unnamed | 2 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. High confidence emitter render callback shape: tests flag 0x200, property ID 1, calls RpAtomicRender on the PTank atomic; formal name uncertain. |
| `0x57EF90` | unnamed | 2 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. High confidence emitter begin-update callback shape: caches atomic frame LTM, adds dt to emitter time, updates particle rotation using 2*pi wrap; installed in callback slot 1. Formal name uncertain. |
| `0x57F050` | unnamed | 2 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. High confidence emitter end-update/scheduling callback shape: installed in slot 2; LCG constants 196314165/907633515, next emission time from bias/spread. Formal name uncertain. |
| `0x57F970` | RpPrtStdPClassStdCreate | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57FA90` | RpPrtStdEClassStdCreate | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57FC60` | RpPrtStdPClassDestroy | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x57FCF0` | RpPrtStdPClassSetCallBack | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x580160` | RpPrtStdEmitterForAllParticleBatch | 3 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x580230` | RpPrtStdEmitterNewParticleBatch | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5802A0` | RpMultiTextureSetEffect | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x581250` | RpPrtStdEmitterSetPTank | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5821A0` | RpPTankAtomicDestroy | 3 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5821B0` | RpPTankAtomicGetDataFormat | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5821D0` | RpPTankAtomicLock | 13 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5823E0` | RpPTankAtomicUnlock | 13 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x582400` | RpPTankAtomicCreate | 4 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5860C0` | RpPVSSetViewPosition | 1 | other plugins | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5861F0` | RpPVSWorldUnhook | 1 | other plugins | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5863D0` | RpPVSWorldGetProgressCallBack | 2 | other plugins | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5865A0` | RpPrtStdWorldSetRenderCallBack | 1 | particles/ptank | 8 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5867D0` | RtRandom | 6 | random | 2 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x586860` | RtRandomSeed | 1 | random | 2 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x587020` | RpSkinAtomicSetHAnimHierarchy | 1 | hanim/skin/animation | 5 | rwplugins.h: rw::Skin::setHierarchy | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x587050` | RpSkinGeometryGetSkin | 3 | hanim/skin/animation | 5 | rwplugins.h: rw::Skin::get | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x5870B0` | RpUserDataArrayGetName | 5 | user data | 5 | rwuserdata.h: rw::UserDataArray::name [field] | **same semantics**: Equivalent value/name retrieval; array handle must be the mapped extension. |
| `0x5870D0` | RwRasterGetWidth | 3 | raster/texture/image | 3 | rwobjects.h: rw::Raster::width [field] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x5870E0` | RpSkinIsSplit | 1 | hanim/skin/animation | 5 | rwplugins.h: rw::Skin::numMeshes [field; inspect split data] | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x5870F0` | RpSkinAtomicGetType | 1 | hanim/skin/animation | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x58BCC0` | RpGeometryRemoveUserDataArray | 1 | user data | 5 | rwuserdata.h: rw::UserDataArray::geometryRemove | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58BCE0` | RpGeometryGetUserDataArrayCount | 2 | user data | 5 | rwuserdata.h: rw::UserDataArray::geometryGetCount | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58BD10` | RpGeometryGetUserDataArray | 2 | user data | 5 | rwuserdata.h: rw::UserDataArray::geometryGet | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58BD40` | RwFrameRemoveUserDataArray | 1 | user data | 5 | rwuserdata.h: rw::UserDataArray::frameRemove | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58BD60` | RpMaterialGetUserDataArrayCount | 2 | user data | 5 | rwuserdata.h: rw::UserDataArray::materialGetCount | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58BD90` | RpMaterialGetUserDataArray | 2 | user data | 5 | rwuserdata.h: rw::UserDataArray::materialGet | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58BDC0` | RwTextureRemoveUserDataArray | 1 | user data | 5 | rwuserdata.h: rw::UserDataArray::textureRemove | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58BDE0` | RwTextureGetUserDataArrayCount | 1 | user data | 5 | rwuserdata.h: rw::UserDataArray::textureGetCount | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58BE10` | RwTextureGetUserDataArray | 1 | user data | 5 | rwuserdata.h: rw::UserDataArray::textureGet | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58BE40` | RpUserDataArrayGetReal | 1 | user data | 5 | rwuserdata.h: rw::UserDataArray::getFloat | **same semantics**: Equivalent value/name retrieval; array handle must be the mapped extension. |
| `0x58BE50` | RpUserDataArrayGetInt | 5 | user data | 5 | rwuserdata.h: rw::UserDataArray::getInt | **same semantics**: Equivalent value/name retrieval; array handle must be the mapped extension. |
| `0x58CBB0` | RpWorldRender | 2 | world/sector | 6 | world.cpp: rw::World::render | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58CBD0` | RpWorldDestroy | 2 | world/sector | 6 | world.cpp: rw::World::destroy | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58CC60` | RpWorldSetSectorRenderCallBack | 2 | world/sector | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x58CC80` | RpWorldGetSectorRenderCallBack | 2 | world/sector | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x58CC90` | RpAtomicSetPipeline | 2 | geometry/atomic/clump | 4 | rwobjects.h: rw::Atomic::pipeline [field] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58CCB0` | RpWorldCreate | 3 | world/sector | 6 | world.cpp: rw::World::create | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58CEF0` | RpWorldForAllClumps | 6 | world/sector | 6 | rwobjects.h: rw::World::clumps [list iteration] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58CF30` | RpWorldForAllMaterials | 4 | material/matfx | 4 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x58CF80` | RpWorldForAllLights | 1 | light | 4 | rwobjects.h: rw::World::localLights / globalLights [list iteration] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58CFF0` | RpWorldForAllWorldSectors | 9 | world/sector | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x58D1A0` | RpWorldSectorGetWorld | 5 | world/sector | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x58DD90` | RpWorldAddCamera | 3 | camera | 4 | world.cpp: rw::World::addCamera | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58DDC0` | RpWorldRemoveCamera | 8 | camera | 4 | world.cpp: rw::World::removeCamera | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58DDF0` | RwCameraGetWorld | 7 | camera | 4 | rwobjects.h: rw::Camera::world [field] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58DE00` | RpWorldRemoveAtomic | 1 | world/sector | 6 | world.cpp: rw::World::removeAtomic | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58DE40` | RpAtomicGetWorld | 1 | world/sector | 6 | rwobjects.h: rw::Atomic::world [field] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58DE50` | RpWorldSectorForAllAtomics | 1 | world/sector | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x58DEF0` | RwCameraForAllWorldSectors | 1 | camera | 4 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x58DF40` | S_006fdfe0_FindWorldFromClump | 10 | world/sector | 6 | rwobjects.h: rw::Clump::world [field] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58DF50` | RpWorldAddLight | 3 | light | 4 | world.cpp: rw::World::addLight | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58DFA0` | RpWorldRemoveLight | 6 | light | 4 | world.cpp: rw::World::removeLight | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58DFD0` | RpLightGetWorld | 8 | light | 4 | rwobjects.h: rw::Light::world [field] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58E280` | RpWorldAddClump | 4 | world/sector | 6 | world.cpp: rw::World::addClump | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58E310` | RpWorldRemoveClump | 9 | world/sector | 6 | world.cpp: rw::World::removeClump | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58E680` | unnamed | 3 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. High confidence atomic pipeline render callback shape: chooses atomic +0x70 pipeline or engine default, invokes RxPipelineExecute, returns atomic/null; medium confidence RpAtomicDefaultRenderCallBack identity. No equivalent Rx graph in librw. |
| `0x58EB40` | _rpAtomicResyncInterpolatedSphere | 6 | geometry/atomic/clump | 4 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x58EC60` | RpAtomicGetWorldBoundingSphere | 7 | world/sector | 6 | clump.cpp: rw::Atomic::getWorldBoundingSphere | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58EEB0` | RpClumpRender | 1 | geometry/atomic/clump | 4 | clump.cpp: rw::Clump::render | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58EF00` | RpClumpForAllAtomics | 26 | geometry/atomic/clump | 4 | rwobjects.h: rw::Clump::atomics [list iteration] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58EF90` | RpClumpForAllLights | 2 | light | 4 | rwobjects.h: rw::Clump::lights [list iteration] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58EFE0` | RpAtomicSetGeometry | 3 | geometry/atomic/clump | 4 | clump.cpp: rw::Atomic::setGeometry | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58F060` | RpAtomicDestroy | 4 | geometry/atomic/clump | 4 | clump.cpp: rw::Atomic::destroy | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58F0E0` | RpClumpGetWorld | 1 | world/sector | 6 | rwobjects.h: rw::Light::clump [database name is suspect; light callers] | **differs**: Existing name likely wrong: light callers use result as Clump. Native Light::clump is a field, so confirm package/plugin identity before installing. |
| `0x58F1D0` | RpClumpRemoveLight | 3 | light | 4 | clump.cpp: rw::Clump::removeLight | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58F210` | RpAtomicRegisterPlugin | 1 | plugin registry | 2 | rwplg.h: rw::Atomic::registerPlugin [PLUGINBASE] | **differs**: librw plugin sizes/offsets and callback object pointers are different; maintain separate ABI registry and bridge callbacks/streams. |
| `0x58F270` | RpAtomicRegisterPluginStream | 1 | stream/chunk I/O | 6 | rwplg.h: rw::Atomic::registerPluginStream [PLUGINBASE] | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x58F300` | RpAtomicSetFrame | 9 | geometry/atomic/clump | 4 | clump.cpp: rw::Atomic::setFrame | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58F560` | RpClumpGetNumAtomics | 2 | geometry/atomic/clump | 4 | clump.cpp: rw::Clump::countAtomics | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58F590` | RpClumpGetNumLights | 1 | light | 4 | clump.cpp: rw::Clump::countLights | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58F5C0` | RpAtomicCreate | 1 | geometry/atomic/clump | 4 | clump.cpp: rw::Atomic::create | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58F740` | RpClumpDestroy | 5 | geometry/atomic/clump | 4 | clump.cpp: rw::Clump::destroy | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58FC60` | RpClumpClone | 5 | geometry/atomic/clump | 4 | clump.cpp: rw::Clump::clone | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x58FD10` | RpClumpStreamGetSize | 3 | stream/chunk I/O | 6 | clump.cpp: rw::Clump::streamGetSize | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x58FDF0` | RpClumpStreamWrite | 2 | stream/chunk I/O | 6 | clump.cpp: rw::Clump::streamWrite | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x58FFC0` | RpClumpStreamRead | 4 | stream/chunk I/O | 6 | clump.cpp: rw::Clump::streamRead | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x5903F0` | RpWorldSectorRender | 1 | world/sector | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x590400` | RpGeometryRegisterPlugin | 1 | plugin registry | 2 | rwplg.h: rw::Geometry::registerPlugin [PLUGINBASE] | **differs**: librw plugin sizes/offsets and callback object pointers are different; maintain separate ABI registry and bridge callbacks/streams. |
| `0x590430` | RpGeometryRegisterPluginStream | 1 | stream/chunk I/O | 6 | rwplg.h: rw::Geometry::registerPluginStream [PLUGINBASE] | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x590670` | RpGeometryForAllMaterials | 5 | material/matfx | 4 | rwobjects.h: rw::Geometry::matList [array iteration] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x590B90` | RpGeometryAddRef | 1 | geometry/atomic/clump | 4 | geometry.cpp: rw::Geometry::addRef | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x590FC0` | RpGeometryDestroy | 1 | geometry/atomic/clump | 4 | geometry.cpp: rw::Geometry::destroy | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x591560` | RpMaterialSetTexture | 8 | raster/texture/image | 3 | geometry.cpp: rw::Material::setTexture | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x591590` | RpMaterialRegisterPlugin | 1 | plugin registry | 2 | rwplg.h: rw::Material::registerPlugin [PLUGINBASE] | **differs**: librw plugin sizes/offsets and callback object pointers are different; maintain separate ABI registry and bridge callbacks/streams. |
| `0x5915C0` | RpClumpRegisterPluginStream | 1 | stream/chunk I/O | 6 | rwplg.h: rw::Material::registerPluginStream [database name is wrong] | **differs**: Existing name wrong: original uses Material registry (same as MaterialRegisterPlugin); native Material registry offsets/callback pointers must be translated. |
| `0x592690` | RxD3D9AllInOneSetRenderCallBack | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5935D0` | RwOsGetFileInterface | 1 | file system | 2 | rwengine.h: rw::Engine::filefuncs [field; not the original filesystem interface layout] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x593D30` | RpHAnimFrameSetID | 1 | hanim/skin/animation | 5 | hanim.cpp: rw::HAnimData::get [id field] | **differs**: No native setter function: write HAnimData::get(mappedFrame)->id; constructor/plugin offset differs. |
| `0x59B990` | _rwD3D9VSSetActiveWorldMatrix | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x59BA20` | _rwD3D9VSGetComposedTransformMatrix | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x59CBD0` | RpLightSetRadius | 1 | light | 4 | rwobjects.h: rw::Light::radius [field] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x59CBF0` | RpLightSetColor | 1 | light | 4 | light.cpp: rw::Light::setColor | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x59CC50` | RpLightGetConeAngle | 2 | light | 4 | light.cpp: rw::Light::getAngle | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x59CE80` | RpLightSetConeAngle | 1 | light | 4 | light.cpp: rw::Light::setAngle | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x59CEC0` | RpLightRegisterPlugin | 1 | plugin registry | 2 | rwplg.h: rw::Light::registerPlugin [PLUGINBASE] | **differs**: librw plugin sizes/offsets and callback object pointers are different; maintain separate ABI registry and bridge callbacks/streams. |
| `0x59CEF0` | RpLightRegisterPluginStream | 1 | stream/chunk I/O | 6 | rwplg.h: rw::Light::registerPluginStream [PLUGINBASE] | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x59D000` | RpLightDestroy | 2 | light | 4 | light.cpp: rw::Light::destroy | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x59D040` | RpLightCreate | 1 | light | 4 | light.cpp: rw::Light::create | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x59FCB0` | RpWorldStreamGetSize | 1 | stream/chunk I/O | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x59FD10` | RpWorldStreamWrite | 2 | stream/chunk I/O | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x59FEA0` | RpWorldStreamRead | 2 | stream/chunk I/O | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5A1A34` | D3DXAssembleShader | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x5A1B17` | D3DXCompileShader | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x61ED00` | RtAnimRegisterInterpolationScheme | 1 | hanim/skin/animation | 5 | anim.cpp: rw::AnimInterpolatorInfo::registerInterp | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x61EE10` | RtAnimAnimationCreate | 2 | hanim/skin/animation | 5 | anim.cpp: rw::Animation::create | **differs**: Original uses interpolation type ID; native create uses AnimInterpolatorInfo*. Resolve scheme registration and bridge keyframe/custom-data ownership. |
| `0x61EE90` | RtAnimInterpolatorDestroy | 6 | hanim/skin/animation | 5 | anim.cpp: rw::AnimInterpolator::destroy | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x61EF90` | RtAnimAnimationStreamGetSize | 1 | hanim/skin/animation | 5 | anim.cpp: rw::Animation::streamGetSize | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x61EFB0` | RtAnimAnimationGetNumNodes | 4 | hanim/skin/animation | 5 | anim.cpp: rw::Animation::getNumNodes | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x61F050` | RtAnimInterpolatorSetCurrentAnim | 9 | hanim/skin/animation | 5 | anim.cpp: rw::AnimInterpolator::setCurrentAnim | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x61F120` | RtAnimInterpolatorCopy | 4 | hanim/skin/animation | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x61F150` | RtAnimInterpolatorAddAnimTime | 3 | hanim/skin/animation | 5 | anim.cpp: rw::AnimInterpolator::addTime | **differs**: Native addTime returns void and supports a reduced interpolator contract; original callbacks/time wrapping/subhierarchy policy require an adapter. |
| `0x61F320` | RtAnimInterpolatorBlend | 2 | hanim/skin/animation | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x61F3C0` | RtAnimAnimationRead | 1 | hanim/skin/animation | 5 | anim.cpp: rw::Animation::streamRead [needs StreamFile opening] | **differs**: Hierarchy/interpolator/skin layouts and callbacks differ; map handles and plugin payloads. Subhierarchy/compressed formats not implied by this candidate. |
| `0x61F590` | RtAnimInterpolatorSetCurrentTime | 6 | hanim/skin/animation | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x61F5E0` | _rtCompressedKeyFrameCompressFloat | 1 | hanim/skin/animation | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x620640` | RtIntersectionLineTriangle | 3 | collision/bounds | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x620840` | RtIntersectionSphereTriangle | 1 | collision/bounds | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x622EE0` | _rwSqrt | 2 | core/frame/matrix | 1 | rwbase.h: rw::length(const V3d&) [sqrtf; no standalone API] | **differs**: librw uses sqrtf, not the original lookup approximation; Normalize returns a vector, game API also returns length; zero-vector guard needed. |
| `0x622F40` | _rwInvSqrt | 1 | core/frame/matrix | 1 | rwbase.h: rw::normalize(const V3d&) [reciprocal sqrt; no standalone API] | **differs**: librw uses sqrtf, not the original lookup approximation; Normalize returns a vector, game API also returns length; zero-vector guard needed. |
| `0x622FA0` | RwV2dLength | 1 | core/frame/matrix | 1 | rwbase.h: rw::length(const V2d&) | **differs**: librw uses sqrtf, not the original lookup approximation; Normalize returns a vector, game API also returns length; zero-vector guard needed. |
| `0x623010` | RwV3dTransformPoint | 3 | core/frame/matrix | 1 | base.cpp: rw::V3d::transformPoints | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x623040` | RwV3dTransformPoints | 25 | core/frame/matrix | 1 | base.cpp: rw::V3d::transformPoints | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x623070` | RwV3dTransformVector | 3 | core/frame/matrix | 1 | base.cpp: rw::V3d::transformVectors | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x6230A0` | RwV3dTransformVectors | 24 | core/frame/matrix | 1 | base.cpp: rw::V3d::transformVectors | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x623250` | RwV3dNormalize | 2 | core/frame/matrix | 1 | rwbase.h: rw::normalize(const V3d&) | **differs**: librw uses sqrtf, not the original lookup approximation; Normalize returns a vector, game API also returns length; zero-vector guard needed. |
| `0x623360` | RwV3dLength | 17 | core/frame/matrix | 1 | rwbase.h: rw::length(const V3d&) | **differs**: librw uses sqrtf, not the original lookup approximation; Normalize returns a vector, game API also returns length; zero-vector guard needed. |
| `0x6235D0` | RwFrameCreate | 10 | core/frame/matrix | 1 | frame.cpp: rw::Frame::create | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x6236A0` | RwFrameUpdateObjects | 14 | core/frame/matrix | 1 | frame.cpp: rw::Frame::updateObjects | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x623720` | RwFrameGetLTM | 29 | core/frame/matrix | 1 | frame.cpp: rw::Frame::getLTM | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x623740` | RwFrameGetRoot | 3 | core/frame/matrix | 1 | rwobjects.h: rw::Frame::root [field; no function] | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x623750` | RwFrameRemoveChild | 6 | core/frame/matrix | 1 | frame.cpp: rw::Frame::removeChild | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x6237C0` | RwFrameForAllChildren | 13 | core/frame/matrix | 1 | frame.cpp: rw::Frame::forAllChildren | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x623830` | RwFrameTranslate | 5 | core/frame/matrix | 1 | frame.cpp: rw::Frame::translate | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x623860` | RwFrameTransform | 8 | core/frame/matrix | 1 | frame.cpp: rw::Frame::transform | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x623890` | RwFrameRotate | 1 | core/frame/matrix | 1 | frame.cpp: rw::Frame::rotate | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x6238C0` | RwFrameSetIdentity | 5 | core/frame/matrix | 1 | rwbase.h: rw::Matrix::setIdentity [Frame::matrix + updateObjects] | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x623910` | RwFrameForAllObjects | 2 | core/frame/matrix | 1 | rwobjects.h: rw::Frame::objectList [iteration; no function] | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x623950` | RwFrameRegisterPlugin | 1 | plugin registry | 2 | rwplg.h: rw::Frame::registerPlugin [PLUGINBASE] | **differs**: librw plugin sizes/offsets and callback object pointers are different; maintain separate ABI registry and bridge callbacks/streams. |
| `0x623B10` | RwFrameDestroy | 11 | core/frame/matrix | 1 | frame.cpp: rw::Frame::destroy | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x623B90` | RwFrameAddChild | 1 | core/frame/matrix | 1 | frame.cpp: rw::Frame::addChild | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x624560` | RwMatrixOptimize | 1 | core/frame/matrix | 1 | base.cpp: rw::Matrix::optimize | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x624620` | RwMatrixUpdate | 6 | core/frame/matrix | 1 | rwbase.h: rw::Matrix::update | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x624630` | RwMatrixMultiply | 10 | core/frame/matrix | 1 | base.cpp: rw::Matrix::mult | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x6246A0` | RwMatrixOrthoNormalize | 1 | core/frame/matrix | 1 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x6246C0` | RwMatrixRotateOneMinusCosineSine | 3 | core/frame/matrix | 1 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x6248B0` | RwMatrixRotate | 4 | core/frame/matrix | 1 | base.cpp: rw::Matrix::rotate | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x624950` | RwMatrixInvert | 5 | core/frame/matrix | 1 | base.cpp: rw::Matrix::invert | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x6249A0` | RwMatrixScale | 2 | core/frame/matrix | 1 | base.cpp: rw::Matrix::scale | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x624B30` | RwMatrixTranslate | 4 | core/frame/matrix | 1 | base.cpp: rw::Matrix::translate | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x624C70` | RwMatrixTransform | 6 | core/frame/matrix | 1 | base.cpp: rw::Matrix::transform | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x624D10` | RwMatrixDestroy | 224 | core/frame/matrix | 1 | base.cpp: rw::Matrix::destroy | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x624D40` | RwMatrixCreate | 132 | core/frame/matrix | 1 | base.cpp: rw::Matrix::create | **same semantics**: Compatible 32-bit value layout and operation; last-bit float differences allowed. No bitwise equivalence claim. |
| `0x625060` | RwImageCreate | 2 | raster/texture/image | 3 | image.cpp: rw::Image::create | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6250C0` | RwImageAllocatePixels | 2 | raster/texture/image | 3 | image.cpp: rw::Image::allocate | **differs**: Image::allocate returns void; preserve input Image handle and allocation-failure result; bpp inserts a layout field. |
| `0x625270` | RwImageSetPath | 12 | raster/texture/image | 3 | image.cpp: rw::Image::setSearchPath | **differs**: librw owns a search-path representation, not the old returned char-pointer contract; keep ABI-visible path storage. |
| `0x625320` | RwImageGetPath | 3 | raster/texture/image | 3 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x6254C0` | RwImageRead | 3 | raster/texture/image | 3 | image.cpp: rw::Image::read | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x625680` | RwImageRegisterImageFormat | 1 | raster/texture/image | 3 | image.cpp: rw::Image::registerFileFormat | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6257E0` | RwImageWrite | 1 | raster/texture/image | 3 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x625870` | RwRGBAToPixel | 2 | raster/texture/image | 3 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x625F10` | RwImageDestroy | 5 | raster/texture/image | 3 | image.cpp: rw::Image::destroy | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6265F0` | RwFreeListPurgeAllFreeLists | 6 | memory/freelist | 1 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x6269C0` | _rwDeviceSystemRequest | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x626AB0` | RwEngineGetCurrentVideoMode | 1 | engine | 2 | engine.cpp: rw::Engine::getCurrentVideoMode | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x626AC0` | Skin_DispatchType4Handler | 2 | hanim/skin/animation | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x626AE0` | RwEngineRegisterPlugin | 1 | plugin registry | 2 | rwengine.h: rw::Engine::registerPlugin | **differs**: librw plugin sizes/offsets and callback object pointers are different; maintain separate ABI registry and bridge callbacks/streams. |
| `0x626B30` | RwEngineSetSubSystem | 1 | engine | 2 | engine.cpp: rw::Engine::setSubSystem | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x626B90` | RwEngineGetNumVideoModes | 3 | engine | 2 | engine.cpp: rw::Engine::getNumVideoModes | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x626C40` | RwEngineSetVideoMode | 1 | engine | 2 | engine.cpp: rw::Engine::setVideoMode | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x626C90` | RwEngineGetTextureMemorySize | 1 | engine | 2 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x626CE0` | RwEngineStop | 2 | engine | 2 | engine.cpp: rw::Engine::stop | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x626D60` | RwEngineStart | 1 | engine | 2 | engine.cpp: rw::Engine::start | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x626E30` | RwEngineClose | 2 | engine | 2 | engine.cpp: rw::Engine::close | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x626EC0` | RwEngineTerm | 1 | engine | 2 | engine.cpp: rw::Engine::term | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x627290` | RwEngineOpen | 1 | engine | 2 | engine.cpp: rw::Engine::open | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x627360` | RwEngineInit | 1 | engine | 2 | engine.cpp: rw::Engine::init | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x627430` | RwRasterGetPluginOffset | 2 | raster/texture/image | 3 | rwplg.h: rw::Raster::getPluginOffset [PLUGINBASE] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x627470` | RwRasterRenderScaled | 4 | raster/texture/image | 3 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x627490` | RwRasterUnlock | 3 | raster/texture/image | 3 | raster.cpp: rw::Raster::unlock | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6274B0` | RwRasterClear | 7 | raster/texture/image | 3 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x627510` | RwRasterRenderFast | 3 | raster/texture/image | 3 | raster.cpp: rw::Raster::renderFast | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x627570` | RwRasterDestroy | 19 | raster/texture/image | 3 | raster.cpp: rw::Raster::destroy | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6275B0` | RwRasterPushContext | 9 | raster/texture/image | 3 | raster.cpp: rw::Raster::pushContext | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x627650` | RwRasterPopContext | 9 | raster/texture/image | 3 | raster.cpp: rw::Raster::popContext | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6276A0` | RwRasterGetNumLevels | 2 | raster/texture/image | 3 | raster.cpp: rw::Raster::getNumLevels | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6276E0` | RwRasterShowRaster | 3 | raster/texture/image | 3 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x627710` | RwRasterSubRaster | 6 | raster/texture/image | 3 | raster.cpp: rw::Raster::subRaster | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x627770` | RwRasterCreate | 15 | raster/texture/image | 3 | raster.cpp: rw::Raster::create | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x627810` | RwRasterLock | 3 | raster/texture/image | 3 | raster.cpp: rw::Raster::lock | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x628E50` | RwCameraBeginUpdate | 8 | camera | 4 | camera.cpp: rw::Camera::beginUpdate | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x628E90` | RwCameraSetNearClipPlane | 3 | camera | 4 | camera.cpp: rw::Camera::setNearPlane | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x628EC0` | RwCameraSetFarClipPlane | 3 | camera | 4 | camera.cpp: rw::Camera::setFarPlane | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x628EF0` | RwCameraFrustumTestSphere | 1 | camera | 4 | camera.cpp: rw::Camera::frustumTestSphere | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x628F60` | RwCameraClear | 4 | camera | 4 | camera.cpp: rw::Camera::clear | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x628F90` | RwCameraSetProjection | 3 | camera | 4 | rwobjects.h: rw::Camera::projection [field] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x629000` | RwCameraSetViewWindow | 5 | camera | 4 | camera.cpp: rw::Camera::setViewWindow | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x629070` | RwCameraDestroy | 4 | camera | 4 | camera.cpp: rw::Camera::destroy | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6290B0` | RwCameraCreate | 5 | camera | 4 | camera.cpp: rw::Camera::create | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6293E0` | RwStreamRead | 7 | stream/chunk I/O | 2 | base.cpp: rw::Stream::read8 | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x629530` | RwStreamWrite | 4 | stream/chunk I/O | 2 | base.cpp: rw::Stream::write8 | **differs**: Native write8 returns byte count; game API returns stream handle/null on failure, not the count. |
| `0x629700` | RwStreamSkip | 3 | stream/chunk I/O | 2 | rwbase.h: rw::Stream::seek | **differs**: Native seek is void and takes whence; check/retain failure policy and return original stream handle. |
| `0x629810` | RwStreamClose | 17 | stream/chunk I/O | 2 | rwbase.h: rw::Stream::close | **differs**: Virtual close/destructor versus C close callback/data contract; preserve close ownership and original success return. |
| `0x6298E0` | RwStreamOpen | 17 | stream/chunk I/O | 2 | base.cpp: rw::StreamFile::open / StreamMemory::open | **differs**: Construct StreamFile or StreamMemory from original stream type/access enums and unions; C stream object cannot be reinterpreted as a virtual Stream. |
| `0x629A80` | _rwStreamWriteVersionedChunkHeader | 1 | stream/chunk I/O | 2 | base.cpp: rw::writeChunkHeader [uses global version/build] | **differs**: librw writeChunkHeader uses global version/build; original has explicit version parameter, so an explicit header adapter is needed. |
| `0x629AE0` | RwStreamFindChunk | 5 | stream/chunk I/O | 2 | base.cpp: rw::findChunk | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x629BE0` | RwStreamWriteReal | 1 | stream/chunk I/O | 2 | base.cpp: rw::Stream::write32 [float array bytes] | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x629C70` | RwStreamWriteInt32 | 6 | stream/chunk I/O | 2 | base.cpp: rw::Stream::write32 | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x629C90` | RwStreamReadInt32 | 6 | stream/chunk I/O | 2 | base.cpp: rw::Stream::read32 | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x629CE0` | RwStreamReadChunkHeaderInfo | 4 | stream/chunk I/O | 2 | base.cpp: rw::readChunkHeaderInfo | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x62B170` | RwD3D9EngineSetMultiSamplingLevels | 1 | D3D9 driver | 7 | engine.cpp: rw::Engine::setMultiSamplingLevels | **differs**: Native setter is Engine-level; translate levels, pre-open lifecycle and status policy rather than merely storing a global. |
| `0x62B200` | RwD3D9EngineSetRefreshRate | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x62B2A0` | RwD3D9EngineSetFullScreenDialogBoxMode | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x62B670` | RwD3D9GetCurrentD3DDevice | 1 | D3D9 driver | 7 | d3d/rwd3d.h: rw::d3d::d3ddevice [native device pointer] | **differs**: Native D3D9 backend only; translate handles/signatures and respect or invalidate librw state caches after direct COM calls. |
| `0x62B720` | RwD3D9SetVertexDeclaration | 1 | D3D9 driver | 7 | d3d/d3d9render.cpp: rw::d3d::setVertexDeclaration | **differs**: Native D3D9 backend only; translate handles/signatures and respect or invalidate librw state caches after direct COM calls. |
| `0x62B760` | RwD3D9SetVertexShader | 1 | D3D9 driver | 7 | d3d/d3d9render.cpp: rw::d3d::setVertexShader | **differs**: Native D3D9 backend only; translate handles/signatures and respect or invalidate librw state caches after direct COM calls. |
| `0x62B7A0` | RwD3D9SetPixelShader | 1 | D3D9 driver | 7 | d3d/d3d9render.cpp: rw::d3d::setPixelShader | **differs**: Native D3D9 backend only; translate handles/signatures and respect or invalidate librw state caches after direct COM calls. |
| `0x62B840` | _rwD3D9SetStreams | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x62B970` | RwD3D9SetIndices | 1 | D3D9 driver | 7 | d3d/d3d9render.cpp: rw::d3d::setIndices | **differs**: Native D3D9 backend only; translate handles/signatures and respect or invalidate librw state caches after direct COM calls. |
| `0x62B9A0` | RwD3D9DrawIndexedPrimitive | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x62BA10` | RwD3D9SetTransform | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x62BB70` | RwD3D9GetTransform | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x62C060` | RwD3D9CreateVertexShader | 1 | D3D9 driver | 7 | d3d/d3drender.cpp: rw::d3d::createVertexShader | **differs**: Native createVertexShader returns a shader pointer; original takes an output pointer/status contract. Native COM handle can be shared. |
| `0x62C0A0` | RwD3D9SetVertexShaderConstant | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x62C0C0` | RwD3D9CreatePixelShader | 1 | D3D9 driver | 7 | d3d/d3drender.cpp: rw::d3d::createPixelShader | **differs**: Native createPixelShader returns a shader pointer; original takes an output pointer/status contract. Native COM handle can be shared. |
| `0x62C0F0` | RwD3D9SetPixelShaderConstant | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x62C110` | RwD3D9GetCaps | 2 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x62D650` | RwD3D9SetRenderState | 1 | D3D9 driver | 7 | d3d/d3drender.cpp: rw::d3d::setRenderState | **differs**: Native D3D9 backend only; translate handles/signatures and respect or invalidate librw state caches after direct COM calls. |
| `0x62F950` | RwD3D9SetTexture | 1 | D3D9 driver | 7 | d3d/d3ddevice.cpp: rw::d3d::setTexture | **differs**: Native setTexture takes stage first and Texture second; translate original argument order and Texture/Raster sidecar before touching the cache. |
| `0x6307D0` | RwTexDictionaryStreamGetSize | 1 | stream/chunk I/O | 6 | texture.cpp: rw::TexDictionary::streamGetSize | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x6308E0` | RwTexDictionaryStreamWrite | 1 | stream/chunk I/O | 6 | texture.cpp: rw::TexDictionary::streamWrite | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x630A20` | RwTexDictionaryStreamRead | 1 | stream/chunk I/O | 6 | texture.cpp: rw::TexDictionary::streamRead | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x6315B0` | RwTextureSetFindCallBack | 1 | raster/texture/image | 3 | rwobjects.h: rw::Texture::findCB [field] | **differs**: Native findCB is a field; setter must return the previous callback and bridge Texture pointers. |
| `0x6315D0` | RwTextureSetReadCallBack | 1 | raster/texture/image | 3 | rwobjects.h: rw::Texture::readCB [field] | **differs**: Native readCB is a field; setter must return the previous callback and bridge Texture/Raster ownership. |
| `0x631650` | RwTexDictionaryCreate | 4 | raster/texture/image | 3 | texture.cpp: rw::TexDictionary::create | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6316E0` | RwTexDictionaryForAllTextures | 13 | raster/texture/image | 3 | rwobjects.h: rw::TexDictionary::textures [list iteration] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x631730` | RwTexDictionaryForAllTexDictionaries | 1 | raster/texture/image | 3 | texture.cpp: rw::TextureGlobals::texDicts [private list; new enumeration adapter needed] | **differs**: Native global dictionary list is private TextureGlobals storage; expose an enumeration adapter, translating pointers and early-stop callbacks. |
| `0x631780` | RwTextureCreate | 5 | raster/texture/image | 3 | texture.cpp: rw::Texture::create | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6317E0` | RwTextureDestroy | 20 | raster/texture/image | 3 | texture.cpp: rw::Texture::destroy | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x631800` | RwTextureSetName | 2 | raster/texture/image | 3 | rwobjects.h: rw::Texture::name [field] | **differs**: Native name is a 32-byte field, not a setter API; implement truncation/NUL policy and return the ABI texture. |
| `0x6318E0` | RwTexDictionaryAddTexture | 3 | raster/texture/image | 3 | texture.cpp: rw::TexDictionary::add | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x631920` | RwTexDictionaryRemoveTexture | 4 | raster/texture/image | 3 | texture.cpp: rw::TexDictionary::remove | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x631950` | RwTexDictionaryFindNamedTexture | 6 | raster/texture/image | 3 | texture.cpp: rw::TexDictionary::find | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x631990` | RwTexDictionarySetCurrent | 2 | raster/texture/image | 3 | texture.cpp: rw::TexDictionary::setCurrent | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6319B0` | RwTexDictionaryGetCurrent | 12 | raster/texture/image | 3 | texture.cpp: rw::TexDictionary::getCurrent | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6319C0` | RwTextureRead | 4 | raster/texture/image | 3 | texture.cpp: rw::Texture::read | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x632340` | RwTexDictionaryDestroy | 6 | raster/texture/image | 3 | texture.cpp: rw::TexDictionary::destroy | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x6327D0` | _rwFrameSyncDirty | 1 | core/frame/matrix | 1 | frame.cpp: rw::Frame::syncDirty | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x6329C0` | RwFrameSetStaticPlugins | 1 | plugin registry | 2 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x6329D0` | RwFrameRegisterPluginStream | 1 | stream/chunk I/O | 6 | rwplg.h: rw::Frame::registerPluginStream [PLUGINBASE] | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x633010` | RwImageSetFromRaster | 1 | raster/texture/image | 3 | raster.cpp: rw::Raster::toImage | **differs**: Raster::toImage allocates/returns an Image rather than filling the caller-provided Image; copy/convert into the supplied ABI image and preserve ownership. |
| `0x633040` | RwRasterSetFromImage | 4 | raster/texture/image | 3 | raster.cpp: rw::Raster::setFromImage | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x633070` | RwImageFindRasterFormat | 1 | raster/texture/image | 3 | raster.cpp: rw::Raster::imageFindRasterFormat | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x633710` | RxPipelineCreate | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x633780` | RxPipelineDestroy | 2 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x6337F0` | RxPipelineSetMaxNodes | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x637E30` | RxLockedPipeUnlock | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x638210` | RxPipelineLock | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x638320` | RxPipelineFindNodeByName | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x6383C0` | RxLockedPipeAddFragment | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x63A4D0` | RwD3D9RasterStreamReadDDS | 1 | D3D9 driver | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x63BD90` | RwMatrixStreamGetSize | 1 | stream/chunk I/O | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x63BDA0` | RwMatrixStreamWrite | 1 | stream/chunk I/O | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x63BF20` | RwMatrixStreamRead | 1 | stream/chunk I/O | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x63C8B0` | RwBBoxCalculate | 1 | collision/bounds | 6 | base.cpp: rw::BBox::calculate | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x63C9B0` | RwBBoxAddPoint | 3 | collision/bounds | 6 | base.cpp: rw::BBox::addPoint | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x63CA30` | RwBBoxContainsPoint | 1 | collision/bounds | 6 | base.cpp: rw::BBox::containsPoint | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x63CB30` | _rwPluginRegistryReadDataChunks | 1 | stream/chunk I/O | 6 | plg.cpp: rw::PluginList::streamRead | **differs**: Stream/handle ABI differs; chunk version and plugin dispatch must be preserved, including unknown extensions. |
| `0x63CE30` | _rwObjectHasFrameSetFrame | 11 | core/frame/matrix | 2 | rwobjects.h: rw::ObjectWithFrame::setFrame | **differs**: Native Frame matrix/list order differs (+0x18 native matrix vs +0x10 ABI); bridge hierarchy/list/callback pointers and void-method→original-handle/status returns. |
| `0x63CED0` | RwIm3DTransform | 6 | immediate mode | 7 | render.cpp: rw::im3DTransform | **differs**: Vertex declaration/UV and camera-Z conventions are backend-specific; convert arrays and primitive enum, not raw passthrough. |
| `0x63CFA0` | RwIm3DEnd | 6 | immediate mode | 7 | render.cpp: rw::im3DEnd | **differs**: Vertex declaration/UV and camera-Z conventions are backend-specific; convert arrays and primitive enum, not raw passthrough. |
| `0x63CFD0` | RwIm3DRenderIndexedPrimitive | 4 | immediate mode | 7 | render.cpp: rw::im3DRenderIndexedPrimitive | **differs**: Vertex declaration/UV and camera-Z conventions are backend-specific; convert arrays and primitive enum, not raw passthrough. |
| `0x63D130` | RwIm3DRenderPrimitive | 2 | immediate mode | 7 | render.cpp: rw::im3DRenderPrimitive | **differs**: Vertex declaration/UV and camera-Z conventions are backend-specific; convert arrays and primitive enum, not raw passthrough. |
| `0x7C5680` | unnamed | 1 | raster/texture/image | 3 | bmp.cpp: rw::readBMP | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. High confidence BMP filename reader: stream mode read, signature 19778 (BM), DIB header sizes and palette handling. Native image layout differs. |
| `0x7C5CA0` | unnamed | 1 | raster/texture/image | 3 | bmp.cpp: rw::writeBMP | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. High confidence BMP filename writer: paired BMP header/palette/pixel writers, stream mode write; original returns image/null whereas librw returns void. |
| `0x7C5D00` | RtCompressedKeyConvertFromStd | 2 | hanim/skin/animation | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7C5F60` | RwResourcesSetArenaSize | 1 | memory/freelist | 2 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7C61A0` | RtFSManagerSetDefaultFileSystem | 2 | file system | 2 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7C6300` | Audio_ReleaseSharedState | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7C73A0` | RtSkinSplitAtomicSplit | 1 | hanim/skin/animation | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7C8D30` | unnamed | 1 | raster/texture/image | 3 | png.cpp: rw::writePNG | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. High confidence PNG filename writer: libpng 1.0.12 create-write context, setjmp, image palette/RGBA conversion; original returns image/null whereas librw returns void. This is RW image glue, not a reimplementation of libpng. |
| `0x7C9050` | unnamed | 1 | raster/texture/image | 3 | png.cpp: rw::readPNG | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. High confidence PNG filename reader: libpng 1.0.12 create-read context, setjmp, read info/palette/pixels; native Image layout differs. This is RW image glue, not libpng internals. |
| `0x7D9D40` | RtQuatConvertFromMatrix | 2 | core/frame/matrix | 2 | base.cpp: rw::Matrix::getRotation | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x7D9E30` | RtQuatRotate | 8 | core/frame/matrix | 2 | base.cpp: rw::Quat::rotate | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x7DA4E0` | RtIntersectionLineSphere | 3 | collision/bounds | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7DB650` | DirectInput8Create | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7DB6F0` | Audio_OpenEngine | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7DB820` | _rwGetNumEngineInstances | 3 | engine | 2 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7DB920` | _rxPipelineOpen | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7DB960` | _rxPipelineClose | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7DBD20` | RtDictFindNamedEntry | 2 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7DBD70` | RtDictGetEntry | 5 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7DBD80` | RtDictGetNumEntries | 6 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7DBE70` | RpMTEffectDictGetCurrent | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7DBE80` | RpMTEffectDictSetCurrent | 4 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7DE550` | DirectSoundEnum_RunWithCallback | 3 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7DE590` | RwFreeListForAllFreeLists | 2 | memory/freelist | 2 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7E4970` | unnamed | 1 | toolkit/unknown | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. High confidence: one-byte RET, no side effects; formal toolkit name unknown. Caller passes child count. |
| `0x7F17C0` | Audio_InitializeEffectDictionarySupport | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F1810` | unnamed | 1 | toolkit/unknown | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. High confidence shape: registers three dictionary/schema descriptors for type IDs 4, 9, 2; exact package/name uncertain, audio-facing caller. |
| `0x7F78E6` | GetDeviceID | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F78EC` | DirectSoundCaptureCreate | 1 | other boundary/pipeline | 7 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F7900` | Rt2dObjectDestroy | 2 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F7BF0` | GfxTeam_GetObjectBoundsData | 2 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F8250` | Rt2dObjectSetMTM | 3 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F8320` | Rt2dObjectSetRenderCallBack | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F8330` | Rt2dObjectGetRenderCallBack | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F8340` | Rt2dObjectSetColorOffset | 4 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F8390` | Rt2dObjectSetColorMultiplier | 4 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F83E0` | Rt2dObjectSetPosition | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F8410` | Rt2dObjectGetPosition | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F9860` | Rt2dObjectSetBoundingRadius | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F9880` | Rt2dObjectStringGetHeight | 2 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7F9B40` | GfxScene_Set2dObjectSize | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FA460` | CollisionShapes_ApplyShapeParameter | 1 | collision/bounds | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FA5B0` | CollisionShapes_ConfigureDefaultShape | 1 | collision/bounds | 6 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FAFE0` | Gfx2d_SetSceneVector | 2 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FB000` | RwImageSetHeight | 2 | raster/texture/image | 3 | rwobjects.h: rw::Image::height [field] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x7FB010` | RwImageSetWidth | 2 | raster/texture/image | 3 | rwobjects.h: rw::Image::width [field] | **differs**: Same purpose, not raw ABI compatibility: translate facade pointers/fields, callbacks and return conventions; see structure section. |
| `0x7FB020` | RpToonInkSetPerspectiveScale | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FB030` | RpToonInkSetNearDistance | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FB040` | RpToonInkSetFarScale | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FB050` | RpToonInkSetFarPerspectiveScale | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FD8D0` | Gfx2d_CreateRenderHandle | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FD9C0` | Rt2dMaestroDestroy | 1 | toolkits | 9 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FDA30` | RtDictAddEntry | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FDA80` | RtDictRemoveEntry | 2 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FDAD0` | RtDictGetSchema | 2 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FDB00` | RtBinMtxCreate | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FDB40` | _rtBitMatrixDestroy | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FE6B0` | RpTeamDictSetFlags | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FE950` | RpTeamRemovePlayer | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FEDC0` | unnamed | 1 | toolkit/team | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. Medium confidence identity: team/player transform-output query gated by membership bitset; +0x20 player ID, +0x68 membership array, caller consumes six output dwords. Formal API name unknown. |
| `0x7FEE10` | RpTeamPlayerSetActive | 5 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FEE60` | _rpTeamPlayerSelect | 5 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FEF10` | _rpTeamPlayerDeselect | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FEFA0` | GfxWater_UpdateRenderObjectFrame | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FF0A0` | RpTeamPlayerSetHAnimHierarchy | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FF0C0` | RpTeamPlayerGetUserData | 1 | user data | 5 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FF0E0` | Gfx2d_CreateVisibilityProxy | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FFB90` | RtWorldImportParametersInit | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FFBF0` | Gfx2d_SetImportSectorLimits | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x7FFC20` | RtWorldImportParametersSetMaxSectorSize | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x8006D0` | Gfx2d_ScaleBounds | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x8013C0` | Rt2dSceneGetChildCount | 6 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x8013D0` | Rt2dSceneGetChild | 2 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x801520` | Rt2dClumpGetClump | 2 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x802410` | GfxTeam_ApplyPlayerMovement | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x802650` | GfxTeam_GetBrushWidth | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x802660` | Rt2dBrushGetHeight | 1 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |
| `0x803770` | Water_ApplyTransform | 3 | toolkits | 10 | none | **missing**: No corresponding API found in this checkout; implement game boundary independently, not original library code. |

# RW offset evidence

This is a conservative grep audit, NOT complete interprocedural type recovery. Boundary-local taints (returns or simple arguments) and name-only candidates are deliberately separate. A file is counted once per offset. Reassignments/aliases need manual review; these are not a reaching-definition proof. For exact carriers and matches see RW_PORT_OFFSETS.csv. Offsets that refer to the game wrapper rather than an RW object are possible in the candidate column.

| Structure | Boundary locals: offset (files) | Name-only candidates: offset (files) |
|---|---|---|
| RpAtomic | `0x4` (3), `0x18` (2), `0x48` (1) | `0x0` (2), `0x4` (9), `0xc` (2), `0x18` (16), `0x28` (3), `0x48` (7), `0x50` (4), `0x70` (1) |
| RpClump | `0x4` (10) | `0x0` (18), `0x4` (15), `0x8` (2), `0xc` (1), `0x9c` (1), `0xa0` (1), `0xa4` (1), `0xa8` (1), `0xac` (1), `0xb0` (1) |
| RpGeometry | `0x8` (1) | `0x0` (15), `0x4` (1), `0x8` (2), `0xb` (2), `0xc` (1), `0x10` (2), `0x14` (1), `0x18` (3), `0x1c` (1), `0x20` (15), `0x24` (7), `0x28` (2), `0x2c` (4), `0x30` (3), `0x34` (1), `0x40` (1), `0x5c` (2), `0x60` (1), `0x64` (1) |
| RpHAnimHierarchy | none | `0x0` (2), `0x4` (11), `0x10` (9), `0x14` (2), `0x20` (13), `0x24` (1), `0x30` (1), `0x34` (1), `0x38` (1) |
| RpLight | none | `0x0` (17), `0x1` (2), `0x4` (8), `0x8` (5), `0xc` (5), `0x14` (3), `0x18` (1), `0x1c` (2), `0x20` (3), `0x24` (2), `0x28` (1) |
| RpMaterial | `0x0` (2), `0x4` (2) | `0x0` (9), `0x4` (5), `0x8` (1), `0x10` (1), `0x18` (3), `0x1c` (1), `0x20` (2), `0x24` (1), `0x28` (2), `0x2c` (1), `0x34` (1), `0x44` (1), `0x48` (1), `0x4c` (1), `0x70` (1), `0x74` (1), `0x78` (1), `0x7c` (1), `0x80` (1) |
| RpSkin | none | `0x0` (18), `0x18` (1), `0xf0` (1) |
| RpWorld | `0x8` (1) | none |
| RpWorld/sector | none | `0x0` (20), `0x4` (13), `0x6` (1), `0x8` (10), `0xb` (3), `0xc` (9), `0x10` (9), `0x14` (2), `0x18` (1), `0x1c` (2), `0x20` (8), `0x24` (1), `0x25` (1), `0x28` (9), `0x30` (3), `0x34` (3), `0x38` (2), `0x3c` (2), `0x40` (3), `0x44` (1), `0x48` (2), `0x4c` (2), `0x50` (5), `0x54` (5), `0x58` (5), `0x5c` (5), `0x60` (3), `0x64` (3), `0x80` (1), `0x82` (1), `0x84` (2), `0x91` (1), `0xb4` (2), `0xb8` (1), `0xc0` (2), `0xc4` (1), `0xc8` (1), `0xcc` (1), `0xd4` (1), `0xf4` (1), `0xf8` (1), `0x100` (1), `0x108` (1), `0x110` (1), `0x118` (1), `0x130` (1), `0x134` (1), `0x138` (1), `0x13c` (1), `0x140` (1), `0x144` (1), `0x148` (1), `0x15c` (1), `0x170` (1), `0x180` (1), `0x184` (1), `0x19c` (1), `0x1ac` (1), `0x1b0` (1), `0x1f0` (1), `0x220` (1), `0x224` (1), `0x228` (1), `0x310` (1), `0x33c` (1), `0x1fd8` (1) |
| RtAnimAnimation | `0x10` (2) | `0x0` (34), `0x4` (6), `0x8` (4), `0xc` (7), `0xd` (1), `0xe` (1), `0x10` (10), `0x14` (5), `0x18` (3), `0x1c` (3), `0x1d` (1), `0x20` (6), `0x24` (9), `0x28` (2), `0x29` (1), `0x2a` (1), `0x2b` (1), `0x2c` (2), `0x30` (2), `0x34` (5), `0x38` (1), `0x3c` (1), `0x40` (1), `0x44` (1), `0x48` (1), `0x54` (1), `0x58` (5), `0x59` (2), `0x5a` (2), `0x5b` (1), `0x5c` (1), `0x5d` (1), `0x1c4` (1), `0x1d8` (1), `0x1e0` (1) |
| RtAnimInterpolator | none | `0x0` (5), `0x4` (4), `0x24` (1) |
| RwCamera | `0x4` (1) | `0x0` (14), `0x4` (6), `0x8` (2), `0xc` (1), `0x10` (3), `0x14` (3), `0x18` (2), `0x1c` (3), `0x20` (4), `0x24` (2), `0x28` (3), `0x2c` (1), `0x30` (1), `0x34` (1), `0x38` (1), `0x3c` (1), `0x40` (2), `0x44` (2), `0x48` (2), `0x60` (3), `0x64` (5) |
| RwFrame | `0x4` (3), `0x10` (3), `0x14` (3), `0x18` (3), `0x1c` (3), `0x20` (3), `0x24` (3), `0x28` (3), `0x30` (3), `0x34` (3), `0x38` (3), `0x40` (3), `0x44` (3), `0x48` (3) | `0x0` (29), `0x2` (2), `0x4` (36), `0x8` (11), `0xa` (1), `0xc` (8), `0xe` (2), `0x10` (10), `0x12` (2), `0x14` (11), `0x18` (16), `0x1c` (12), `0x20` (12), `0x21` (1), `0x22` (1), `0x24` (9), `0x28` (4), `0x2c` (1), `0x30` (6), `0x34` (6), `0x38` (7), `0x3c` (2), `0x40` (7), `0x44` (5), `0x48` (4), `0x50` (2), `0x7c` (1), `0x8c` (1), `0x90` (2), `0x98` (2), `0xab` (1) |
| RwImage | `0x4` (2), `0x8` (2), `0xc` (1), `0x10` (1), `0x14` (2) | `0x0` (1), `0x4` (3), `0x8` (3), `0xc` (2), `0x10` (1), `0x14` (2) |
| RwMatrix | `0x0` (6), `0x4` (5), `0x8` (5), `0xc` (1), `0x10` (3), `0x14` (4), `0x18` (3), `0x20` (5), `0x24` (5), `0x28` (6), `0x30` (5), `0x34` (5), `0x38` (5) | `0x0` (118), `0x4` (122), `0x8` (34), `0xc` (22), `0x10` (16), `0x14` (17), `0x18` (11), `0x20` (33), `0x24` (33), `0x28` (35), `0x30` (23), `0x34` (22), `0x38` (21), `0x44` (1), `0x48` (1), `0x4c` (1), `0x50` (1) |
| RwRaster | `0xc` (3), `0x10` (3), `0x14` (1), `0x23` (1) | `0xc` (8), `0x10` (9), `0x14` (6), `0x23` (3) |
| RwTexture | `0x30` (1), `0x31` (1), `0x34` (1), `0x38` (1), `0x3c` (1), `0x40` (1), `0x44` (1), `0x48` (1), `0x4c` (1), `0x50` (4), `0x51` (1), `0x54` (3) | `0x0` (34), `0x4` (5), `0x8` (2), `0xc` (2), `0x14` (1), `0x18` (2), `0x30` (1), `0x31` (1), `0x34` (1), `0x38` (1), `0x3c` (1), `0x40` (1), `0x44` (1), `0x48` (1), `0x4c` (1), `0x50` (4), `0x51` (1), `0x54` (6) |

## Direct device vtable grep

These are candidates, not an exhaustive device-flow proof. The manually confirmed methods are in RW_PORT.md.

- `0045f9a0.cpp`: `CSF_AT(void**, inputDevice, 0)[2]`
- `0045f9a0.cpp`: `CSF_AT(void**, targetComponent, 0)[42]`
- `004d3d30.cpp`: `CSF_AT(void**, provider, 0)[0x3C / 4]`
- `004d4680.cpp`: `CSF_AT(void**, skin, 0)[0x3C / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, target, 0)[0x28 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, owner, 0)[0x80 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, materialTable, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, definitions, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, factory, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, effect, 0)[0x40 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, effect, 0)[0x28 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, definitions, 0)[0x10 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, definitions, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, factory, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, effect, 0)[0x40 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, effect, 0)[0x28 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, parameterTable, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, registry, 0)[0x38 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, registry, 0)[0x3C / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, soundObject, 0)[0xC4 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, definitions, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, factory, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, effect, 0)[0x40 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, effect, 0)[0x28 / 4]`
- `00526780.cpp`: `CSF_AT(void**, voice, 0)[0x28 / 4]`
- `00526780.cpp`: `CSF_AT(void**, voice, 0)[0x28 / 4]`
- `005268c0.cpp`: `CSF_AT(void**, globalObjA, 0)[0x4C / 4]`
- `005268c0.cpp`: `CSF_AT(void**, globalObjB, 0)[0x44 / 4]`
- `005268c0.cpp`: `CSF_AT(void**, voice, 0)[0x1C / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[8 / 4]`
- `00527630.cpp`: `CSF_AT(void**, manager, 0)[0xC / 4]`
- `00527630.cpp`: `CSF_AT(void**, device, 0)[0x6C / 4]`
- `00527630.cpp`: `CSF_AT(void**, device, 0)[0x84 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x4 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x54 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x40 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x40 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x4C / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x1C / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x14 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x38 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x14 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x14 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x38 / 4]`
- `00527dc0.cpp`: `CSF_AT(void**, audioDevice, 0)[0x34 / 4]`
- `00527dc0.cpp`: `CSF_AT(void**, voiceObject, 0)[0x8 / 4]`
- `006bd3d0.cpp`: `CSF_AT(void**, self, 0)[0x38 / 4]`
- `006bd3d0.cpp`: `CSF_AT(void**, clumpManager, 0)[0x88 / 4]`
- `006bd3d0.cpp`: `CSF_AT(void**, sceneController, 0)[0x38 / 4]`
- `006cd0e0.cpp`: `CSF_AT(void**, renderResources, 0)[1]`
- `006cd0e0.cpp`: `CSF_AT(void**, worldResources, 0)[1]`
- `006cdaa0.cpp`: `CSF_AT(void**, accessor, 0)[0x10 / 4]`
- `006cdaa0.cpp`: `CSF_AT(void**, textureManager, 0)[0x10 / 4]`
- `006cdaa0.cpp`: `CSF_AT(void**, world, 0)[0x2C / 4]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[32]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[4]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[21]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[21]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[21]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[22]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[21]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[21]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[22]`
- `006d3db0.cpp`: `CSF_AT(void**, cameraObject, 0)[14]`
- `006d3db0.cpp`: `CSF_AT(void**, cameraObject, 0)[16]`
- `006d3db0.cpp`: `CSF_AT(void**, cameraObject, 0)[8]`
- `006d3db0.cpp`: `CSF_AT(void**, cameraObject, 0)[24]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[31]`
- `006dc2c0.cpp`: `CSF_AT(void**, device, 0)[0x5C / 4]`
- `006dc2c0.cpp`: `CSF_AT(void**, texture, 0)[0x48 / 4]`
- `006dc2c0.cpp`: `CSF_AT(void**, device, 0)[0x84 / 4]`
- `006dc2c0.cpp`: `CSF_AT(void**, surface, 0)[0x34 / 4]`
- `006dc2c0.cpp`: `CSF_AT(void**, surface, 0)[0x38 / 4]`
- `006dc2c0.cpp`: `CSF_AT(void**, surface, 0)[0x8 / 4]`
- `006dc2c0.cpp`: `CSF_AT(void**, texture, 0)[0x8 / 4]`
- `006dc560.cpp`: `CSF_AT(void**, device, 0)[0x54 / 4]`
- `006dc750.cpp`: `deviceVtable[0x1C / 4]`
- `006dc9b0.cpp`: `CSF_AT(void**, renderDevice, 0)[0x74 / 4]`
- `006e4e90.cpp`: `CSF_AT(void**, controller, 0)[8 / 4]`
- `006e4e90.cpp`: `CSF_AT(void**, device, 0)[0xE4 / 4]`
- `006e4e90.cpp`: `CSF_AT(void**, device, 0)[0xE4 / 4]`
- `006f61b0.cpp`: `CSF_AT(void**, device, 0)[0xE4 / 4]`
- `006f61b0.cpp`: `CSF_AT(void**, device, 0)[0xE4 / 4]`
- `00709810.cpp`: `CSF_AT(void**, file, 0)[7]`
- `00709810.cpp`: `CSF_AT(void**, file, 0)[2]`
- `00709810.cpp`: `CSF_AT(void**, obj, 0)[3]`
- `00709810.cpp`: `CSF_AT(void**, obj, 0)[2]`
- `00709bc0.cpp`: `CSF_AT(void**, owner, 0)[2]`
- `00709bc0.cpp`: `CSF_AT(void**, lightingSource, 0)[9]`
- `00709bc0.cpp`: `CSF_AT(void**, frameProvider, 0)[35]`
- `0070d700.cpp`: `CSF_AT(void**, oldCamera, 0)[4 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, sourceCamera, 0)[16 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, frame, 0)[140 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, sourceCamera, 0)[16 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, frame, 0)[140 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, sourceCamera, 0)[16 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, frame, 0)[140 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, reflectionCamera, 0)[16 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, frame, 0)[148 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, sourceCamera, 0)[16 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, frame, 0)[140 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, reflectionCamera, 0)[16 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, frame, 0)[148 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, sourceCamera, 0)[36 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, reflectionCamera, 0)[4 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, self, 0)[0x70 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, controls, 0)[0x20 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, self, 0)[0x70 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, controls, 0)[0x28 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, self, 0)[0x70 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, controls, 0)[0x1C / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, self, 0)[0x70 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, controls, 0)[0x24 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, self, 0)[0x68 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, self, 0)[0x5C / 4]`
- `0070f630.cpp`: `CSF_AT(void**, file, 0)[8 / 4]`
- `0070f630.cpp`: `CSF_AT(void**, self, 0)[0xA8 / 4]`
- `0070f630.cpp`: `CSF_AT(void**, self, 0)[0xA8 / 4]`
- `0070f630.cpp`: `CSF_AT(void**, self, 0)[0xA8 / 4]`
- `0070f630.cpp`: `CSF_AT(void**, self, 0)[0xA8 / 4]`
- `00710850.cpp`: `CSF_AT(void**, self, 0)[0xD0 / 4]`
- `00710df0.cpp`: `CSF_AT(void**, self, 0)[49]`
- `00710ff0.cpp`: `CSF_AT(void**, device, 0)[3]`
- `00710ff0.cpp`: `CSF_AT(void**, outBuffer, 0)[0]`
- `00710ff0.cpp`: `CSF_AT(void**, outBuffer, 0)[2]`
- `00712f00.cpp`: `deviceVtable[0x20 / 4]`
- `00712f00.cpp`: `deviceVtable[0x20 / 4]`
- `00712f80.cpp`: `CSF_AT(void**, device, 0)[0x18 / 4]`
- `00713030.cpp`: `CSF_AT(void**, device, 0)[8]`
- `007130b0.cpp`: `CSF_AT(void**, device, 0)[kDeviceSlotUnacquire]`
- `007130b0.cpp`: `CSF_AT(void**, device, 0)[kDeviceSlotAcquire]`
- `007130b0.cpp`: `CSF_AT(void**, device, 0)[kDeviceSlotAcquire]`
- `007132f0.cpp`: `CSF_AT(void**, input, 0)[12 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, keyboard, 0)[44 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, keyboard, 0)[52 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, keyboard, 0)[24 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, keyboard, 0)[28 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, input, 0)[12 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, mouse, 0)[44 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, mouse, 0)[52 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, mouse, 0)[28 / 4]`
