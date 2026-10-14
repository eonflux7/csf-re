# Slice 3: partial, 42/50 slots

**42 of the exact 50 allocated addresses are implemented**, not a complete
raster/device cutover. Raster tests run on a NULL engine with librw's existing
offline D3D9 raster codec. NULL's own raster callbacks assert (engine.cpp).
No librw files were patched; no SDK declarations were used. Slots remain
standalone-only; the hooked DLL excludes these sources.

## Implemented addresses

```
00566520 00566530 005870D0
00625060 006250C0 00625270 00625320 006254C0 00625680 006257E0 00625F10
00627430 00627490 00627570 006275B0 00627650 006276A0 00627710 00627770 00627810
006315B0 006315D0 00631650 006316E0 00631730 00631780 006317E0 00631800
006318E0 00631920 00631950 00631990 006319B0 006319C0 00632340
00633010 00633040 00633070
007C5680 007C5CA0 007C8D30 007C9050
```

Each definition has a caller-contract comment. Caller grep/packet evidence:
work/rw3-check/caller-*.packet and audit.json. Leading-zero grep initially
missed the short-spelled 00631730 reference; 006c1770 and its visitor 006c10b0
supply that contract. Source/export metadata was not renamed or edited.
006315B0 and 00632340 actually read one argument, although export/abi.json
records two/four: the extra values are previous calls' unpopped cdecl arguments.
Packet disassembly confirms one stack argument and plain RET for both.

## Bridges

* Stable Image/Raster/Texture/Dictionary ABI facades, selected-RW-allocator
  storage and private identity maps. x86 assertions: image 0x1C/pixels +14;
  raster 0x34/width +0C/format byte +23; texture 0x58/filter +50/refcount +54;
  dictionary 0x18. Images use a facade rather than the proposed access rewrites,
  because codecs and game callbacks expose images.
* Separate Raster/Texture/Dictionary ABI PluginLists. Constructors/destructors
  receive facade/tail/ABI offsets. Synthetic payloads test identity, offsets
  and ctor/dtor balance across reinitialization. No unallocated registry slots.
* Stop rejects live images, textures, rasters, contexts and non-initial
  dictionaries. Initial dictionary facade/plugins/map are released BEFORE
  native Stop releases its sidecar, extending slice 2's frame-only stop guard.
* TEXTURERASTER engine-state thunks translate pointers both directions. A device
  fixture records native values; NULL itself deliberately stores no state.
  No unimplemented engine device dispatch entries are populated.
* Images import direct pixel/palette/stride/ownership-flag writes. Borrowed pixels
  stay borrowed. Bound dimensions before upstream stride allocation arithmetic.
  Preserve an ABI path string because librw splits its copy into NUL components.
* Typed image/texture callback adapters, not pointer casts. Resolve explicit
  image extensions using librw's path helper: native read otherwise appends one.
* BMP/PNG writers return image/null, not native void: scoped FileFunctions
  capture collects native encoded bytes, then checks the real write AND close.
  Tight private copies preserve caller headers and bridge PNG's strided input/
  16-bit mutation. BMP RGBA normalizes to RGB; native BMP has incorrect row
  padding on BOTH read/write. Repair encoded row boundaries, leaving codec
  work to librw. Odd-width four-bit images, compressed/top-down BMPs fail.
* Native D3D rasterToImage wrongly applies a four-byte colour converter to
  one-byte palette indices. A palette fixture produced an ASan heap overrun
  before the bridge. Native allocation/locking and conv_8_from_8 now handle
  indices, plus a POD palette copy. Depth 4/8 palette/index round trips pass.
* Texture refcounts import game +54 writes. Dictionary destruction releases one
  reference while held textures survive detached. Final destruction removes
  facade/plugin/raster identities before native release. Packed filter/U/V
  addressing imports direct +50 writes. Names are bounded/terminated.
* Dictionaries maintain facade link rings, current/default identities, typed
  visitors. Add/remove do not add references; case-insensitive find does not
  add a reference, read/find does. Snapshot visitors permit removing/destroying
  the current texture and stop on null callback returns.
* Raster locks remember mip level, export temporary dimensions and restore them
  on unlock. Bound levels; reject nested locks and destruction while owned,
  parented or anywhere in the context stack. Subrasters inherit format/depth/
  type (native subRaster omits them), preserving facade parent chains.

## Plan corrections / remaining eight

| Address | Why not implemented |
|---|---|
| 00591560 | MaterialSetTexture needs slice 4's material facade/ownership map. Classified as a texture slot by its name; never cast game material to rw::Material. |
| 00625870 | RGBA-to-device-pixel conversion requires format/device dispatch (+50), not an invented NULL policy. |
| 00627470 | Scaled raster rendering has no public librw equivalent; needs device/compositing boundary. |
| 006274B0 | Raster clear needs current-context/device clear contract. |
| 00627510 | Native renderFast returns NULL-device failure; no success/no-op replacement installed. |
| 006276E0 | ShowRaster needs camera/device/window/flip policy, not successful NULL presentation. |
| 007FB000 | Named ImageSetHeight, but 006d6cd0/006d6fc0 pass a toolkit configuration from 008013C0, not an established Image. Belongs with toolkit slices. |
| 007FB010 | Same issue, +4 setter. |

The 50-slot allocation contains NO texture/image stream read/write, raster
setter, filter/address setter or ImageFreePixels slots. Direct header writes
exercise filtering/addressing; Destroy exercises freeing. Uninstalled ordinary
texture reference stream helpers prepare later integration and use slice 2's
memory stream facade. BMP/PNG FILE round trips are not image stream slots.

Seven undefined slice-2 addresses remain undefined: random/Rx/static-frame
plugin/device budget policies are not prerequisites for these fixtures. Archive/
filesystem handles (007C5F60/007C61A0) ARE prerequisites for real game asset IO,
which stdio tests do not establish.

Create supports NORMAL/TEXTURE and DONTALLOCATE headers. Actual game callers
also request CAMERA/ZBUFFER/CAMERATEXTURE (006d3250, 0070d5c0, 006fb5f0,
006d5c90); these fail explicitly until slice 7. Lock tests cover
1555/565/4444/LUM8/8888/888/555/PAL4/PAL8 with mips, NOT the requested
every-game-format/device acceptance.

## Verification

Native build/test, including instrumented librw:

```sh
cmake -S . -B /tmp/csf-rw3-asan -G Ninja -DCSF_RW_CPU_SLICE=ON \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g' \
  '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined'
cmake --build /tmp/csf-rw3-asan --target csf_rw_image_link csf_rw_life_link csf_rw_cpu_link -j 8
for t in image life cpu; do
  ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 \
    /tmp/csf-rw3-asan/src/boundary/rw/csf_rw_${t}_link
done
```

All three pass. LeakSanitizer is disabled as in slices 1/2; no general leak-free
claim. Tests reference/exercise all 42 definitions: images, borrowed pixels,
allocation failure, paths/formats/missing files/short output, odd-width BMP
padding/RGBA conversion, raster/image/palette round trips, mip locks, subviews,
contexts, plugin tails, engine pointer translation, texture references/filter/
address/name, dictionary add/find/remove/current/global visitation/current
texture destruction, reference stream round trip, held texture survival and
restart. Slice 2's custom lifecycle allocator-balance regression also passes.

Win32 probes:

```sh
cmake -S . -B build -DCSF_RW_CPU_SLICE=ON
cmake --build build --target csf_rw_image_link csf_rw_life_link csf_rw_cpu_link -j 8
```

Actual library linking and x86 assertions pass. No Windows executable run.
Logs/snapshots: work/rw3-check/. `git diff --check` passes;
`git -C work/tools/librw status --short` is empty.

Compiled full-source linktest, before and after:

```text
before: compiled 14594/14594 files standalone; 14891 functions implemented
unresolved functions: crt 2, game 92, netz 200, not-a-function 7, png 1, rw 322
after: compiled 14595/14595 files standalone; 14933 functions implemented
unresolved functions: crt 2, game 92, netz 200, not-a-function 7, png 1, rw 280
```

Both `.venv/bin/python tools/linktest.py -j 8 --out work/rw3-check/{before,after}.json`
snapshots have empty failed/duplicates/raw/asm maps. Set comparison verifies
that the 42 removed RW addresses are EXACTLY the new definitions. The baseline
started before image.cpp was created; existing life.cpp helper changes do not
add/change any original-address definition. These are compiled-slot counts,
not caller text greps. Exact-set/reference and calling-convention audits also
pass (conventions.log, set-check.log); calling-convention checking is not an
original runtime behavioral differential.

Full Win32 builds succeeded (exit 0):

```sh
cmake --build build --target csf_rw_image_link csf_rw_life_link csf_rw_cpu_link csf_hook -j 8
cmake --build build --target csf_standalone -j 8
```

win-build.log ends in linking dinput8.dll; standalone-build.log ends in linking
csf_standalone.exe. `file` identifies the DLL, standalone exe and three probes
as PE32 i386. The standalone is still a diagnostic/trap-linked exe: its expanded
data/vtable/initializer inventory contains 356 RW traps (different input set
from ordinary source linktest's 280); this is not a usable graphics cutover.
No build output was run on Windows.

A redundant combined build was subsequently interrupted after Ninja attempted
a large recompilation, rather than a no-op (win-final-build.log, exit 130).
Concurrent Ninja invocations had emitted a premature-log-recovery warning;
do not treat that interrupted repeat as verification. The earlier full builds
and final separate probe build completed successfully, on the same production
source. Avoid concurrent Ninja invocations sharing this build directory.

## Next slices / not verified

Slice 4 must use rasterNative/rasterFacade for camera buffers and
textureNative/textureFacade for material textures. Releasing native textures
directly bypasses the maps; route final ownership release through the facade.
Material setter remains a trap pending that lifecycle.
In particular, textureFacade currently supplies identity lookup, not automatic
export after an arbitrary native Material method changes refCount. Slice 4 must
extend outward synchronization/accounting for those ownership changes BEFORE
calling textureNative (which imports the game count). Native asset rasters not
created through this slice will also need an adoption/rollback helper; the
current rasterFacade is a lookup, not an untracked native-object adopter.

Slice 7 must replace fixed offline-D3D9 selection with actual device/platform
policy, supply camera/Z resources, rendering slots, RGBA conversion and engine
raster dispatch table. No GPU/device, Windows execution, original differential,
game or archive IO was available/run. Plugin streams still need explicit ABI
stream/tail translation; ordinary texture helpers do not implement it. Subraster
locks, automatic mip generation, palette locking, unsupported colour conversion,
malformed asset/stream robustness and general transactional upstream OOM are
not established. Arbitrary reentrant destruction, threading and pointer-reuse
ABA during visitors are not supported/verified.

## Files

Changed src/boundary/rw/{abi.h,life.h,life.cpp,CMakeLists.txt}; added
src/boundary/rw/{image.h,image.cpp}, tests/rw_image.cpp, this report.
No docs/PLAN.md, docs/JOURNAL.md, db/, queue/ or export/ changes.
