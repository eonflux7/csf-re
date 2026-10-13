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
