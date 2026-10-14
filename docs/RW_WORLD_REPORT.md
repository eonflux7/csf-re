# World API follow-up — partial delivery

## Finding

18 requested addresses have new standalone definitions. This is **not** the
complete world/asset cutover: eight requested addresses remain traps, and the
sector helpers only support the tracked empty leaf that this boundary creates.

The original bytes contradict several inventory names. No db/export renames
were made:

* World clump count is **+0x24**, not +0x20; clump insertion is at the head of
  World+0x2C, using Clump+0x20. The facade field order is corrected and asserted.
* **0058F0E0 is Light-to-Clump**, reading the offset cell 008B13B4. It is not
  Clump-to-World. Original User/World plugin attachment and its light-destruction
  caller establish the object types. The corrected getter reads the published
  facade clump pointer; Clump-to-World remains 0058DF40 / cell 008B1350.
* **0058BD60/0058BD90 are Frame** user-data count/get (cell 008B12A4), not
  Material. **0058BDC0/0058BDE0/0058BE10 are Material** remove/count/get (cell
  008B12BC), not Texture. 0058C040's registration calls establish those cells.
  The misleadingly named callers also agree: 006B86A0 reads the frame at a
  light's +4; 006BB8A0 consumes the material's extension.
* **0058E680 is the default Atomic render callback**: AtomicCreate installs it
  at +0x48; it chooses Atomic+0x70's pipeline or the engine default, then calls
  pipeline execution. It remains trapped: a game Rx pointer cannot be passed
  to librw's pipeline implementation.

## Step 1: membership, spheres, empty-sector helpers

New definitions:

```
0058E280 RpWorldAddClump
0058E310 RpWorldRemoveClump
0058DE00 RpWorldRemoveAtomic
0058EC60 RpAtomicGetWorldBoundingSphere
0058CC60 RpWorldSetSectorRenderCallBack
0058CFF0 RpWorldForAllWorldSectors       (tracked empty root only)
0058D1A0 RpWorldSectorGetWorld          (tracked empty root only)
```

Membership preflights mapped members before librw's assertion-only addClump.
It publishes facade world pointers for clump, atomics, lights and cameras,
maintains the facade clump/light rings and count, and updates native membership.
The currentClump cursor advances before its clump is unlinked. Removal composes
librw's per-object removal primitives and native clump-link removal so an
independently removed atomic does not trigger librw removeClump's assertion.
Light world rings are prepended in both facade and native worlds; otherwise
librw's appended order could change finite-light-set selection.

World sphere retrieval composes existing interpolated-sphere resync, facade
frame synchronization and librw centre transformation. The boundary supplies
the missing maximum-basis-length radius scaling. Repeated cached calls do not
multiply the radius again. The returned pointer is Atomic's facade sphere.

Null SetSectorRender restores the empty-leaf default. The original default
tests triangle count, not vertex count; a vertex-only leaf needs no draw.
Nonempty triangle rendering still fails rather than pretending to draw it.

Existing world clump/light walkers retain next-before-callback and null-stop
rules; the material walker retains count-once and null-stop. The new
single-leaf world-sector visitor has only one callback;
it is **not** a BSP left-before-right walker. Unknown/untracked roots are rejected.

### Step 1 left (five traps)

```
0058CBB0 RpWorldRender
0058DE50 RpWorldSectorForAllAtomics
0058DEF0 RwCameraForAllWorldSectors
0058E680 default atomic pipeline callback
005903F0 RpWorldSectorRender
```

The unmodified librw World has no BSP, sector ties or camera visible-sector
cache. The current facade also has no tie creation/removal or static-sector
geometry conversion. Original DE50 inserts a temporary cursor into its tie
ring during callbacks; a vector of world clump members would not reproduce
that mutation contract. DEF0 walks the camera plugin's cached sector array,
not all world roots. CBB0 renders that visibility list and sector ties, not
librw World::render's unconditional clump list. Neither these differences nor
the custom Rx pipeline dispatch can safely be hidden behind direct forwarding.

## Step 2: clump streaming — left (three traps)

```
0058FD10 RpClumpStreamGetSize
0058FDF0 RpClumpStreamWrite
0058FFC0 RpClumpStreamRead
```

No streaming slots were installed. Native Clump::streamRead creates native
frames, atomics, shared geometries, materials, lights and cameras, but the
boundary's identity maps only know objects built through its facade helpers.
There is no graph adopter/rollback path. Native extension streaming also does
not invoke the ABI registries' game callbacks with facade identities/offsets.
Returning the native root would expose incorrect +4 frames, +18 geometry,
material headers and plugin offsets. Merely allocating a facade root would
still leave the children unmapped. GetSize/Write likewise cannot omit the game
plugin streams and claim an original-compatible round trip.

This is an integration task still to do, not a claim that librw cannot read a
clump. Required next work is transactional graph adoption plus supported plugin
payload/stream translation (including HAnim/skin and game plugin callbacks).

## Step 3: user-data array slots

All eleven requested numeric addresses are defined, using byte-proven types:

```
0058BCC0 Geometry remove       0058BCE0 Geometry count
0058BD10 Geometry get          0058BD40 Frame remove
0058BD60 Frame count           0058BD90 Frame get
0058BDC0 Material remove       0058BDE0 Material count
0058BE10 Material get          0058BE40 real value (ST0 / double)
0058BE50 int value
```

librw owns the per-object arrays and their payload allocations. The extension
and array POD headers match x86 (8 and 16 bytes respectively); getters publish
a borrowed extension into the ABI tail, never a native owning object header.
Native fixtures widen the extension allocation to avoid truncated pointers.
Count returns arrays with nonnull data, not allocated capacity. Removal uses
librw's payload release and then compacts the ownership headers, clearing the
last slot while retaining capacity, as the original does. Direct array POD
access and scalar retrieval therefore remain game-compatible.

No requested user-data slot is left. This does **not** make unadopted streamed
objects readable. Populated Frame user-data cloning is explicitly rejected:
byte-copying borrowed facade tails aliases ownership, and upstream's copy
callback writes through the source array. The rejection fixture verifies that
the source string remains intact. A deep-copy boundary is still open.

## Verification

Original ABI/layout/algorithm evidence was read from CommXPC.exe using pefile
and Capstone, with the exports/callers as cross-checks. Disassembly is in
`work/rwworld/original.asm` and `original-extra.asm`. All new slots use cdecl;
scalar real retrieval is FLD DWORD / RET, not an integer-register return.

Native configure:

```sh
cmake -S . -B /tmp/rwworld-asan -G Ninja -DCSF_RW_CPU_SLICE=ON \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g' \
  '-DCMAKE_C_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g'
```

All eleven native targets were built and executed: cpu, life, image, object,
device, fs, particles, plugins, toolkit9, world, world_userdata. The runner uses
`ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1`; no sanitizer errors
remain. LeakSanitizer's enabled run reports that it cannot operate under the
sandbox's ptrace environment; leak checking is not claimed. The synthetic
UserData first-add fixture seeds an empty allocation to avoid upstream's
`memcpy(dst, nullptr, 0)` nonnull-attribute UB without suppressing UBSan or
patching librw. Logs: `/tmp/rwworld-native-build.log`,
`/tmp/rwworld-native-tests.log`.

The original linktest started before edits and compiled boundary files after
edits. For a clean before inventory, HEAD versions of object/world/plugins
were compiled with linktest.flags() into `work/rwworld/baseline/`; their COFF
objects replace those three inputs, and userdata.cpp is excluded. The exact
input manifest is `work/rwworld/before-objects.txt`. The reconstructed baseline
has 14,879/14,879 inputs, 15,748 slot definitions, RW unresolved **176**, no
duplicates or raw original addresses. Commands:

```sh
.venv/bin/python tools/linktest.py -j 8
.venv/bin/python tools/linktest.py -j 8 --objects work/rwworld/before-objects.txt \
  --out work/rwworld/before.json
.venv/bin/python tools/linktest.py -j 8 --out work/rwworld/after.json
```

After: **14,880/14,880** files compiled, **15,766** slot definitions, RW
unresolved **158** (176 -> 158). Zero compile failures, duplicate slots or raw
original-address hits. A set comparison of before.json and after.json verified
that exactly the 18 listed addresses disappeared, no new RW unresolved slots
appeared, and the eight listed remaining slots are still unresolved. Log:
`/tmp/rwworld-link-after.log`.

```sh
cmake --build build --target csf_standalone -j 8
cmake --build build --target csf_hook -j 8
file build/csf_standalone.exe build/dinput8.dll
git diff --check
git -C work/tools/librw status --short
```

Both builds finished successfully and produced PE32 i386 executable/DLL files.
The hook DLL excludes the boundary; no RW hooks are installed over the original
library. The actual generated `build/cutover/stubs.json` was checked separately:
all 18 implemented addresses are absent and all eight listed remaining RW
traps are present. Its total RW trap count is **318**, not 158: the actual linker
inventory includes generated-data/vtable references that source-call linktest
does not count. Logs: `/tmp/rwworld-standalone.log`, `/tmp/rwworld-hook.log`.
Whitespace check passed; librw's git status is clean.

## Runtime conclusion (not Windows-verified)

Menu teardown should get past the three new membership slots and the corrected
Light-to-Clump getter, and its empty-sector callbacks should receive the facade
root. Settings-to-Game succeeding is **not** established. A sector atomic walk,
camera visibility/render path or pipeline callback can still hit the five
listed traps. A level load using RpClumpStreamRead still stops at 0058FFC0;
there is no claim that a level loads or renders. No Windows/game/smoke execution
was possible or attempted.
The new clump fixture contains atomics and lights, not cameras; the
camera-containing-clump publication path is code-reviewed but not exercised by
that fixture (ordinary camera/world add/remove remains covered by rw_plugins).

## Files and tests

Changed boundary files: world.cpp/h, object.cpp/h, plugins.cpp, CMakeLists.txt.
Added userdata.cpp, tests/rw_world.cpp, tests/rw_world_userdata.cpp. Updated
tests/rw_plugins.cpp's default-empty-sector expectation to the byte-proven
triangle-count condition, with a separate nonempty-triangle rejection check.

World tests cover head order, callback stop/current-removal teardown, count and
cursor, native/facade light ordering, published world/clump getters, wrong-owner
rejection, independent Atomic removal before Clump removal, scaled/cached
spheres, empty sector identity, callback replacement/reset and full teardown.
User-data tests cover every numeric slot, POD-tail publication, scalar values,
compaction/capacity, invalid positive indices, string release, populated-frame
clone rejection and native ownership teardown. Invalid owner/index inputs are
rejected safely; exact error-reporting behavior for invalid original inputs is
not claimed. librw remains unmodified; no SDK source/headers were used.
