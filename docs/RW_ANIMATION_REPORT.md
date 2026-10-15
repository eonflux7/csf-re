# Animation / character boundary — partial delivery

## Finding

26 of the 27 requested slots now have standalone implementations, including
`0061F3C0 RtAnimAnimationRead`. There are 34 new address definitions in
`animation.cpp`, including the immediately adjacent interpolation APIs. Native
ASan/UBSan tests read and sample **all 560 animations in retail Parachut.pak**
(559 compressed scheme 2, one standard scheme 1; 10,431,976 decoded bytes).
This is not evidence that FR01 loads or renders in the Windows standalone.

**007C73A0 RtSkinSplitAtomicSplit is deliberately still a trap.** Librw reads
split-skin metadata but has no equivalent mesh partitioning tool. The original
function changes geometry mesh/bone palettes and RLE/remap metadata; a successful
no-op would be false. Its original early returns and partitioning/validation
stages were inspected. Existing split metadata is exposed, not regenerated.
This can still stop mission loading if the unsplit branch actually executes.

## Per requested slot

| Address | Slot | Delivery |
|---|---|---|
| 0061F3C0 | RtAnimAnimationRead | File/chunk reader using facade file streams |
| 0061EE90 | RtAnimInterpolatorDestroy | Frees interpolator sidecars or raw animation allocations; returns 1 |
| 0056CF80 | RpHAnimHierarchyGetMatrixArray | Stable, native-owned 64-byte matrix array |
| 0056DB50 | RpHAnimHierarchyAttach | Frame-subtree ID walk; returns hierarchy |
| 0061F050 | RtAnimInterpolatorSetCurrentAnim | Registered callbacks, initial key pairs, native interpolator |
| 0056D3F0 | RpHAnimHierarchyUpdateMatrices | Stack composition, subhierarchy roots and local/LTM flags |
| 00587020 | RpSkinAtomicSetHAnimHierarchy | Translates both identities; publishes ABI plugin tail |
| 0061F590 | RtAnimInterpolatorSetCurrentTime | Forward/backward time, endpoint/wrap and callbacks |
| 0061F120 | RtAnimInterpolatorCopy | **Pose-only** copy, not interpolator state copy |
| 0061EFB0 | RtAnimAnimationGetNumNodes | Bounded predecessor-chain sentinel scan |
| 0056CDB0 | RpHAnimHierarchyCreateSubHierarchy | Parent-owned matrix/node slices; separate interpolation storage |
| 0056CFA0 | RpHAnimFrameGetID | Reads authoritative ABI HAnim extension |
| 007C5D00 | RtCompressedKeyConvertFromStd | Scheme 2 conversion and translation centre/scale |
| 0061EE10 | RtAnimAnimationCreate | Original contiguous header/key/custom allocation contract |
| 005712A0 | RpMatFXAtomicQueryEffects | Native plugin query |
| 0056CF90 | RpHAnimFrameGetHierarchy | Returns facade identity from ABI extension |
| 0056CED0 | RpHAnimHierarchyCreateFromHierarchy | Independent nodes/matrices/interpolator; unattached frames |
| 0056CD30 | RpHAnimHierarchyDestroy | Root/sub ownership; original success result **0** |
| 0056CF50 | RpHAnimHierarchyDetach | Clears node frame bindings; returns hierarchy |
| 005712B0 | RpMatFXMaterialGetEffects | Native effect type |
| 00571610 | RpMatFXMaterialSetEnvMapFrameBufferAlpha | Native effect setter; returns facade material |
| 005715D0 | RpMatFXMaterialSetEnvMapFrame | Translates frame identity |
| 005870F0 | RpSkinAtomicGetType | Native pipeline plugin ID/data, not a cast of the facade pipeline |
| 00587050 | RpSkinGeometryGetSkin | Stable skin header; shared native bone/vertex buffers |
| 005870E0 | RpSkinIsSplit | Original remap-pointer predicate |
| 007C73A0 | RtSkinSplitAtomicSplit | **Not implemented**, reason above |
| 00571920 | RpMatFXMaterialSetUVTransformMatrices | Compatible Matrix POD pointers retained by native MatFX |

Additional definitions: `0061EEB0` animation stream reader, `0061EFE0`
interpolator create, `0061F150` add time, `0061F410` subtract time, `0061F320`
blend, `0061F380` subinterpolator create, `0056CC30` hierarchy create,
`00587040` atomic hierarchy getter. Existing `0061F5E0` float compression in
`plugins.cpp` is reused, not defined twice. No original function or database
entry was renamed.

## ABI mapping and contracts

These are original x86 headers, not owning librw C++ headers. Offsets and sizes
are asserted in the Windows build; pointers widen only in native tests.

| Facade | x86 size / relevant offsets |
|---|---|
| Animation | 24 bytes; scheme +0, count +4, flags +8, duration +12, keys +16, custom +20 |
| Interpolator | 76 bytes; anim/time +0/+4, next +8, callbacks +12..+28, capacity +32, pose stride +36, key stride +40, nodes +44, sub/offset/parent +48/+52/+56, cached callbacks +60..+72; poses follow at +76 |
| Hierarchy | 36 bytes; flags/nodes +0/+4, matrices +8, owning allocation +12, nodes +16, parent frame +20, parent hierarchy +24, root parent index +28, interpolator **pointer** +32 |
| Node | 16 bytes; ID/index/flags/frame +0/+4/+8/+12 |
| Compressed key | 24 bytes; prev +0, time +4, seven 16-bit components +8, padding +22 |
| Skin | 64 bytes; bone count +0, used count +4, used indices +8, inverse matrices +12, weight count +16, indices +20, weights +24, split limit/count/RLE size +28/+32/+36, remap +40, RLE counts +44, RLE +48 |

Native interpolators and scheme views live in private sidecars. Import/export
copies the compatible pose POD array and synchronizes time/key/callback state;
no native animation/interpolator header is returned to the game. Custom game
schemes retain their callback pointers; stream callbacks receive facade streams.
Builtin HAnim callbacks use librw quaternion/matrix/vector primitives. The
compressed 1/4/11 component encoding is **not IEEE half**. Its decoding and
translation custom data follow the binary; conversion calls the existing
boundary encoder. Disk strides (standard 36, compressed 22) are independent of
host pointer widths; serialized predecessor offsets use original in-memory
strides (36/24).

Retail initial time-zero keys contain wrapped offsets originating from null
predecessors at the writer's heap address. Their predecessors are never followed.
The reader normalizes invalid time-zero predecessors to null rather than forming
wild host pointers; invalid noninitial offsets and short streams are rejected.
Standard key decoding itself uses librw's registered stream decoder.

HAnim matrices, skin inverse matrices (64-byte stride), four indices/weights per
vertex, used bones and split/RLE POD storage remain native-owned. Hierarchy/node
headers translate every frame identity. Subhierarchies alias parent slices and
never invoke librw's owning node/matrix destructor. MatFX owning headers stay
native; only the requested query/setter APIs translate identities. No promise
is made that backend-specific skin `platformData` has original RW layout.

The original Copy copies only poses; callback/time/animation metadata stays in
the destination. Attach visits all matching node IDs, including duplicates;
librw's unattached-frame search has different duplicate-ID semantics. Matrix
updates compose librw matrices and supply the upstream missing subhierarchy,
NOMATRICES and modelling/LTM flag behavior. All frame lookups/imports happen
before matrix writes, avoiding a later lookup overwriting an earlier result.

ABI plugin registration and game callbacks are retained. Builtin HAnim/skin
pointer-sized reservations widen in native tests, but have unchanged x86 sizes.
Native clump payload publication now installs HAnim/skin facade pointers before
game read callbacks. Teardown removes borrowed sidecars in both ordinary and
transactional destruction. HAnim frame copies duplicate hierarchy ownership;
clump clones retarget self-contained skin hierarchies to the cloned frame tree.

The new fixture exposed an existing clump Input EOF bug: librw checks EOF after
reading a header, and a final empty extension was incorrectly treated as a
failed read, entering its unsafe already-freed geometry-list cleanup. Bounded
Input now has stdio-style successful-read EOF semantics, with explicit final
cursor validation. Librw itself was not changed.

## Evidence and reproduction

Original contracts: read `export/funcs` for every requested slot and relevant
adjacent/helper functions, including `0056D980`, `0056CFB0`, `0061F380`,
`0061F6D0`, `0061FF00`, `0061F5E0`. Source caller inventory is retained at
`work/rwanim/slot-callers.json`. Especially relevant direct-field consumers:
`006E9FF0`, `006EA4D0`, `006EA430`, `006E81A0`, `006C3290`, `006E80F0`,
`006C4DF0`, `006E80C0`, `006E5940`. References were the original binary/exports
and the unmodified MIT librw checkout, not leaked SDK material.

`tests/rw_animation.cpp` covers standard/compressed stream decoding, short and
invalid noninitial offsets, compression including constant translation axes,
nontrivial rotations/application and short-arc blend mutation, forward/backward
and wrapped time, time/loop callbacks, pose-only copy, custom scheme read and
interpolation callbacks, root/sub/copy hierarchy ownership, shared skin buffers,
both split predicates, native MatFX state, streamed game plugin callbacks,
duplicate-ID attach, raw ID writes, modelling/LTM updates, clone/source teardown,
plugin-read rollback, and animation/interpolator allocation failures. Engine
shutdown/reinitialization is exercised twice and sidecar maps must be empty.
The old nonfunctional animation callback doubles in `rw_globals.cpp` are gone.

The optional asset test is reproducible without Windows:

```sh
ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 \
  .venv/bin/python tests/rw_animation_assets.py \
  --pak /mnt/e/dev/csf-builds/csf-pc-retail/Maps/Parachut.pak
```

It validates PAK/zlib framing, extracts only animations into ignored
`work/rwanim/retail`, records SHA256s in `manifest.json`, then passes every file
to the native fixture. Each animation is sampled at fractions .25, .75, 1, .5,
0 and 1.5 of its duration, checking finite quaternion/translation values for
every node. Both standard and compressed real files are exercised. Sample:
`Anims/Comm/LlamarSanitario.anm`, 10,704 bytes, SHA256
`c423ce25961687d8a2981caf10dec51a39e3837adf317d5dd2a43f38f7e0aa15`.
No additional extraction from the user is needed for these animation checks.

Full check command: `tools/check_all.sh --no-walk --offline`. The new optional
`--offline` switch skips only Windows dryrun/walk, which this worker cannot run.
The regular script behavior is unchanged without that switch. Native tests use
ASan/UBSan with `detect_leaks=0`; LeakSanitizer is not claimed. Retail animation
and native synthetic tests are not original-vs-port numerical/emulated diffs.

## Still open / not verified

* Skin mesh partitioning (`007C73A0`) remains unimplemented, not replaced by
  a successful no-op or made to report split metadata it does not have.
* No Windows execution, campaign load, skinned rendering or GPU verification.
  Native skin/MatFX pipeline tests are on the NULL backend; production slots
  are compiled for librw D3D9. Backend bone palette limits still matter.
* The wider atomic Rx/pipeline facade and execution boundary is not completed
  here. In particular, direct game checks of Atomic +0x70 belong to that
  existing gap; a passing native `RpSkinAtomicGetType` query does not establish
  a complete original pipeline-header ABI or exercise the game splitter caller.
* No complete hostile-input audit of librw's native HAnim/skin/MatFX decoders,
  and no full animation/native-plugin allocation-failure sweep. The existing
  clump failure sweeps remain separate from this new fixture's targeted cases.
* Animation writers, arbitrary unrequested toolkit APIs and complete MatFX
  owning-header ABI publication are not implemented by this slice.

## Final build / link evidence

All 13 native `csf_rw_*_link` executables pass with ASan/UBSan after the final
clone/subhierarchy test changes. The retail asset command above also passes
after those changes (`/tmp/rwanim-assets-final.log`). Final per-test logs are
`/tmp/rwanim-final-csf_rw_*_link.log`.

`tools/linktest.py -j 8` succeeds with no duplicate slots or compile failures:

```text
compiled 14909/14909 files standalone; 15832 functions implemented
unresolved functions: netz 154, png 1, rw 126
data to generate: 2880 globals, 1154 vtables; raw image addresses in code: 0 in 0 files; asm thunks with original addresses: 0 files; other undefined symbols: 50
```

The matching baseline is **162 RW**, not the older journal's 156: inventory
the same source-slot object manifest without `animation.cpp`, substituting a
freshly compiled pristine-HEAD `plugins.cpp` object. This removes the new slot
definitions and restores the old compressed descriptor's eight original
callback references. `work/rwanim/baseline-link.json` and
`work/rwanim/rw-slot-delta.json` establish **36 fewer RW unresolved slots, no
new RW unresolved slots**. 28 are newly defined source-referenced APIs; eight
were callback identities that are no longer used by the executable descriptors.
This is a compiled-reference delta, not a claim those eight original callback
addresses have new implementations. Source-only and data/vtable-inclusive
standalone trap counts are distinct.

`cmake --build build --target csf_standalone -j 8` succeeds, producing a PE32
i386 executable (not executed). Build log: `work/standalone_build.log`. All 34
new address definitions are absent from generated `build/cutover/stubs.json`;
`007C73A0` is still present. The data/vtable-inclusive executable has 286 RW
traps, which is not the source-only 126 above.

Fresh `tools/initreach.py --root 0x504970 --vtables` reaches 6774 functions and
55 traps: **22 RW**, 32 NetZ, one CRT. The earlier mission report's static
inventory had 47 RW traps. This remains an upper bound, not execution evidence;
arbitrary boundary-to-game callbacks are outside that traversal. Full output:
`work/rwanim/mission-reach.txt`.

`tools/check_all.sh --no-walk --offline` completed with exit 0. Both standalone
and hook builds succeed. The suite reports all 13 native RW tests passing,
1044 NetZ emulated calls passing, nine wmrest binary/COFF cases passing,
72 particle original/COFF cases passing, 18 audio binary/COFF cases passing,
and the datagen-text test `OK`. Windows dryrun/walk were explicitly skipped,
not counted as passes. Log: `work/rwanim/check-all.log`.

`git diff --check` passes and `git -C work/tools/librw status --short` is empty.
No changes were made to export/, db/, queue/, PLAN.md or JOURNAL.md.
