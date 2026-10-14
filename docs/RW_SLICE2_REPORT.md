# Slice 2 — partial: 30 newly implemented of the planned 39 slots

Finding: the 39-slot allocation is not the requested set of memory/freelist and
filesystem primitives. It contains an Rx node-definition getter and audio-object
enumeration. There are **no freelist alloc/free or malloc/free function slots** in
that allocation. I did not turn these mismatches into no-op success stubs.
Two of the 39 (007DB820/007DE590) are already defined in the existing RWA adapter;
I did not duplicate them. Thus 32/39 currently have definitions, seven remain
undefined; this job newly installs and tests 30, not 39.

## Implemented

- Six lifecycle entries: 00627360, 00627290, 00626D60, 00626CE0, 00626E30,
  00626EC0. Explicit NULL Init/Open/Start/Stop/Close/Term replaces CPU lazy startup.
- Six registries: 0058F210, 00590400, 00591590, 0059CEC0, 00623950, 00626AE0.
- Four mode/subsystem entries: 00626AB0, 00626B30, 00626B90, 00626C40.
- Eleven stream/chunk entries: 006293E0, 00629530, 00629700, 00629810,
  006298E0, 00629A80, 00629AE0, 00629BE0, 00629C70, 00629C90, 00629CE0.
- Three remaining CPU helpers: 0063CE30, 007D9D40, 007D9E30.

All are standalone-only address slots, using export/abi.json conventions. The
existing 36-slot CPU regression test remains separate from the new test, which
references and exercises all 30 new slots. Nothing installs over library code in
the hooked game.

### Bridges

- The original four-entry allocator table is malloc/free/realloc/calloc;
  librw's is malloc/realloc/free/mustmalloc/mustrealloc. Convert signatures/order,
  preserve the game table in the engine facade, and use it for native allocation.
- Publish an ABI engine facade through CSF_GLOBAL at 008B68C0, not rw::engine.
  Its x86 prefix is 300 bytes (original executable registry metadata). Render
  state dispatch at +20/+24 and memory callbacks at +108..+114 are populated.
  Other dispatch fields are deliberately not invented: later object/device
  slices still need to populate them.
- ABI plugin registries are **separate librw PluginLists**. Native object
  registries cannot provide game offsets/callback pointers. Original registry
  prefix sizes from executable metadata: atomic 116, geometry 136, material 28,
  light 64, frame 164, engine 300. Frame/engine widen pointers for native tests
  only; x86 layout assertions remain in force. Offset rounding is four bytes.
  Registration is rejected except between Init and Open; duplicate IDs and
  allocation failure return -1. A bounded descriptor allocator adapter prevents
  librw registerPlugin's unchecked-null allocation from crashing this slot.
- ABI constructor calls check null results and roll back completed constructors.
  Engine and frame callbacks receive facades and ABI offsets, never sidecars.
  Frames now allocate their plugin tail and invoke its constructors/destructors.
- Stream handles are opaque facade identities: no compiled game caller in this
  slice dereferences their internals. Native virtual Stream objects remain
  private. Filename streams use StreamFile; memory streams use StreamMemory.
  Memory writes grow with the selected RW allocator and transfer their buffer
  on Close(output), or free it on Close(null). Read buffers are borrowed.
- Bridge byte-count read versus handle/null writes, int32 byte counts and
  handle/null array reads, short headers, memory EOF/uint32 arithmetic overflow,
  bounds-checked memory skip, and engine-close rejection while streams are live.
  Unsupported stream types fail, rather than treating a callback/FILE* as a name.
- Versioned header writing uses the explicit caller version AND build (the sole
  game caller supplies version 225282/build 55), not librw process globals.
  Header output is 20 bytes; the unobserved isComplex member is zero. Find leaves
  the stream at payload and permits null size/version outputs.
- Quat rotation converts degrees to librw radians. Matrix-to-quat uses a local
  copy because librw's getter is non-const. Object/frame attachment maintains the
  facade list and dirties the native frame; future owning-object sidecars must
  additionally attach their own native object.
- NULL lifecycle still constructs inactive platform drivers. Their close
  callbacks leave default pipelines allocated, and PS2 caches two pipelines.
  Stop releases those through librw destroy methods and clears PS2 caches. A
  custom allocator fixture found four outstanding allocations per subsequent
  lifecycle before this bridge; it now asserts zero outstanding allocations.

## What the plan got wrong / not installed by this RW slice (9)

| Address | Allocated name | Finding / remaining work |
|---|---|---|
| 005867D0 | RtRandom | No librw random counterpart; independent, specified seed/range policy still needed. |
| 00586860 | RtRandomSeed | Same; not replaced with arbitrary std::rand behavior. |
| 005935D0 | RwOsGetFileInterface | 0070a230 passes it to RxLockedPipeAddFragment and dereferences its first field as a node name. This is an Rx node definition, NOT a file-function table; belongs with Rx policy. |
| 00626C90 | RwEngineGetTextureMemorySize | Native budget/query policy absent; caller 006dc750 consumes bytes as a hardware capability. Not a made-up NULL memory size. |
| 006329C0 | RwFrameSetStaticPlugins | 006bf020 switches the policy around environment streaming; not safely equivalent to a plugin-registration toggle. Needs a stream/static-plugin policy. |
| 007C5F60 | RwResourcesSetArenaSize | Caller 006971b0 uses (-1) as a prerequisite to filesystem creation. Its arena identity is suspect and not verified here; requires filesystem boundary audit. |
| 007C61A0 | RtFSManagerSetDefaultFileSystem | Needs the game filesystem-handle/callback layout and native FileFunctions adapter, including archives. stdio alone does not implement this contract. |
| 007DB820 | _rwGetNumEngineInstances | 00712ab0/00710d80 pair it with audio-object visitors, not graphics engines. Already defined as is_open by boundary/rwa/adapter.cpp. Do not bind to rw::Engine::state. |
| 007DE590 | RwFreeListForAllFreeLists | Visitors include 007128a0, which accesses audio objects at +30/+40/+50. Already defined as visit_all by the RWA adapter. This is not a graphics freelist iteration. |

The last two have direct caller/visitor evidence, but no formal API rename is
claimed. db/, export/, queue/, docs/PLAN.md and docs/JOURNAL.md were left untouched.
The inventory should be corrected by the orchestrator, not silently regenerated
from the reduced unresolved set.

## Verification

- Native NULL build and actual library link via CMake, not just unresolved-slot
  suppression. ASan/UBSan tests use **instrumented librw too**:

  ```sh
  cmake -S . -B /tmp/csf-rw2-asan -G Ninja -DCSF_RW_CPU_SLICE=ON \
    -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
    '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g' \
    '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined'
  cmake --build /tmp/csf-rw2-asan --target csf_rw_life_link csf_rw_cpu_link -j 8
  ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 \
    /tmp/csf-rw2-asan/src/boundary/rw/csf_rw_life_link
  ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 \
    /tmp/csf-rw2-asan/src/boundary/rw/csf_rw_cpu_link
  ```

  Both pass. Test cases include lifecycle order/reinitialization; registry freeze,
  duplicate/failed allocation; engine/frame callback identity and rollback;
  custom allocator table mapping and balance; borrowed/growing/transferred memory
  streams; temporary filename streams; missing file; truncated headers 0..11
  bytes; uint32 read overflow; skip bounds; chunk version/build/find and int/float
  round trip; frame attach/detach; quaternion degree conversion. LeakSanitizer
  itself cannot run here (fatal error under ptrace); it is disabled as in slice 1.
  No general heap-leak-free claim is based on ASan. Allocator balance is an
  independent assertion in the custom allocator fixture.
- `git diff --check`: pass. `git -C work/tools/librw status --short`: empty.
- Offline caller packets and the 39-slot implemented/calling-convention/caller
  audit are in work/rw2-check/ (scratch). The audit checks that all 30 installed
  addresses are referenced by tests/rw_life.cpp.
- A separate source/export check passes calling conventions and argument counts
  for all 66 installed RW slots (36 old + 30 new). This does not establish return
  types or callback signatures; log: work/rw2-check/abi-check.log. I did not run
  rw_survey.py's main against the reduced set: it rewrites the original inventory.
- Win32 compile and actual librw link both pass:

  ```sh
  cmake -S . -B build -DCSF_RW_CPU_SLICE=ON
  cmake --build build --target csf_rw_life_link csf_rw_cpu_link csf_hook -j 8
  ```

  `file` identifies both link probes as PE32 i386 executables and dinput8.dll as a
  PE32 i386 DLL. An incremental rerun recompiles both final boundary sources and
  relinks both probes; log: work/rw2-check/win-final-build.log. No Windows binary
  was executed.
- Full standalone compile: `.venv/bin/python tools/linktest.py -j 8 --out
  work/rw2-check/after.json`. Before/after:

  ```text
  before: compiled 14389/14389 files standalone; 14657 functions implemented
  unresolved functions: crt 2, game 95, netz 185, not-a-function 7, png 1, rw 352
  after:  compiled 14390/14390 files standalone; 14687 functions implemented
  unresolved functions: crt 2, game 95, netz 185, not-a-function 7, png 1, rw 322
  ```

  Both snapshots have empty `failed` and `duplicates`, and zero raw-image address
  hits. Set comparison verifies the 30 removed RW slots are **exactly** the new
  planned slice-2 definitions. These counts differ from the old RW_PORT baseline
  because this worktree already contains CRT/NetZ/RWA progress.

  Baseline methodology: the first full `linktest.py -j 8` started before editing,
  but reached cpu.cpp after its three new helper definitions were added. For the
  corrected baseline, I separately compiled cpu.cpp from task-start HEAD
  ac5ad1c270e5091d54a937c332379f209d879ce7, using linktest.flags(), and inventoried
  the full compiled COFF set with that original CPU object substituted and life
  excluded: `linktest.py -j 8 --objects work/rw2-check/before-objects.txt --out
  work/rw2-check/before.json`. All other sources are unchanged by this task.
  This is a compiled-object baseline, not a text-derived count. The initial run
  also had zero compilation failures. Logs/snapshots are under work/rw2-check/.

## Not verified / still open

This is a partial CPU boundary, **not a complete usable graphics cutover**.
No game, original-library differential run, Windows execution, D3D9 device, archive
filesystem, native asset streaming, or rendering was available/run. Standalone
flags/Win32 linking are not x86 runtime verification.

OOM in librw's own Init/Open/Start internals is still not generally transactional:
upstream contains unchecked allocations and must-allocate paths. Fixtures cover
registry descriptors, memory-stream growth, and frame creation/constructor
rollback, not arbitrary native lifecycle OOM. The original Init arena quota is
not implemented; only the game's flags=0 NULL path is supported. Memory-write
fixtures assume a newly owned output buffer, not a caller-supplied initial buffer.
isComplex behavior and pathological >2GB stream offsets are not established.
Callbacks are single-threaded and reentrant object destruction/mutation is not
established. Other engine dispatch slots and native owning-object attachment must
be supplied before their callers can execute.

## Slice 3 dependencies

Use the explicit lifecycle (no lazy bootstrap), shared facade identity, selected
allocator and stream conversion helper. Extend the ABI registry set for
raster/texture/dictionary plugin tails; never publish their native PluginList
offsets. Allocate actual facade prefixes plus those tails and pass game pointers
to callbacks. Add raster conversion for engine render-state TEXTURERASTER; the
current +20/+24 thunks cannot treat a future facade Raster as a native Raster.
Install required engine raster/texture dispatch fields only with those slots.
Destroy/clear facade ownership maps before native Stop destroys textures and
dictionaries; extend the live-object stop guard beyond frames. Fix filesystem
handle/archive policy before claiming real asset IO. Keep plugin stream callback
translation separate from native PluginList::streamRead/Write (those currently
expect native Stream callback pointers).

## Changed files

src/boundary/rw/life.h, life.cpp, cpu.cpp, CMakeLists.txt;
tests/rw_life.cpp, rw_globals.cpp, rw_cpu.cpp; this report.
