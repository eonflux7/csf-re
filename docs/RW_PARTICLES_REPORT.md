# Particle boundary, slice 8 — partial delivery

## Finding

The first particle initializer has a real class/property-table boundary now:
**005784E0 is implemented, not an empty-class/success stub**. All class/property
slots requested in step 1, and both previously unclassified callbacks, are
installed. Emitter/batch ownership and the small update callbacks are implemented;
PTank lock/unlock adapt its binary layout. **This is not the completed particle
port.** The large simulation callbacks, owning PTank creation and particle draws
remain fatal. No particle rendering or Windows execution is claimed.

Only unchanged MIT librw, original executable disassembly and game callers were
used. No SDK headers/source, export edits, database edits or library instruction
hooks. Boundary code is standalone-only and excluded from the hook DLL.

## Slots, in requested order

All 27 definitions are cdecl at their original addresses. Available export
conventions and the executable's caller-cleaned returns were checked; audit:
`work/rw8-check/abi-check.json`. 5785B0's five exported arguments include three
stale stack words: the executable consumes only the first two.

### Step 1: startup classes, properties, callbacks, ownership

Implemented:

* **005784E0**, **005785B0**, **0057F970**, **0057FA90**: advanced/standard
  emitter and particle class construction, schema-based or flag-ID class reuse,
  refcounts, property ownership and copied default callback sets.
  **5785B0 creates a particle class**, despite its database EClass name.
* **0057CE10**, **0057FCF0**: owned callback-array replacement, preserving old
  callbacks on allocation failure; emitter sets have 11 entries, particle sets 7.
* **0057CE90**, **0057CED0**: property-table acquire/release, object stride,
  borrowed getter (returns the class, writes the table pointer, no acquire).
* **0057C9A0**, **0057C770**, **0057C8B0**: four-byte property size rounding,
  table interning by ID/size irrespective of order, offset end sentinel,
  refcounts, missing-property result -1 and final release.
* **0057CD60**, **0057FC60**: class release, callback storage and table cleanup.
* **0057ED00**: initializes the actual standard emitter defaults/flags.
* **0057F0B0**: positive emitter count requires property 6; nonzero tank count
  invokes its atomic's +48 callback with facade identity and propagates failure.
* **00580100**, **00580A10**: emitter allocation/initialization and destruction.
* **0057C220**, **0057C290**: atomic emitter presence/borrowed extension getter.

The PrtStd atomic reservation now constructs/destroys an owned emitter ring,
instead of copying pointer bytes. Populated particle/PTank clump cloning is
rejected **before** allocation: upstream PluginList::copy ignores callback return
values, so returning null from a plugin copy callback alone is insufficient.
EngineStop rejects live particle allocations/classes/tables before losing their
allocator. Two complete native lifecycles exercise cleanup.

### Step 2: small callbacks, batches and PTank descriptors

Implemented:

* **0057EDA0**: conditionally releases the standard emitter's owned texture.
* **0057EF90**: stores borrowed frame LTM, advances emitter time, applies the
  original single-wrap constant tank rotation update and dirty bit.
* **0057F050**: deterministic next-emission scheduling using the particle's
  wrapping 32-bit LCG, not the unrelated engine random plugin. NaN comparison
  behavior is included in the differential fixtures.
* **00580160**: active circular-batch traversal, caches next before callback,
  oldest-first order, requires the callback to return that exact batch.
* **00580230**: capacity-checked inline batch allocation and inactive-batch reuse,
  particle-class initialization callbacks, active-ring insertion.
  The original **discards a batch initializer's failed return** on both allocation
  and reuse; that quirk is preserved and differentially tested.
* **005802A0**: emitter particle-class/batch-size setter, previous batch cleanup,
  old/new class ownership. It is not a multi-texture setter.
* **005821D0**, **005823E0**: every single cluster selector (1..256), absent
  cluster zero descriptor/failure, interleaved stride override, write-lock dirty
  masks and unlock transfer. The original combined-mask quirk (supported bit but
  no selected descriptor) is preserved without indexing a buffer by that mask.

Still fatal, **not implemented**: **0057CFE0** (standard particle update),
**0057DBA0** (standard emission), **0057BBD0** (sphere emission). Their bodies
are 3003, 4447 and 817 bytes respectively, intertwining lifetime/compaction,
per-property RNG/interpolation, transforms and several PTank cluster paths.
The small scheduling fixture is not a fixture proving these simulation kernels.

### Step 3: rendering

**No new particle draw backend.** PTank creation, format getter, destruction and
emitter/tank binding (**00582400**, **005821B0**, **005821A0**, **00581250**) remain
fatal, as do PrtStd atomic create/update (**0057C5D0**, **0057C2D0**) and particle
world render setup (**005865A0**). Therefore real particle output is not reachable
through this partial boundary. F0B0 correctly forwards to an installed facade
render callback; a recording callback in the tests is not a librw/GPU draw.
PTank lock fixtures supply explicitly test-owned buffers and a binary-layout
prefix; no production PTank buffer owner or fake rendering-success slot is added.

Default class callback identities are retained, including advanced interpolation,
shape, clone and stream callbacks not implemented here. Those identities introduce
additional unresolved references; they are fatal production slots, and aborting
link-fixture placeholders only in tests/rw_globals.cpp. Startup immediately
replaces these arrays through the game's own registration routines. Do not infer
their execution merely from an address-taken reachability line.

## Layout evidence

32-bit assertions in particles.h cover the following layouts. Disassembly is
under `work/rw8-check/00*.asm`; game consumers include 006E0920, 006E0E00,
006E04D0, 006E05D0, 006E0670, 007047C0, 00704D60 and 006EED20.

| Header | Original offsets / source of evidence |
|---|---|
| Property table, 0x28 | allocation +0, circular next +4, ID +8, refcount +C, property count +10, IDs +18, offsets +1C, padded sizes +20, unrounded sizes +24; 57C9A0/57C770/57C8B0 plus direct game reads |
| Both classes, 0x1C | next +0, ID +4, refcount +8, stride +C, table +10, callback-set count +14, callback array +18; 57CCF0/57FC00 and setters/destructors |
| Emitter, 0x28 | next +0, flags +4, owning atomic +8, emitter class +C, count +10, batch capacity +14, particle class +18, inactive/active tails +1C/+20; 57FFF0/580100/580230/5802A0 and game consumers |
| Batch, 0x20 | next +0, class +4, count +8, capacity +C, first-new index +10, inline-data offset +14; 57FFA0/57FEE0/580230/580160 |
| Standard emitter property, 0xAC | seed +0, time +8, gap/bias +34/+38, previous/next emission +3C/+40, size +74, color +7C, rotation +80, UV +84/+8C, texture +A4, borrowed LTM +A8; create/begin/schedule/destroy bytes |
| Emitter PTank property, 0x34 | count +8, flags +10, atomic +18, cluster pointer/stride arrays +1C/+20/+24, blending +28/+2C/+30; 580FC0/581130/581250 and game callers |
| PTank prefix, 0x138 | write-lock mask +3C, dirty mask +40, cluster descriptors +4C..+90, interleaved stride +9C, format flags +A0, rotation +C0; 5821D0 jump tables/5823E0/57EF90 and 006EED20 |

Original-byte fixtures assert the startup schemas, cache/refcount paths, defaults
and descriptors. In particular 131131 has emitter properties
`0,1,2,6,5,4,0x1000007` (sizes `40,172,64,52,32,16,20`); 131187 has
`0,1,2,6,8,5,0x1000007` (`40,172,64,52,116,32,20`). The game's custom 8/4/68-byte
properties can be appended without losing the old layout or leaking tables.

Native tests widen pointers; property-table numeric offsets remain x86 values
and a boundary-local accessor adjusts pointer-bearing emitter properties there.
They are not 32-bit ABI execution tests. The COFF/Unicorn fixtures are x86.

## Verification

Final build/inventory results are recorded in the addendum below. Logs, original
disassembly, ABI audit and inventories are in `work/rw8-check/` (scratch).

* All eight native RW targets, including every tests/rw_*.cpp fixture (rw_globals
  is shared link data), are configured/built/run with AddressSanitizer and
  UndefinedBehaviorSanitizer. LeakSanitizer is disabled, as in earlier slices.
* `.venv/bin/python tests/rw_particles_original.py` uses tools/emudiff.py's image
  and COFF loaders, executes original schemas/defaults/PTank bytes, and compares
  **25 original/COFF callback/batch cases**, including memory, returns, ordered
  callbacks, cdecl stack and nonvolatile registers. Allocator, registry and
  callback doubles are explicit; real Windows/device/asset calls are absent.
  Undefined original batch padding/data is excluded, not claimed equivalent.
* No claim of stock randomly filled emudiff profiles passing these heap-owning
  library functions. No Windows/game/driver or real-particle frame run.

Boundary validation additionally rejects invalid counts/overflow, preserves old
callback arrays on OOM and makes table self-assignment safe. Undefined original
header padding is zeroed. Arbitrary reentrant ring mutation/destruction, failing
destructor callbacks in the generic (void) plugin destructor chain, advanced
pointer-bearing property payloads on a 64-bit host, and custom allocator alignment
or lifetime beyond these fixtures remain unverified.

## Next Windows run (prediction, not verification)

With the earlier successful startup path and `CSF_RW_TRACE=1`, expect class traces
for 005784E0, 0057FA90, 0057F970 and 005785B0. It should **pass the old
UNRESOLVED 005784E0 barrier**, finish the registered startup class/property arrays
and proceed toward subsequent world/light/asset initialization. A new exact first
stop cannot be promised: asset calls, virtual calls and callbacks are not ordered
by the static closure. Particle construction/update/drawing is still blocked by
the fatal slots above; DDS/arena/compressed-animation barriers are independent.

## Changes and follow-up

Changed: boundary/rw CMake integration, life.cpp, plugins.cpp, object.cpp; new
particles.h/.cpp; tests/rw_globals.cpp; new tests/rw_particles.cpp and
tests/rw_particles_original.py; this report. librw is unmodified. No changes to
PLAN/JOURNAL/queue/db/export and no commits.

For the orchestrator: record a **partial** slice, not a completed simulation or
renderer. Preserve metadata corrections (5785B0 particle class/two arguments;
5802A0 particle-class setter; ED00/F0B0 cdecl callback entries). Continue with the
three simulation kernels, real PTank owners and librw quad/atomic/immediate
conversion, followed by stream/clone advanced properties and Windows acceptance.

## Final verification addendum

Completed with exit 0 on final sources:

* Native instrumented configure:
  `cmake -S . -B /tmp/csf-rw8-asan -G Ninja -DCSF_RW_CPU_SLICE=ON
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g'
  '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined'`.
  All eight `csf_rw_{cpu,life,image,object,device,toolkit9,plugins,particles}_link`
  targets build with `-j 8` and pass with `ASAN_OPTIONS=detect_leaks=0
  UBSAN_OPTIONS=halt_on_error=1`. Final logs: native-final-build.log,
  native-final-tests.log; expanded all-nine-selector fixtures additionally in
  native-strides-build.log/native-strides.log.
* `.venv/bin/python tests/rw_particles_original.py`: **25 original/COFF cases
  pass**, plus original startup schemas/cache/defaults and all nine PTank cluster
  selectors. COFF is the fresh linktest object, not a stale hook object.
  Log: original-final.log. This does not execute the missing simulation kernels.
* `.venv/bin/python tools/linktest.py -j 8 --out work/rw8-check/before.json`:
  **14801/14801 files, 15342 definitions, RW unresolved 176**.
  Final equivalent command with after.json:
  **14802/14802 files, 15369 definitions, RW unresolved 192**.
  All **27 new definitions exactly match the particles.cpp HOOK set**; no lost
  definitions, compile failures, duplicates, raw-address or assembly-address
  records. Other groups: game 82, netz 163, png 1 unchanged;
  not-a-function **6 -> 4** (ED00/F0B0 supplied).
  RW **23 old traps removed, 39 new default callback references added**, hence
  the net increase of 16, not a misleading unresolved-slot reduction claim.
  Exact lists: set-check.json; before.log/after.log.
* `cmake -S . -B build -DCSF_RW_CPU_SLICE=ON` and
  `cmake --build build --target csf_hook csf_rw_particles_link
  csf_rw_d3d9_link -j 8`: hook DLL and both Win32 probes link.
  `cmake --build build --target csf_standalone -j 8`: PE32 i386 GUI executable
  links against the unmodified librw D3D9 archive. Final incremental build of
  all four targets also passes. Logs: win-build.log, standalone-build.log,
  win-final-incremental.log. **None of these Windows binaries was executed.**
* The standalone's expanded actual-input inventory has **354 RW fatal traps**;
  that includes image/data/vtable roots, not just the 192 source-only slots.
  `.venv/bin/python tools/initreach.py --root 0x509250` on these freshly generated
  traps: **1217 source-reached functions, six RW traps**, no unclassified
  callback traps. They are 57CFE0, 57DBA0, 57BBD0, 63A4D0 (DDS), 61F5E0
  (compressed float helper), 7C5F60 (arena). Log: reach-linked.txt.
  This walk follows game sources/address-taken references, not execution order,
  boundary internals, computed calls, virtuals or backend code. It does not bound
  all runtime traps or prove that the next stop is one particular slot.
* `git diff --check` passes; `git -C work/tools/librw status --short` is empty.

No general leak-free claim, Windows/device/game acceptance, real asset parsing,
complete particle simulation or librw particle draw verification.
