# RtFS / DDS cutover

## Finding

The boundary now supplies **40 RtFS slots and 0063A4D0 DDS reading**. Five of
the RtFS slots replace the provisional implementations formerly in
`src/boundary/rwa/filesystem.cpp`; they are not additional independent
filesystems. The game still owns its archive/cache/loose-file policy. Its
`vfs:` callbacks call 00698350, and filename RW streams now dispatch through
that installed filesystem, rather than bypassing archives with host fopen.
No Windows/game execution is claimed. Verification results are recorded below.

## 1. Filesystems and address slots

All entries below take stack arguments with caller cleanup (**cdecl**), except
the no-argument 007C6010, 007C60C0, 007C6300, exported as **stdcall**. For the
unexported 007C5FA0 and 007C6CC0, the original bytes establish RET without an
immediate. 007C68A0 replaces its one stack argument and tail-jumps to engine
+10C (cdecl free); 007C6CC0 also takes one caller-cleaned stack argument.
007C5FA0 has no arguments (cdecl/stdcall have identical machine ABI there).

| Slots | Boundary operation |
|---|---|
| 007C5F60, 007C5FA0 | manager initialize/terminate |
| 007C6010, 007C6040, 007C6090, 007C60C0, 007C60D0, 007C6110 | capacity, name lookup/check, list head, device lookup, path-to-filesystem selection |
| 007C61A0 | set default; return the installed pointer, not the old default |
| 007C61B0, 007C61F0, 007C6220, 007C6250 | close, fread/fwrite element counts, CRT-origin seek |
| 007C62C0, 007C62D0, 007C62F0, 007C6300 | EOF, exists, abort, poll all live records |
| 007C6350, 007C63C0, 007C64B0, 007C64F0 | registration, open-on-system, flags open, mode-string open |
| 007C65C0, 007C6610, 007C66A0, 007C66D0 | strip first colon, bounded path join/separator conversion, 64-bit position EOF, registration events |
| 007C6710, 007C6780, 007C67D0 | device-name ownership, async callback completion, initialize caller-owned system |
| 007C6880, 007C68A0, 007C68C0 | disk record accessor, destroy record array, close disk record |
| 007C68E0, 007C6A50, 007C6B80, 007C6BC0 | disk read/write/seek/sync |
| 007C6CB0, 007C6CC0, 007C6CE0, 007C6D40, 007C6E90 | disk abort/status/exists/open/create |

Tracing 006971B0 and its original vtables establishes the installation order:

1. `win`: 006A01D0 calls 007C6E90 with two records, current-directory drive
   prefix (`GetCurrentDirectoryA`, then `cwd[2]=0`), name `win`; registers it.
2. `vfs`: 006A0020 creates four 72-byte records and a 92-byte system; its
   callbacks open/read/seek game scene-file objects (0069FDD0..0069FFD0).
3. `asyn`: 0069FC00 creates two 64-byte records and a 92-byte system. Its game
   callbacks forward to the system at 008BB404; forwarded accessors re-tag the
   underlying record's owner at +38. Those game callbacks remain game code.
4. 0069FDC0 installs the stored **vfs** pointer as manager default.

The manager does not auto-select the first registered system. Prefix lookup
includes the colon and is ASCII case-insensitive: 0063D832 installs 0063D690
at engine +F0. Name lookup uses ordinary strcmp at engine +E8, installed at
0063D812. A path without a matching registered prefix uses the default.
Consequently `win` is a system name, not the Windows device prefix: that
device is e.g. `E:`. No invented `win:` alias or archive extraction/search is
introduced. Disk joining follows original prefix + stripped path semantics;
in particular drive-relative versus rooted paths are not silently rebased.
Run with the game directory as CWD, as dryrun does. No new asset location,
environment variable, synthetic resource, or fallback content is installed.

### Layout and ownership evidence

Derived from exported pseudocode, game offset accesses, original vtables and
Capstone disassembly of `work/pc/CommXPC.exe`, not SDK declarations:

* Manager cells: 008BC7C4 default; C8 linked-list head; CC max systems; D0
  registered count; D4 initialized; D8 error; DC error callback. 007C5F60 takes
  max systems (the game passes -1), not resource-arena bytes. Windows also
  initializes the actual CRITICAL_SECTION at 008BEB40: 0050D1A0 inlines manager
  teardown and calls DeleteCriticalSection on that address itself.
* Common system prefix is **88 bytes**: next +00, count +04, kind +08,
  owned device string +0C, async +10, accessor +14, array destructor +18,
  reserved/game getter +1C, registration events +20/+24, operation table
  +28..+4C (open, close, read, write, seek, sync, abort, EOF, status, exists),
  eight-byte name +50. Game allocates **92** bytes and puts its record pointer
  at +58. Windows allocates **96**, stores sector bytes +58 and records +5C.
* Common record is **64 bytes**: 64-bit length +00, position +08, async +18,
  operation state +1C, status +20, operation +28, transferred/result +2C,
  completion callback +30 and user data +34, owner +38, reserved +3C.
  Windows records are **96** bytes (host-file identity +40, private pending
  storage +44..+5F); vfs records are **72** (game scene-file pointer +40).
  Asyn's own allocation is **64** and forwards to the underlying record.
  x86 sizeof/offsetof assertions compile in the production boundary; native
  tests widen pointer fields without pretending those are original x86 bytes.
* Status 1 is reusable/closed, 2 open, 3 pending, 4 error, 5 reserved. Pool
  reservation and close are serialized. Failed opens return their slot to
  status 1. Array/device/system allocations use the same engine allocator
  that the game's inlined teardown frees through engine +10C.

Disk bytes are real host files. Append does not truncate; read/write return
element counts; short reads, cached position and pool exhaustion are retained.
Windows open/exists retain the original 256-byte joined-path limit. The
completion helper returns the original record identity when no callback runs.
The host performs disk work synchronously; async-marked requests publish
completion at sync/status polling, with original operation/callback fields,
and a pending read does not advance the visible position until completion.
This is not an overlapped/worker-thread throughput implementation.

## 2. Streams

No additional stream-address slot was needed: every compiled game Open uses
filename type 2 (memory type 3 remains supported by existing fixtures). Existing
006298E0 now creates a private librw Stream adapter with a snapshot of native
FileFunctions. Manager initialization installs real manager-backed native
FileFunctions, also serving librw image codecs. The snapshot prevents a raw
host file opened before manager initialization from later being closed as an
RtFS record. Native virtual headers never escape to game code.

Existing 006293E0/00629530/00629700/00629810, 00629A80/00629AE0,
00629BE0/00629C70/00629C90/00629CE0 continue to provide read/write/skip/close,
chunk headers/find and typed word IO through the same sidecar. Filename
assets after manager initialization therefore reach the game-installed vfs;
archive precedence and loose-file fallback remain 00698350's decisions.
Pre-manager file streams and native memory streams remain usable.

## 3. DDS

**0063A4D0 cdecl(stream)** validates and reads the DDS magic/124-byte header,
pixel format, dimensions, mip count, pitches and payload lengths. Librw has
no DDS parser in this checkout. The boundary parses the container; unmodified
librw's D3D9 raster creation, native texture allocation, locks and destruction
own the raster/pixels. There is no D3DX loader or SDK dependency.

* DXT1..DXT5 are retained compressed, including odd block dimensions and all
  declared mips. DXT1 transparency is detected from valid pixels' block
  indices; the facade preserves the 1555/565 or 4444 class used by 00705830.
* Common 8/16/24/32-bit RGB/alpha-mask, luminance/alpha and P8 inputs convert
  to native BGRA/XRGB32 with mip levels. Opaque inputs retain the opaque RW
  format class rather than incorrectly selecting the game's alpha path.
  The conversion can change packed-input depth/format (e.g. 565 to XRGB32),
  not pixel colour or opaque/alpha classification.
* Compressed copies respect D3D9 lock row pitch; offline librw's compressed
  storage is contiguous and its reported pixel stride is not a block pitch.
* Invalid/truncated headers or data, invalid masks/mip counts, cubemap/volume,
  DX10, float/FourCC-vendor formats and 64/128-bit formats return null. No
  half-initialized facade is returned. GPU format-conversion fallback for
  unsupported DXT formats is not implemented; no GPU success is inferred
  from the offline backend. Nonstandard padded lower-mip layouts are not
  established. General upstream OOM/driver failure recovery is not proved.

## Expected next Windows run (static prediction, NOT executed)

With the user's already-passing camera/particles/world path, initialization
should pass 007C5F60, create/register win, vfs and asyn, and install vfs default.
This phase opens **no assets** itself (only queries CWD/disk-sector information).
Expected trace includes `RW startup 0x007C5F60 RtFSManagerOpen`.

Asset request order, only as far as static calls determine it:

1. Earlier WinMain requests (already before this stop): Config/Juego.cfg,
   Config/JuegoVer.cfg, Config/JuegoPla.cfg, Config/Punteria.cfg,
   Config/Red.cfg, Config/MultiP.cfg; then optional Patch.pak (unless the
   launch record selects the no-patch path). Video initialization requests
   MENUS\\TEXTURES\\RELOJ.RAW through game IO before RtFS initialization.
2. After sound/file-system setup returns successfully, 0050FBD0 constructs
   CMenuManager (005113E0 -> 00510860), which requests its configured font
   through controls +4C -> 00726EF0 -> 0073B290 -> 00698350. The binary default
   and the read-only retail Juego.cfg string are `menus\\Ingame_01.fnt`
   / `Menus/Ingame_01.fnt` (case/separators preserved by the relevant caller).
3. Font version 1/2/3 loaders request page textures in page-index order.
   Version 3 sets `<font directory>/Textures/`, names pages `<font basename>0`,
   `...1`, etc.; 00726B00 -> 006E2ED0 -> 00705DD0 tries DDS first via
   RwStreamOpen and this reader. For that font: first
   `Menus/Textures/Ingame_010.dds`, then subsequent pages if declared. A
   missing/unsupported DDS falls back to the existing 00705B40 image path.
4. 00510860 next calls 004A0B10, which opens `gfx/iconos.txt`; texture names
   subsequently come from that file, under `gfx/textures/`. Later menu/scene
   loading is configuration/resource-driven; there is no defensible fixed
   complete file order beyond these calls. Optional `framedump%03d.fd`
   probes/writes belong to 0050EB10's enabled debug branch, not normal assets.

These are logical game requests, not a promise that the host opens each loose
name: 00698350 checks the existing cache/archive chain before loose-file
fallback. Existing mounted archives can satisfy a request without another
host open. No hardcoded GlobalEK.pak mount has been invented by this boundary.
Use CSF_RW_TRACE=1; disk-boundary host opens print path/flags/result, and
successful DDS reads print dimensions/mips/DXT/alpha. A vfs-served DDS need
not produce a disk-boundary-open line because game scene IO owns that handle.

## Verification

Logs and the retained pre-change inventory are under `work/rwfs-check/`.

Native configure:

```
cmake -S . -B /tmp/csf-rwfs-asan -G Ninja -DCSF_RW_CPU_SLICE=ON \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g' \
  '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined'
```

Build/run targets `csf_rw_{cpu,life,image,object,device,toolkit9,plugins,particles,fs}_link`
with `-j 8`, `ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1`.
**All nine pass**, rerun after the final source edits; no ASan/UBSan errors.
The allocator-failure diagnostic lines in the older image/object fixtures
are intentional tests, not sanitizer failures.
rw_globals.cpp is their common fixture, not an independent executable.
The new temporary-directory fixture tests pre-manager streams across cutover,
host reads/write/append, failed opens and pool reuse/exhaustion, short reads,
seek/EOF/count semantics, async publication/callbacks, case-folded device
selection, caller-owned vfs callbacks, DDS through a synthetic archive
callback, DXT1..5 mip payloads, RGB565/L8/A8/P8 conversion and malformed DDS.
No original binary differential, real archive read, Windows/GPU execution or
leak-free claim is made. No game, smoke, verifier or SDK was used.

The source/export/binary calling-convention audit is repeatable with
`.venv/bin/python work/rwfs-check/audit.py` (log `abi-audit.log`). It checks
the 37 exported RtFS prototypes, the three unexported instruction sequences
described above, and DDS cdecl. The x86 production compile also exercises
the structure-size and offset assertions. `git -C work/tools/librw status
--short` is empty; `git diff HEAD --check` passes.

`tools/linktest.py -j 8`: retained before (`before.json`, `link-before.log`)
14868/14868 sources, 15460 definitions, RW unresolved **192**; after
14869/14869, 15496 definitions, RW unresolved **188**. No failed compiles,
duplicate slots, raw original-address references or assembly-address hits.
The four formerly unresolved compiled slots supplied here are 0063A4D0,
007C5F60, 007C61A0 and 007C6300. The other new slots close the RtFS callback
family; they were not all independently referenced in that before inventory.
Game 27, NetZ 154, not-a-function 4, PNG 1 remain unchanged; other undefined
symbols stay 50 in this pre-data/library inventory.

Windows cross-build checks, all **exit 0** (not executed on Windows):

* `cmake --build build --target csf_standalone -j 8`
  (`standalone-build-final.log`, rerun after the explicit DDS cdecl edit).
* `cmake --build build --target csf_hook -j 8` (`hook-build.log`).
* Additional `cmake --build build --target csf_rw_fs_link -j 8`
  (`windows-fs-link-final.log`), including the Windows critical-section fixture.

`file` identifies csf_standalone.exe, dinput8.dll and csf_rw_fs_link.exe as
PE32 Intel i386 binaries. The standalone's **generated-data/linker-input**
inventory still reports **350 RW fatal-trap slots** (includes data/vtable-only
references, unlike the 188 source-reference inventory). All 41 slots supplied
here are absent from `build/cutover/stubs.json`; successful linking is not a
claim that the remaining traps, asset contents or rendering work.

## Changes and remaining work

Added `src/boundary/rw/{fs.h,fs.cpp,dds.cpp}`, `tests/rw_fs.cpp` and this report;
changed RW CMake, life.cpp, image.cpp/.h and tests/rw_globals.cpp. Removed the
five-slot provisional `src/boundary/rwa/filesystem.cpp` in favor of the common
RtFS implementation. Librw remains unmodified; hook DLL excludes boundary
sources as before. PLAN/JOURNAL, db, queue and export are untouched.

Still open: Windows acceptance of actual game callbacks, archive contents and
next startup stop; genuine async throughput; unsupported DDS kinds above;
existing nonempty world/native-geometry/plugin/particle streaming and GPU
rendering boundaries. The game's inlined shutdown/later engine recreation is
not covered by the native fixtures (which call manager termination before
engine teardown). No first menu/scene frame is claimed.
