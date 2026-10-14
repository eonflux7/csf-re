# Plugin/startup cutover, resumed rwplugins job

## Finding

The plugin-attach barrier has concrete address slots and ABI extension storage.
The interrupted tree also contained a layout bug: **00590400/00590430 operate
on world sectors, not geometries**. Their executable registry is 0089A858,
prefix 136 bytes. **00590BA0/00590BD0 operate on geometries**, registry 0089A870,
prefix 96 bytes. Geometry destruction at 00590F50 uses the latter registry;
WorldCreate constructs its empty root with the former. Removed the artificial
40-byte geometry padding and routed registrations, constructors, copies and
destruction to the correct registries. No db/export renames.

Empty-world construction/membership and camera device delegation are provided.
**No Windows run or presented frame is verified.** Missing toolkits are storage
reservations, not functional particle/lightmap/patch/collision implementations.

## Step 1: the 19 calls, in 00506CF0 order

Offsets below are hexadecimal **x86 ABI offsets**, followed by payload bytes.
They assume this exact attach order. E=engine, F=frame, A=atomic, G=geometry,
S=world sector, M=material, C=camera, L=light, T=texture, W=world, K=clump.
Four-byte rounding applies even to the 1/2-byte texture plugins. Native test
pointers widen; those fixtures do not validate x86 execution.

All 18 library attach/initialize/register entries below use original
zero-argument **stdcall**. The game's 006BBA20 and 006C34F0 remain its existing
cdecl definitions; zero-argument cdecl callers of the attach slots have the
same stack behavior. Actual registrations taking arguments use cdecl.

| # | Slot / plugin | ABI reservation | What is attached / what the game subsequently needs |
|---|---|---|---|
| 1 | 0058D0B0 World | E builtins: 4,48,4,8,4,0,12,0,12 bytes; C+184/28, A+74/8, K+2C/8, L+40/8; clump C+1A0/12, L+48/12; zero-size rights/mesh/native IDs | Native World/Camera/Light are librw core. Register native mesh, native-data, atomic/material rights. Game needs world membership, sector callbacks/BSP, native geometry and pipelines. Empty-world subset only; nonempty BSP/rendering/streaming remains absent. |
| 2 | 0061ECD0 RtAnimInitialize | E+188/0, ID 439 | Native animation subsystem is librw core; separate game scheme storage. Later needs keyframe streaming, interpolator/callback ABI, blend/copy/time operations. |
| 3 | 0056DAE0 HAnim | E+188/0; F+A4/8 | `rw::registerHAnimPlugin`. Facade frame ID starts at -1, hierarchy pointer at zero. Game needs hierarchy/node IDs, matrices, embedded interpolator layout, attach/update and animation adoption. Native hierarchy is NOT game-compatible. |
| 4 | 00587450 Skin | E+188/0; A+7C/4; G+60/4 | `rw::registerSkinPlugin`. Game needs skin getter, bone matrices/weights/index/split data, atomic hierarchy/type and skin pipelines. Those later slots/adoption remain open. |
| 5 | 00571FF0 MatFX | E+188/0; M+1C/4, A+80/4, S+88/4 | `rw::registerMatFXPlugin`. Game needs effect queries, environment frame/alpha, texture/effect ownership and UV transform matrices. Native payload pointers are never published as ABI payloads. |
| 6 | 0056F3C0 Lightmap | E+188/0; A+84/28, G+64/8, S+8C/28, W+74/8, M+20/20 | No librw lightmap plugin: reserve storage, success. Material defaults include white colour and unit scales. Game calls 0056EFF0/0056F2B0 render-style get/set and 0056F380 world lightmap query; those and texture/sample/UV ownership/rendering/streaming remain absent. |
| 7 | 00577060 MipmapKL | E+188/0; T+58/2 | No exact native plugin: reserve packed K/L, default 0FC0. Game needs texture streaming/settings; D3D9 bias policy not installed. |
| 8 | 005663C0 ADC | E+188/0; G+6C/12, S+A8/12 | `rw::ps2::registerADCPlugin`; separate original three-word tails. Game later needs native mesh/ADC streaming/fixups. No sector/native-data adoption is claimed. |
| 9 | 0056C1B0 Collision | E+188/4; A+A0/4 | No librw collision plugin: reserve storage, success. Game needs intersection/sector/atomic traversal, collision mesh/tree stream data, build/destruction policy. |
| 10 | 005816E0 PTank | E+18C/4; A+A4/4 | No librw PTank: reserve pointer storage. Later needs owning particle atomics, cluster buffers, flags/strides, lock/unlock and draws. |
| 11 | 0057C4A0 PrtStd | E+190/0; A+A8/4 | No librw PrtStd: reserve emitter pointer; success. Later class/property tables, emitter ownership, particle batches, update/render and callback execution remain absent. |
| 12 | 005784A0 PrtAdv | E+190/0, ID 306 | Not Rt2d despite its exported name: original registers PrtAdv class callbacks. No native counterpart. Class construction and advanced emission callbacks remain absent. |
| 13 | 005869E0 Random | E+190/16 | No native generator: independent 31-word additive state, allocator-owned; x86 payload holds base/front/back/end pointers. Seed uses the observed LCG and 310 warm-up outputs. 005867D0/00586860 supplied; not `std::rand`. |
| 14 | 005865E0 Patch | W+7C/44; S+B4/104 | No native patch toolkit: reserve storage, success. Sector tag=-1 and flags=3 defaults. Later patch/BSP conversion, control points, tessellation, streaming and ownership remain absent. |
| 15 | 0058C040 UserData | E+1A0/0; G+78/8, S+11C/8, F+AC/8, C+1AC/8, L+54/8, M+34/8, T+5C/8 | `rw::registerUserDataPlugin`; separate count/pointer tails. Game later needs array names/types/values/removal and streamed ownership. Native arrays cannot be cast to game arrays; those accessors/adoption remain absent. |
| 16 | 005665A0 Anisotropy | E+1A0/0; T+64/1 | `rw::registerAnisotropyPlugin`. Default 1; existing 00566530 now mirrors its byte outward, and texture import bridges direct writes inward. Native int plugin versus original byte is handled in the slot. |
| 17 | 005776E0 Morph | E+1A0/0; A+AC/12, G+80/12 | No exact native time/callback plugin: reserve storage, success. Game needs 005774B0 add-time and morph callbacks. Existing object slice handles some morph interpolation, not this plugin API. |
| 18 | 006BBA20 Game object properties | E+1A0/12; A+B8/8, S+124/8, M+3C/8, F+B4/8, L+5C/8 | Existing game code registers real constructor/copy/destructor identities. They receive facades and original offsets. This callback was mislabeled geometry registration; it is sector registration. Five stream slots retain callback identities and return the same offset. Actual plugin stream dispatch/adoption is not implemented by registration. |
| 19 | 006205C0 Compressed keyframes | separate 48-byte descriptor: ID 2, interpolation size 36, keyframe/custom size 24 | Store original callback identities through address resolution, never invoke them as native stream callbacks. Compressed codec/playback and stream adaptation remain absent. |

The later-needs column is not an asset-coverage claim. Skin/HAnim/MatFX,
lightmap, user-data, particles/PTank, random and morph have live API callers in
the inventory. No later direct KL/ADC/patch API caller was established by
this audit; those rows describe conditional streamed-data responsibilities,
not proof that PC assets exercise them. Their real reservation sizes/order
come from the original attach bodies regardless.

Then **006C34F0 -> 0061ED00** stores the game's ID 273 descriptor (36-byte
interpolation frames, 16-byte keyframes). Duplicate/invalid descriptor
registrations fail. Registration is not animation playback; original HAnim's
engine-open scheme-1 descriptor/freelist facade is not supplied by this subset.

New step-1 definitions: the 18 library entries above; 0061ED00; stream slots
0058F270, 00590430, 005915C0 (material, not clump), 0059CEF0, 006329D0.
Additionally supply correctly identified geometry register/stream slots
00590BA0/00590BD0. Existing 00590400 is corrected to sector registration.
Stream registrations retain game identities separately from native PluginLists.

Evidence: original exported attach bodies, registry operands/data from the
executable, original geometry destruction and WorldCreate, game 006BBA20 and
its callback bodies. `work/rwplugins-check/plugin-audit.txt` records caller
references. RW_PORT_OFFSETS.md is conservative and cannot disambiguate these
registries by variable names alone. No SDK source/headers were consulted.

## Step 2: startup objects / presentation

New definitions (all cdecl except existing stdcall CameraCreate):

* 0058CCB0: original 116-byte World facade, 136-byte empty leaf prefix plus
  sector plugins, original bounds/rings/render-order; private `rw::World`.
* 0058CC80: get sector-render callback. The supplied callback succeeds only for
  an actually empty leaf; nonempty geometry is rejected, not swallowed.
* 0058CC90: set the game's two-argument callback at World+6C (NOT atomic pipeline).
* 0058DD90 / 0058DDC0 / 0058DDF0: add/remove/get camera world. Native membership
  and ABI camera extension+12 mirror agree on x86.
* 0058DF50 / 0058DFA0 / 0058DFD0: add/remove/get light world. Native lists and
  original World+34/+3C light rings, Light+34 link and extension world agree.
* 0058CBD0: destroy tracked, detached **empty** world; reject live cameras,
  lights/clumps or nonempty/changed root rather than corrupt ownership.
* 00628E40: end-update through the facade callback. Existing 00628E50/00628F60
  now support the production D3D9/recording-device seam, synchronize frame,
  translate buffers, and delegate native librw camera operations. Valid
  CAMERA/CAMERATEXTURE target and ZBUFFER are checked; nested update and active
  camera destruction are rejected. End clears currentCamera for subsequent
  raster composition.

Librw's camera device callbacks return void; delegation success is **not** a
BeginScene/EndScene/Clear HRESULT guarantee. D3D9 checks cooperative level before
begin/clear. Upstream resize/reset behavior is not a completed boundary reset
policy. Recording fixture runs the same bridge with native camera/frame/raster
identities; it is not real Windows COM/GPU execution.

Existing rwstart presentation slots 006274B0 / 00627510 / 006276E0 and lifecycle,
image/raster slots are retained, not replaced by no-op success functions.
Source startup order matters: **the loading-clock presentation precedes camera
creation, particles and world/lights**, despite the requested conceptual order.
The new world/light/camera-update slots are later capabilities; an early
particle trap still prevents reaching those later initializers in this run.

## Step 3: remaining WinMain reach slots

The supplied WINMAIN_REACH.txt is a stale 33-RW-slot artifact stopping traversal
at game 00506CF0. A fresh HEAD-object inventory/source walk reaches **58** RW
traps, reproducing the task's count. The walk includes address-taken callbacks:
registration of a callback is not its execution. Logs: reach-before.txt and
reach-after.txt. Of those 58, **35 have definitions here; 23 remain**:

| Slots left | Why |
|---|---|
| 005784E0, 005785B0, 0057F970, 0057FA90, 0057FCF0, 0057CE10, 0057CE90, 0057CED0, 0057C770, 0057C9A0 | Real PrtAdv/PrtStd class/property storage and callbacks absent from librw. Tail reservations do not provide class instances. First direct particle initializer call is 005784E0. |
| 0057CFE0, 0057DBA0, 0057EDA0, 0057EF90, 0057F050, 0057BBD0, 0057C8B0, 00580160, 005821D0, 005823E0 | Batch/property traversal, emission/render callbacks and PTank lock/unlock need the above owning toolkit and cluster buffers. No success/empty-class stubs. |
| 0061F5E0 | Compressed-keyframe float quantizer used by the ID-273 reciprocal/compression callback 006C2AD0. No native counterpart; format/rounding compatibility and playback bridge not verified. Descriptor registration does not invoke it. |
| 0063A4D0 | DDS stream reader/native raster adoption still absent; later texture callback, not loading RAW clock. |
| 007C5F60 | Exported name RwResourcesSetArenaSize is misleading: filesystem-manager initialization, handle/callback/archive/default-table contract, not a safe success no-op. |

Other unresolved RW slots outside this 58-slot closure remain unchanged in
scope, including nonempty world/BSP streaming/rendering and plugin APIs.
Compressed registration also references eight original library callback slots;
they intentionally remain traps when invoked, not executable callback stubs.

## Expected next Windows run (source expectation, NOT executed)

Use `CSF_RW_TRACE=1` with `tools/dryrun.py --full`. After `RW startup
0x00627360 EngineInit`, expect `RW startup <address> <operation>` in the exact
table order: 0058D0B0, 0061ECD0, 0056DAE0, 00587450, 00571FF0, 0056F3C0,
00577060, 005663C0, 0056C1B0, 005816E0, 0057C4A0, 005784A0, 005869E0,
005865E0, 0058C040, 005665A0, 005776E0. Storage-only entries say so. Interleaved
`RW plugin id=... registry=... offset=... bytes=...` describe reservations;
registry values: E=0, F=1, A=2, G=3, M=4, L=5, T=7, K=9, C=10, W=11, S=12.

During game 006BBA20, expect RegisterPlugin traces 00626AE0, then
0058F210/0058F270, 00590400/00590430, 00591590/005915C0,
00623950/006329D0, 0059CEC0/0059CEF0 (each pair register then stream).
Next: `006205C0 RtCompressedKeyFrameRegister (descriptor only)`, then
`0061ED00 RtAnimRegisterInterpolationScheme` twice: compressed ID 2 and game
ID 273. No compressed callback executes just from those lines.

Assuming earlier game/NetZ initialization, device setup and RAW file open work:

1. 00627290 EngineOpen (`HWND=... (copied)`), 00626B30 SetSubSystem,
   00626B90 GetNumVideoModes, 00626BE0 GetVideoModeInfo (possibly many),
   00626C40 SetVideoMode; optional 0062B200 refresh, then 0062B170 MSAA or
   0062B2A0 dialog policy.
2. 00626D60 EngineStart; `RW startup D3D9 device=... (librw-owned)
   backbuffer=... format=... depthFormat=... windowed=... msaa=...`.
   Game property constructors execute now, on the ABI engine facade.
3. 00627770 CAMERA then ZBUFFER, with facade/native/dimension/format traces;
   0062B670 GetCurrentD3DDevice, 0062C110 GetCaps.
4. RELOJ.RAW: 00625060 ImageCreate, 006250C0 ImageAllocatePixels,
   00627770 NORMAL raster, 00633040 upload, 00625F10 destroy.
5. Context push 006275B0; trace `006274B0 RasterClear`, `00627510
   RasterRenderFast`; pop 00627650; `006276E0 ShowRaster`, then
   `RW startup Present HWND=... HRESULT=00000000`. Three successful
   presentations total, then RELOJ1/2/3 uploads. Push/pop have no trace lines.
6. Later `006290B0 CameraCreate`, zero-size CAMERA/Z headers, 00627710
   subrasters, frame creation/attachment. Subsequently the particle initializer
   should trap **UNRESOLVED 0x005784E0**. Do NOT expect world/light initialization
   traces before that barrier. Once particles are implemented, later traces
   include 0058CCB0/0058CC80 and 0058DF50/0058DFA0/0058DFD0; update paths trace
   00628F60 CameraClear, 00628E50 CameraBeginUpdate, 00628E40 CameraEndUpdate.

RAW-open failure skips this clock frame. Earlier indirect traps, game allocator
callbacks, asset/archive IO, mode/device failure or game errors can stop before
the predicted particle trap. A successful link is not first-frame evidence.

## Verification and files

Final command results are recorded in the verification addendum below.
Artifacts are under work/rwplugins-check/. Baseline COFF objects use git HEAD
versions of modified pre-existing RW sources/headers in /tmp, excluding new
plugins/world units; no production sources were reverted to obtain the baseline.

Changed: boundary/rw CMake integration; abi.h; life.cpp/.h; object.cpp/.h;
image.cpp; device.cpp/.h; new plugins.cpp/.h and world.cpp/.h; rw_globals.cpp,
rw_life.cpp, rw_object.cpp, rw_device.cpp and new rw_plugins.cpp tests; this
report. Hook DLL excludes standalone boundary code. librw is unmodified.

No PLAN/JOURNAL/queue/db/export changes, commits, Windows/game/verifier runs,
SDK source or original-library instruction hooks. No original-emulator
differential claim. First presented clock frame has no known missing RW slot
on its audited successful path, but real asset IO/device/presentation remain
Windows acceptance checks. Full menu/scene first frame is still blocked by the
particle/toolkit, filesystem and asset/native-data boundaries above.

## Final verification addendum

Completed with exit 0 on final sources:

* Instrumented native configure:
  `cmake -S . -B /tmp/csf-rwplugins-asan -G Ninja -DCSF_RW_CPU_SLICE=ON
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g'
  '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined'`.
  Build the seven `csf_rw_{cpu,life,image,object,device,toolkit9,plugins}_link`
  targets with `-j 8`, then run each with `ASAN_OPTIONS=detect_leaks=0
  UBSAN_OPTIONS=halt_on_error=1`: all pass. rw_globals.cpp is their shared link
  fixture, not a separate test. Logs: native-build-final.log,
  native-tests-final.log. The device fixture forces D3D9 platform selection for
  its recording camera update/clear path; actual Windows COM is not executed.
* New plugin/world fixture checks attach order, offsets/duplicate/frozen
  registration, first-descriptor allocation failure, scheme metadata, two
  complete lifecycles, RNG warm-up/output, anisotropy byte/native-int roundtrip,
  empty root defaults/rejection, camera/light membership, lists and cleanup.
  Arbitrary upstream OOM, populated toolkit payload copying/destruction and
  real game allocator/property callbacks are not validated by these fixtures.
* HEAD-object baseline:
  `.venv/bin/python tools/linktest.py -j 8 --objects
  work/rwplugins-check/before-objects.txt --out work/rwplugins-check/before.json`.
  **14798/14798 files, 15297 definitions, RW unresolved 205**.
* Final full-source run:
  `.venv/bin/python tools/linktest.py -j 8 --out work/rwplugins-check/after.json`.
  **14800/14800 files, 15336 definitions, RW unresolved 176**. No failed,
  duplicate, raw-address or assembly-address records. Other source-only groups
  unchanged: crt 2, game 83, netz 165, not-a-function 7, png 1.
  **39 new definitions**, 37 previously unresolved RW addresses removed, eight
  added unresolved callback identities (0056DB70, 0056E0A0, 0056E700, 0061F6D0,
  0061FF00, 0061FFB0, 00620060, 00620070). Thus the net reduction is 29, not 39.
  Exact-set check: set-check.txt. The two new true geometry registration slots
  have no source-only production references yet.
* `cmake --build build --target csf_hook csf_rw_d3d9_link csf_rw_plugins_link
  -j 8`: final DLL and both x86 probes link. These probes were NOT executed.
  Full and incremental logs: win-build-final.log, win-final-incremental.log.
* `cmake --build build --target csf_standalone -j 8`: links PE32 i386 GUI EXE,
  log standalone-build.log. Its final internal full-source linktest re-run
  has identical defined/unresolved/error sets to after.json. Expanded
  data/vtable/initializer inventory still emits **338 RW fatal traps**; this
  is distinct from the source-only 176. Librw D3D9 archive, not NULL, is linked.
* `.venv/bin/python tools/initreach.py --root 0x509250` on the newly generated
  standalone trap inventory: **23 RW traps**, 1217 source-reached functions;
  matches the independently recomputed after closure. reach-linked.txt.
  Before closure was exactly 58; all 58 are covered by the dispositions above.
* New-slot conventions checked against export/abi.json (abi-check.json);
  `git diff --check` passes; `git -C work/tools/librw status --short` is empty.

LeakSanitizer is disabled as in earlier jobs; no general leak-free claim.
No Windows/GPU/game/real-asset execution or original binary differential.
