# Slice 7: partial boundary, NOT a device/startup cutover

Added **27/47 planned address definitions**, **1/5 deferred slice-3 raster
definitions**, and **one corrected slice-2 Rx getter** (29 definitions total).
This does **not** produce a first presented frame. Production still selects
NULL, discards the HWND, rejects camera/Z rasters, and does not bind/install
these helpers. No success/no-op presentation slots were added.

The task's new-files/append-only constraint prevents correcting `life.cpp`,
`image.cpp`, and `cmake/librw.cmake`. These are genuine prerequisites; adding
another HOOK for existing lifecycle addresses would introduce duplicates.
No slice-4 files/slots were changed.

## Address definitions

Planned slice 7:

```
00592690
0062B670 0062B720 0062B760 0062B7A0 0062B840 0062B970 0062B9A0
0062BA10 0062BB70 0062C060 0062C0A0 0062C0C0 0062C0F0 0062C110 0062D650
00633710 00633780 006337F0 00637E30 00638210 00638320 006383C0
0063CED0 0063CFA0 0063CFD0 0063D130
```

Deferred raster: `00625870` (six explicit packed-colour formats; other formats
return zero, not complete device-format conversion). Corrected slice-2 getter:
`005935D0`. Each definition has a caller-contract comment. Evidence:
`work/rw7-check/audit.json`, `caller-*.packet`, `dispatch.txt`, startup packets.
No db/export renames. Original conventions retained, including zero-argument
stdcall slots. Ignored trailing cdecl arguments are not invented parameters.
AddFragment remains variadic and accepts exactly one terminated fragment.

## Bridges

* Borrowed real-device binding (`bindD3D9Device`); native fixture is a typed
  recording backend, NOT a fake COM vtable. Windows adapter uses actual MSVC
  SDK D3D9 declarations. Device/caps getters fail until bound. Caps are a stable
  writable 304-byte snapshot preserving the game's +EC modification; this does
  not propagate changes into librw's private caps.
* Shader/declaration/index/stream COM handles stay raw. Translate two original
  16-byte streams; null streams zero stride/offset; useOffsets selects offsets.
  Shader creation returns BOOL, constants count float4 registers, draw uses
  D3D primitive numbers/signed base vertex, not RW's different enumeration.
* Transform snapshots are 16-float D3DMATRIX values, not rw::Matrix. Null sets
  identity; failed writes retain saved values; reset/rebind invalidates snapshots
  and the current immediate batch. Native Camera devView/devProj and external
  COM transform writes are not imported into these snapshots; integration must
  synchronize them before relying on camera save/restore.
* Low-level setters always forward, including equal values after external COM
  writes. **Native librw private-cache coordination remains open**; recording
  tests are not proof of a working mixed renderer. State writes are immediate
  rather than original deferred batching; audited water callers consume side
  effects, not incidental returns.
* Im2D dispatch +28/+2C/+30/+34 converts reciprocal Z at +C to librw D3D9's
  camera Z, retaining ARGB/UV and caller storage. Dispatch installation is an
  explicit helper; current lifecycle does not invoke it.
* Im3D retains original 36-byte POD position/normal/ARGB/UV through End; add
  mandatory XYZ|RGBA flags. Validate primitives/16-bit indices; reject nested/
  unmatched batches and the unestablished native LIGHTING extension. **Maximum
  10,000 vertices/indices**: unpatched D3D9 uses fixed buffers; Im3D has no
  overflow guard and Im2D silently skips oversized draws. Chunking up to the
  original 65,536 limit is not implemented.
* Translate original numeric render states to librw's reordered zero-based
  enumeration, raster pointers through slice 3 both ways, and three supported
  alpha comparisons. Bound values that index native tables. Unsupported states
  fail (including shade/fog-type/density/perspective/border). Current slice-2
  thunks still pass original ids directly and must be replaced by this helper.
* One-node Rx **callback carrier**, not an Rx graph executor. Stable original
  pipeline/node/private-data prefixes (x86 node private +14, callback +C).
  Create/Lock/Add/Unlock/Find/SetCallback/Destroy accept only the supported getter
  identity and one terminated fragment; unknown graphs/fragments fail. Synthetic
  descriptor name is private and not a recovered SDK definition.

### Rx contract / assumptions of slice 4

`0070A230` installs `0070A070`. The boundary carrier calls cdecl `(original
resource/mesh header, facade atomic, uint8 type, uint32 flags)`; only its first
two arguments are established by the water caller/implementation. The trailing
type/flags contract is an assumption for slice 4, not an original call-site
proof. Water ignores those additional cdecl words. `dispatchPipeline` supplies
game pointers, never a native Atomic or
InstanceDataHeader. Water reads index buffer +20, primitive +24, two streams
+28, useOffsets +48, declaration +4C, mesh count +1C; per-mesh base vertex +6C,
vertex count +70, start index +74, primitive count +78, stride 36.

Slice 4/6 must synchronize that old header and import atomic pipeline +70.
**No native instance-data conversion/adoption is supplied; 0058E680 remains
unresolved.** Geometry/material/frame ownership stays with slice 4. For cameras
assume audited old frame +4, framebuffer +60, Z buffer +64; slice 4 must import
mutable fields, translate sidecars and maintain native currentCamera through
begin/end. No old Camera is cast to rw::Camera here. Before Stop, detach/destroy
pipeline carriers and unbind; reset must rebind/invalidate these helpers.

## Plan errors / remaining work

* `005935D0`, called a filesystem getter in slice 2, is the node descriptor
  getter Water_InitRenderPipeline uses, not a FileFunctions table.
* `005A1A34/005A1B17` are D3DX boundaries, not RW implementations.
* Nine of the 47 rows are audio/OS: `007C6300 007DB650 007DB6F0 007DB920
  007DB960 007DE550 007F17C0 007F78E6 007F78EC`. Six already have rwa
  definitions in this tree; all left untouched.
* BeginUpdate `00628E50` and CameraClear `00628F60` are slice 4, not these 47.
  Historical EndUpdate `00628E40` and VideoModeInfo `00626BE0` are missing from
  the live inventory despite current source callers. No duplicate lifecycle/
  camera/mode hooks were installed.
* Remaining graphics rows: `0058E680 0059B990 0059BA20 006269C0 0062B170
  0062B200 0062B2A0 0062F950 0063A4D0`. World/composed shader matrices,
  device-request translation, pre-start display policy, texture usercall/context
  adaptation and DDS decoding are not implemented.
* Four transferred raster slots remain unresolved: `00627470 006274B0
  00627510 006276E0`. Native renderFast supports CAMERA → CAMERATEXTURE copy;
  startup instead draws NORMAL clock texture → CAMERA. Forwarding native
  renderFast would be wrong. Compositing/current-context clear/present/window/
  flip/reset policy needs a real bridge. Native void showRaster cannot prove
  HRESULT success.
* Separate CMake targets reference unmodified upstream sources:
  `csf_rw_d3d9_backend`, `csf_rw_d3d9_boundary`, `csf_rw_d3d9_link`. Last target
  is a **compile/link-only probe, not run**. Production intentionally still
  links NULL until lifecycle prerequisites are fixed. This is NOT engine open/
  start/device creation/reset implementation.

## Startup to first presentation (source-derived, not runtime verified)

The first normal present is **before camera creation/update**, not after Im3D.
Successful loading-clock asset path, cross-checked with game vtables:

1. Host initializes data/original initializers → game WinMain `00509250`.
   `005074E0` initializes heap (`006912A0`); WinMain creates/shows/focuses HWND,
   then calls menu/BDD setup `0050FBD0` → `00506F70`.
2. `00506F70`: **EngineInit `00627360`**, game allocator/flags 0/arena bytes;
   video `006B1F20` → `006B1B80` → resource screen `006DB7A0`.
   Screen vtable +10 is `006DBFA0`.
3. `006DBFA0` copies config, then +6C (`006DC000`) with **address of HWND stack
   argument**: **EngineOpen `00627290`**, **SetSubSystem `00626B30`** (0),
   dimensions +64 (`006DC600`), **GetNumVideoModes `00626B90`**, loop
   **GetVideoModeInfo `00626BE0`** and predicate +70 (`006DC1F0`), then
   **SetVideoMode `00626C40`**.
4. Pre-start +74 (`006DC250`): optional **refresh `0062B200`** (0), then either
   **multisampling `0062B170`** or **fullscreen dialog `0062B2A0`**.
5. `006DB9A0`: **EngineStart `00626D60`**, two **RasterCreate `00627770`**:
   CAMERA type 2/configured depth; ZBUFFER type 1/depth 0. Bind native device
   before next step, NOT after camera construction.
6. Post-start +78 (`006DC290`): **device `0062B670`**, **caps `0062C110`**;
   caches COM pointer at renderer +5C, clears caps +EC bit 80.
7. Finish `006DBDB0`: load `MENUS\\TEXTURES\\RELOJ.RAW` via `006DBA70`;
   **RasterCreate `00627770`** (NORMAL), **SetFromImage `00633040`**,
   **ImageDestroy `00625F10`**, **PushContext `006275B0`**, **Clear `006274B0`**
   (0), **RenderFast `00627510`** (clock x/y), **PopContext `00627650`**,
   **ShowRaster `006276E0`** (window 0, flags 0). First intended loading frame.
   Three clear/draw/present sequences, then remaining clock images loaded.
8. Only afterwards `006B1B80` constructs camera resources: `00728000` →
   `00726C70`, CameraCreate `006290B0`, subrasters/frame attachment. Later scene
   rendering uses slice-4 begin/end and immediate callbacks. Regular present
   `006DB810` calls `006276E0`, flags 1 iff renderer +28 is set.

If first RAW image fails, `006DBDB0` presents no loading clock. Another black
frame path `006DB850` uses RGBA pixel `00625870`, push/clear/pop, renderer +2C
(`006DB810`) three times; not proved to precede the clock path. This patch cannot
reach either presentation: NULL mode selection/camera raster creation and the
unimplemented modes/raster slots remain blockers. This is not a runtime trace.

## Verification

```sh
cmake -S . -B /tmp/csf-rw7-asan -G Ninja -DCSF_RW_CPU_SLICE=ON \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g' \
  '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined'
cmake --build /tmp/csf-rw7-asan --target csf_rw_device_link csf_rw_image_link \
  csf_rw_life_link csf_rw_cpu_link -j 8
for t in device image life cpu; do
  ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 \
    /tmp/csf-rw7-asan/src/boundary/rw/csf_rw_${t}_link
done
```

All pass with instrumented upstream. Recording test exercises all 29 definitions,
explicit Im2D/state dispatch installation, caps identity, COM args/failures/equal
state forwarding, snapshot invalidation, vertex conversion/counts/indices/batch
lifetime, raster pointer round trip, one-node callbacks/unknown fragments.
LeakSanitizer disabled as before. Initial UBSan run caught opaque-node function
pointer type mismatch; fixed signatures and reran full suite successfully.

No hardware/Windows runtime, real asset IO, original differential, or complete
cache-correct mixed rendering claim. Standalone `linktest.flags()` compile
device.cpp/im.cpp against actual ~/.xwin D3D9 headers (logs in work/rw7-check).

Compiled linktest (`.venv/bin/python tools/linktest.py -j 8 --out
work/rw7-check/{before,after}.json`):

```
before: compiled 14780/14780 files standalone; 15118 functions implemented
unresolved functions: crt 2, game 95, netz 229, not-a-function 7, png 1, rw 280
after: compiled 14782/14782 files standalone; 15147 functions implemented
unresolved functions: crt 2, game 95, netz 229, not-a-function 7, png 1, rw 251
```

Exact-set check proves the 29 removed RW addresses equal the 29 new HOOK
definitions; after failed/duplicates/raw/asm maps are all empty (set-check.log).

Windows cross-builds completed successfully (exit 0):

```sh
cmake -S . -B build -DCSF_RW_CPU_SLICE=ON
cmake --build build --target csf_rw_device_link csf_rw_image_link \
  csf_rw_life_link csf_rw_cpu_link csf_hook -j 8
cmake --build build --target csf_rw_d3d9_link -j 8
cmake --build build --target csf_standalone -j 8
```

The D3D9 probe compiled and linked the complete unmodified upstream D3D9
backend and boundary, not just declarations against a NULL archive. Final
repeat after per-slot comment additions also passed: native-final-tests.log,
win-final-build.log, after.log, standalone-final-build.log. `file` identifies
DLL/exe/device probes as PE32 i386. No Windows binary was run. No concurrent
Ninja invocations used the same build directory.

Standalone remains a diagnostic/trap-linked NULL executable: the expanded
data/vtable/initializer inventory has 327 RW fatal traps (different input set
from ordinary source linktest's 251), plus other regions. A successful link is
not a usable graphics cutover. `git diff --check` passes; librw status is empty.

## Files

Added src/boundary/rw/device.h, device.cpp, im.cpp; tests/rw_device.cpp; this report.
Only appended to src/boundary/rw/abi.h and CMakeLists.txt.
No PLAN/JOURNAL/db/queue/export edits; librw worktree unchanged.
