# Clump streaming boundary (2026-10-15)

## Finding

`0058FFC0 RpClumpStreamRead` and `0058FD10 RpClumpStreamGetSize` now have
standalone facade implementations. This is an offline-tested asset-streaming
boundary, **not evidence that a campaign mission loads or renders**. Write
(`0058FDF0`) is deliberately still a trap.

## Original evidence

Read `export/funcs/{0058ffc0,0058fd10,0058fdf0,0063cb30,00632d40,0058f940}.c`
and the original executable's disassembly. The address packets and disassembly
are in `work/rwclump/`; no SDK source or headers were used, and librw is unchanged.

The original cdecl reader accepts versions 3.5.0 through 3.7.0.2, reads the
three-count struct, creates the clump, constructs the frame list/hierarchy and
reads frame extensions, reads the geometry list (including materials/textures),
then atomics, lights, cameras, and finally the clump extension. Atomic extensions
run after geometry/frame assignment but **before** adding the atomic to its
clump. Light/camera extensions run before their frame/clump assignment. Geometry
list holds are released before the clump extension. Failure returns null;
`0063CB30` checks each read callback's nonnull result, unlike librw's reader.

Original direct-call scan and `export/boundary_callers.json` agree:

* Read: `006BE9C0`, `006BEC60`, `006D47E0`, `006FD860`.
* GetSize: `006D41C0`, `006D47E0`, `006FD860`. The last two are loading paths,
  using the size for memory accounting; leaving GetSize trapped would stop them.
* **The game does write clumps**: `006BDDC0 GfxEnvironment_WriteToFile` (four
  callsites), `00708980 GfxAnimInterpolator_WriteSectorClumpsWithExtension`.
  This is static evidence, not evidence of their runtime frequency.

## Design

The caller has already consumed the CLUMP header. The boundary buffers through
the root extension, validates chunk envelopes, version, counts and graph
indices, then invokes **librw's Clump::streamRead**, not a copied RW reader.
Known malformed graphs are rejected before native unchecked indexing. Successful
reads leave the following parent-stream chunk untouched; callback failures
restore the parent stream to the failed reader's cursor.

A scoped adapter temporarily substitutes native plugin dispatch lists, without
changing native object sizes or editing librw. Original constructors,
destructors, readers, always callbacks and rights callbacks remain native. A
constructor observer creates the corresponding facade and runs ABI constructors
at native object construction time. Extension readers refresh facade fields,
run any native decoder, then route registered game payload readers through
`plugins.cpp` with ABI identities, offsets and sizes. Game callbacks receive a
registered, borrowed facade stream bounded to that plugin payload. Unknown
chunks remain skipped by librw. A null callback result, overread or underread
aborts; reentrant clump reads are rejected.

Frame maps publish hierarchy links; facade frame object rings contain facade
objects, never native objects. Atomics share one facade for each native geometry;
materials/textures likewise retain their native reference counts. Morph/vertex,
triangle, colour and UV POD storage remains native-owned. Mesh headers and mesh
material pointers are translated. Clump atomic/light/camera facade rings use the
original head-insertion order, while native membership remains native-owned.

The transaction journals native/ABI engine allocations and completed native
objects. Rollback destroys unlinked in-flight members before clump-owned frame
trees, then releases remaining geometry-list/material references and frames.
Native destructor observers run ABI destructors and remove identity maps;
facade storage is deferred until graph teardown finishes. Unwound native
temporary arrays and partially constructed allocations are reclaimed from the
journal. Frame allocation counters are restored on rollback (upstream
destroyHierarchy does not decrement its counter).

Shared dictionary textures survive rollback with corrected counts. UserData
headers borrow native POD payloads. Partially decoded UserData is not traversed
by upstream's unsafe partial-array destructor; its allocations are journaled.
For preexisting shared textures, old UserData ownership is retained until commit
and restored on rollback. Permanent zero-storage native texture/raster destructor
observers also remove facade aliases for secondary textures owned by native
plugins (not just a material's primary texture).

On commit the ordinary facade destroy APIs own teardown; the temporary native
stream adapters and allocator are restored. GetSize uses native size primitives
with facade game size callbacks, and removes repeated geometry chunks to match
the original's unique geometry list. It does not install a writer whose size
or plugin payloads would silently disagree with the original.

## Coverage and limitations

Covered: frames/hierarchy, atomics, shared geometries, material/texture identity
and references, CPU geometry/morph data, translated mesh headers, clump lights
and cameras, native plugin decoding, registered game read/size callbacks,
UserData publication and teardown, primary and secondary texture lifetimes.

HAnim/skin and MatFX, **if attached**, are still decoded/owned by librw; this job
does not provide ABI HAnim hierarchy/skin/MatFX object facades or animation/pipeline
APIs. Their native owning headers are not copied into game plugin tails. The
fixture's secondary-texture plugin exercises MatFX-like ownership, not actual
MatFX rendering or decoding. Native D3D9 geometry uninstancing, HAnim/skin and
GPU resource rollback were not exercised. Nonempty world rendering remains a
separate boundary task. No Windows execution was attempted.

The structural validator supports the original version range, a connected frame
tree, and geometry-list-referenced atomics. It rejects empty/disconnected frame
lists and the original's zero-geometry-list/inline-geometry fallback rather than
letting librw dereference an absent list. It is not a hostile-input audit of
every native plugin decoder. Buffering validates an asset before callbacks, so
malformed/truncated inputs need not reproduce original partial-read side effects.
Arbitrary game callback mutations to external/global state are not reversible;
the transactional contract concerns the streamed graph and tracked ownership.

## Tests and commands

`find $CSF_PAKS/GlobalEK -iname '*.dff'` found **no files**. The fixture
in `tests/rw_clump.cpp` is generated by librw's writer: two frames, two atomics
sharing a geometry, materials/textures, a light, a camera, string UserData,
registered game chunks on all eight object types, unknown chunks and a native
plugin owning a secondary texture. The writer's unused duplicate geometry tests
geometry-list cleanup as well as shared facade identity.

Native configuration:

```sh
cmake -S . -B /tmp/rwclump-asan -G Ninja -DCSF_RW_CPU_SLICE=ON \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g' \
  '-DCMAKE_C_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g'
```

Build all twelve `csf_rw_*_link` targets, then run each with
`ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1`. Logs:
`/tmp/rwclump-all-build.log`, `/tmp/rwclump-all-tests.log`. LeakSanitizer is not
claimed. All twelve executables passed. The clump fixture passed eight extension
failure stages, ten constructor failures and **75 allocation-failure points**
with a game texture loader, plus **93 allocation-failure points** through the
production default Image-to-raster texture loader,
with retry after rollback, existing
dictionary textures, root/atomic callback timing, plugin offsets, size callbacks,
following-stream preservation and complete teardown. Its allocator accounting
also asserts that no engine allocations made by the fixture/read remain after
teardown, including partial-reader buffers (separately from map/counter checks).
Ordinary frame/atomic/geometry/material/light/camera/clump/raster factories and
destruction are also exercised inside the game clump callback: temporary native
observers reuse the same facade identity and do not double-run ABI destructors.
The callback also clones a native atomic, exercising the adapter's copy slots.
The default-loader sweep also exercises temporary image facade cleanup; it uses
a synthetic native image format and file-open fixture, not real game textures.
The game-loader sweep deliberately leaves a C-style temporary Image unfreed
when texture allocation aborts; the transaction's image checkpoint removes it.

Final cutover verification:

```sh
.venv/bin/python tools/linktest.py -j 8 --out work/rwclump/after-final.json
cmake --build build --target csf_standalone -j 8
cmake --build build --target csf_rw_clump_link -j 8
git diff --check
git -C work/tools/librw status --short
```

All commands succeeded. The Windows clump fixture and standalone linked as
PE32 i386 executables; neither was executed. The exact compiled input manifest
excluding only `clump.cpp` (`work/rwclump/without-clump-objects.txt`, inventoried
with `linktest.py --objects`) has RW **158** unresolved; the final compiled source
inventory has **156**. A set comparison verified that precisely `0058FD10` and
`0058FFC0` disappeared, with no new RW unresolved slots. The actual generated
standalone stubs separately contain **316** RW traps (including data/vtable-only
references); Read/GetSize are absent and Write remains present.

Final source inventory (`/tmp/rwclump-final-link.log`):

```text
compiled 14882/14882 files standalone; 15769 functions implemented
unresolved functions: game 23, netz 154, png 1, rw 156
data to generate: 2897 globals, 1154 vtables; raw image addresses in code: 0 in 0 files; asm thunks with original addresses: 0 files; other undefined symbols: 50
```

No compile failures or duplicate slots. Standalone build log:
`/tmp/rwclump-standalone-final.log`; Windows fixture link log:
`/tmp/rwclump-win-link.log`. Whitespace check passed and librw's worktree is clean.
