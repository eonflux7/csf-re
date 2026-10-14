# Slice 4: partial, 31/41 allocated slots + material setter

**31 of the exact 41 slice-4 addresses, plus slice 3's 00591560, are defined.**
The ten remaining addresses require the world/sector boundary allocated to
slice 6. They remain unresolved; no empty-world successes or casts of game
worlds to native worlds were installed. This is not a complete graphics cutover.

## Implemented addresses (32)

```
0058EB40 0058EEB0 0058EF00 0058EF90 0058EFE0 0058F060 0058F1D0
0058F300 0058F560 0058F590 0058F5C0 0058F740 0058FC60
00590670 00590B90 00590FC0 00591560
0059CBD0 0059CBF0 0059CC50 0059CE80 0059D000 0059D040
00628E50 00628E90 00628EC0 00628EF0 00628F60 00628F90
00629000 00629070 006290B0
```

Each definition has caller-contract comments. `work/rw4-check/audit.json` has
the caller grep results; the first caller of every allocated address has a
`tools/packet.py` packet there. Padded/unpadded address spellings were searched
case-insensitively. AtomicCreate/CameraCreate use zero-argument stdcall; others
use cdecl. LightGetConeAngle returns double through ST0. LightSetRadius
transports four float bits despite its exported integer spelling. No renames.

## Bridges

* Separate stable facades/maps and ABI PluginLists, selected-RW-allocator storage.
  x86 assertions: atomic 116/geometry +18/render callback +48; geometry 136/
  refcount +0E/material list +20/morph targets +5C; material 28/refcount +18;
  light 64/colour +18; camera 388/buffers +60/+64/view window +68/planes +94;
  clump 44/frame +4. The geometry registry's existing 40-byte reserved suffix
  is preserved, not interpreted as a native plugin header.
* Native fixtures widen pointers. Slice-2's four literal registry-prefix test
  expectations now use sizeof(facade); x86 retains the original sizes. Plugin
  constructors/copy/destructors receive facade identities/offsets, not native
  objects. Synthetic tails test copy and ctor/dtor balance.
* Translate geometry/material/morph owning headers. Share verified POD vertex,
  normal, triangle, colour and UV storage, not native headers. Morph parents and
  material arrays contain facade pointers. Unsupported structural count/storage
  replacements fail; in-place material-list entry changes transfer ownership.
* Import signed 16-bit geometry/material reference counts. Export native retains
  before another inward conversion can overwrite them. Atomic set-geometry
  self-assignment is a no-op; native releases before retaining. Route final
  atomic/geometry/material releases through facade ownership maps. Bound retain
  and clone counts to the facade refcount range.
* MaterialSetTexture retains BEFORE releasing, including sole-owner self-assignment:
  use native setTexture with a temporarily empty owner, immediately export its
  texture count, then release the old texture via slice 3. Native material
  destruction must not free a texture behind the facade/raster/plugin maps.
  Texture release preflight protects locked/parented/contextual raster owners.
* Maintain disjoint facade/native frame-object rings. Generic SetFrame updates
  sidecars, including direct +4 parent writes imported at later boundaries.
  Frame destruction detaches both identities. FrameSync imports owning objects
  before native sync, exports spatial results before game callbacks, and does
  not continuously redirty itself by reattaching the same native object.
* Native atomic interpolation is missing: compose librw vector math over the
  original interpolator fields, update local sphere/dirty flags, use target 0
  for invalid positive morph indices. World-sphere scaling remains slice 6's
  0058EC60; native librw does NOT scale the world radius.
* Native clump clone loses the original atomic clone's interpolated local sphere
  and dereferences a null root. Both original and native clone only atomics, NOT
  lights/cameras (0058FC60/0058F690). Compose
  native creation/Atomic::clone with translated frame hierarchy cloning, shared
  retained geometry, atomic render callbacks and ABI plugin copies. Include
  no lights/cameras; reject a null hierarchy. Destruction tears down each
  owner through the facade rather than recursively freeing sidecars behind maps.
* Snapshot visitors translate identities and stop on null; destroying/removing
  the current atomic/light is supported. Clump render propagates facade callback
  failures while continuing remaining members (native render returns void).
  NULL default atomic rendering fails rather than pretending to execute a GPU
  pipeline; arbitrary original pipeline pointers are never cast to native ones.
* Bridge light constructor cone zero versus native +1; preserve colour alpha
  (native setColor is RGB only), dirty frame for radius changes, enforce original
  [0, pi/2] cone setter range, saturate finite out-of-domain getter cosines before
  native acosf. Promoted native float angle is not last-bit original equivalence.
* Camera original buffer/view/matrix/frustum order is preserved. Translate
  borrowed rasters, clip/projection/window, facade-only reciprocal window and
  direct spatial POD writes. Export native frustum/view results after frame sync.
  Projection is limited to 1/2; positive nonzero view windows/valid headers are
  assumed, not arbitrary malformed-header compatibility.
* Camera BeginUpdate dispatches via its facade +18 callback and a typed native
  adapter. NULL camera begin/clear callbacks are intentionally void no-ops in
  librw, unlike asserting NULL raster callbacks. Default begin performs facade
  frame sync then native begin/current-camera selection; clear forwards colour,
  mode and translated buffers. Handle/null returns implement this explicit NULL
  policy. Default begin/end/clear reject non-NULL platforms until slice 7 can
  report actual device failures. Camera owns neither frames nor buffers.
* Engine Stop now rejects all live owning-object maps.

## Plan corrections / ten unresolved addresses

| Address | Missing prerequisite |
|---|---|
| 0058CC90 | Not an atomic pipeline setter: stores +6C, while AtomicCreate/default render use +70. 00705F90 forwards the object to WORLD-light removal; 00706040 installs two-argument world-sector callback 00705FF0. High-confidence world-sector render-callback setter; needs World facade. |
| 0058CF30 | World material list +10/+14; no native world material list or established facade. |
| 0058CF80 | World light enumeration/membership rings. |
| 0058DD90 / 0058DDC0 / 0058DDF0 | World camera add/remove/get and original plugin membership storage. |
| 0058DEF0 | Visible sectors consumed by 006BC970/006BC8F0; librw has no sectors/BSP. Empty enumeration is not a bridge. |
| 0058DF50 / 0058DFA0 / 0058DFD0 | World light add/remove/get and original membership storage. |

The plan puts World/sector construction/serialization in slice 6 but these
dependent boundaries in slice 4. Move them or bring actual world/sector work
forward. GetNumLights (0058F590) actually reads ONE cdecl argument; export's
spurious second is caller stack leftovers. Extra cdecl caller args remain safe.

No GeometryCreate, ClumpCreate, MaterialCreate/Destroy, geometry lock/unlock or
mesh construction slot is allocated here. Private construction helpers support
fixtures/future stream integration, not additional original-address definitions.
Native streamed-graph adoption, mesh-header/material-pointer translation and
original built-in plugin payloads are still open. Light/camera clump membership
currently lives in sidecars, not invented original plugin extensions. No direct
membership-offset proof was established; allocated visitors/count/removal slots
translate native membership, rather than publishing native object pointers.

## Verification

```sh
cmake -S . -B /tmp/csf-rw4-asan -G Ninja -DCSF_RW_CPU_SLICE=ON \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g' \
  '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined'
cmake --build /tmp/csf-rw4-asan \
  --target csf_rw_object_link csf_rw_image_link csf_rw_life_link csf_rw_cpu_link -j 8
for t in object image life cpu; do
  ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 \
    /tmp/csf-rw4-asan/src/boundary/rw/csf_rw_${t}_link
done
```

All four pass, with instrumented librw. The object test exercises all 32 new
definitions: construction/allocation failure/destruction, clone/root identity,
null-root clone rejection, current-member destruction, callback early stop/failure,
original-layout geometry/material ownership and native count synchronization,
sole-owner texture replacement, POD buffers/morph interpolation, light cone/
alpha/radius, camera clip/window/projection/frustum, begin callback failure,
borrowed raster conversion with native clear callback fixture, native sphere
transform, direct parent import, plugin copy/dtor balance, stop guard/restart.
Borrowed buffer conversion uses a NORMAL offline raster: CAMERA/ZBUFFER creation,
GPU clearing and NULL pixel clearing are NOT established. Slice 3's custom
lifecycle allocator-balance regression still passes. LSan remains disabled;
no general leak-free or transactional upstream OOM claim.

Logs/snapshots: `work/rw4-check/`. Standalone flags come from
`tools/linktest.flags()`; x86 assertions compile. All checks below completed.
No Windows executables or game have been run.

Full-source compiled slot inventory:

```sh
.venv/bin/python tools/linktest.py -j 8 --out work/rw4-check/before.json
.venv/bin/python tools/linktest.py -j 8 --out work/rw4-check/after.json
```

```text
before: compiled 14780/14780 files standalone; 15118 functions implemented
unresolved functions: crt 2, game 95, netz 229, not-a-function 7, png 1, rw 280
after: compiled 14781/14781 files standalone; 15150 functions implemented
unresolved functions: crt 2, game 95, netz 229, not-a-function 7, png 1, rw 248
```

Failed/duplicates/raw/asm maps are empty. Set comparison verifies EXACTLY the
32 definitions above removed from unresolved RW. The final fresh-source
`build/cutover/base.json` inventory also has identical defined addresses and
empty error maps (`link-set-check.log`). The larger source baseline than slice
3's report includes unrelated functions already present in this worktree;
the RW baseline remains the requested 280. Reference/exact-set/convention
audits pass (`set-check.log`). Conventions are not a runtime differential.

Actual Win32 linking and x86 assertions pass, exit 0:

```sh
cmake -S . -B build -DCSF_RW_CPU_SLICE=ON
cmake --build build --target csf_rw_object_link csf_rw_image_link \
  csf_rw_life_link csf_rw_cpu_link csf_hook -j 8
cmake --build build --target csf_standalone -j 8
```

`win-final-build.log` includes linking dinput8.dll and all four probes;
`standalone-final-build.log` ends in linking csf_standalone.exe. `file` identifies
DLL, standalone and object probe as PE32 i386. The initial long Win32 build
picked up object.cpp while it was being edited, then tried linking a newer test
against that stale archive and failed with three missing symbols. The subsequent
serial fresh-source rebuilds above passed; that initial log is not verification.
No concurrent Ninja builds shared build/. The standalone's expanded data/vtable/
initializer inventory still has **324 RW fatal traps**, different from ordinary
source linktest's 248; it is still a diagnostic executable, not usable graphics.

`git diff --check` passes and `git -C work/tools/librw status --short` is empty.

## Following slices / not verified

**Slice 5:** use these facades, ABI PluginLists and private conversions. Preserve
facade plugin offsets/identities. Route material/texture releases through the
facade and export native count changes before inward conversion: native MatFX
methods freeing textures directly are unsafe. Add outward/adoption helpers for
new native plugin owners before using them. Do not interpret original material
pipeline/reserved geometry suffix as native pointers. User-data/MatFX/HAnim/skin
payloads are not supplied; frame/clone copy callbacks provide bridge points.

**Slice 6:** World/sector/BSP facades and ten remaining slots, streamed graph/
mesh/material adoption with rollback, original light/camera clump extensions,
and 0058EC60 radius scaling (native radius ignores scaling). Construction helpers
are not asset adoption or game archive IO.

**Slice 7:** replace NULL-only camera begin/end/clear policy with checked device
dispatch/failure returns; CAMERA/ZBUFFER/CAMERATEXTURE rasters via slice 3;
native device pointers internally, facade pointers in engine globals. Supply
0058E680 default atomic render slot and original RxPipeline/native pipeline
policy; never cast game +70 pipeline pointers. No D3D9/GPU/window/presentation
execution is verified. Reentrant graph destruction, pointer-reuse ABA during
visitors, external-to-clump frame cloning and threading are not established.

## Files

Added src/boundary/rw/{object.h,object.cpp}, tests/rw_object.cpp, this report.
Changed src/boundary/rw/{abi.h,cpu.cpp,life.h,life.cpp,image.h,image.cpp,CMakeLists.txt}
and tests/rw_life.cpp. No PLAN/JOURNAL/db/queue/export changes; librw is unpatched.
