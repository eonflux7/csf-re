# Scattered RW boundary slots (2026-10-15)

## Finding

**Partial: 17 of the requested 18 slots have boundary implementations.**
`007C73A0 RtSkinSplitAtomicSplit` still traps: no successful no-op or fabricated
split metadata was substituted. The other implementations have native tests;
Windows execution, water-shader rendering and campaign loading are not verified.
The DirectSound import deliberately returns **DSERR_NODRIVER**, with a null
capture output. Offline single-player does not provide voice capture.

No world-loading/rendering files, particle/toolkit implementations, librw sources,
export/, db/, queue/, PLAN.md or JOURNAL.md were edited. References were the
original executable/exports, compiled game callers, and unmodified MIT librw.

## Per-slot contracts and ABI mapping

All entries are standalone-only, address-keyed slots. Extra cdecl arguments in
some generated callers are stale stack values, not additional parameters.

| Slot | Status / mapping |
|---|---|
| `00571270 RpMatFXAtomicEnableEffects` | Atomic facade → native MatFX/pipeline; publish the enabled integer in the ABI atomic plugin tail. Repeated enable preserves the current enabled pipeline. Null/unregistered objects fail. |
| `00571A80 RpMatFXMaterialSetEffects` | Material facade → owning native MatFX storage, engine-allocator failure checked before upstream's unchecked allocation. Effects 0–6, clearing old effects/texture ownership delegated to librw. Invalid effects fail rather than installing unsupported state. Native owning MatFX headers are not published into ABI tails. |
| `005774B0 RpMorphAtomicAddTime` | Original atomic/geometry morph plugin records, geometry-cache refresh, loop/advance callback, and original atomic interpolator start/end/time/reciprocal/position/dirty fields. Callback receives the atomic facade; frame dirtiness goes through the existing object bridge. Missing geometry returns null; no sequence leaves the atomic unchanged. Native tests widen pointer fields and use memcpy because plugin offsets remain four-byte aligned. |
| `005860C0 RpPVSSetViewPosition` | Original PVS globals and sector plugin fields, ABI BSP traversal, first-sector fallback and last containing-sector selection; clear each leaf's visibility word. Returns the selected **sector**, not a boolean. |
| `005861F0 RpPVSWorldUnhook` | Restore the saved sector callback through the existing world callback setter (including its default-callback behavior), clear hooked/saved fields, return the world facade. |
| `0058FDF0 RpClumpStreamWrite` | Boundary-owned unique geometry/frame indexing; librw frame/material/geometry/light/camera and plugin codecs. Shared geometry is serialized once, not once per atomic; game write callbacks likewise run once for that geometry. Output is buffered before checking/writing the destination. Game callbacks receive facade objects, ABI offsets and writable borrowed streams. Null/failing callbacks, bad frame references and short output return null. |
| `0059B990 _rwD3D9VSSetActiveWorldMatrix` | Copy the compatible RW matrix's basis/translation into an affine RawMatrix, discard flags/padding as D3D matrix coefficients, null selects identity. Return value is unused by the game. |
| `0059BA20 _rwD3D9VSGetComposedTransformMatrix` | Output **transpose(world × camera.devView × camera.devProj)**; null world omits that product. Uses the current native librw camera's D3D9 matrices, not ABI camera view data or boundary SetTransform snapshots. librw's D3D9 beginUpdate includes its X-handedness flip, view offsets and D3D depth projection. No current camera/output returns null. |
| `0061EF90 RtAnimAnimationStreamGetSize` | Scheme getSize callback +20, with builtin standard/compressed on-disk sizes (36 / 22 bytes per key, plus compressed custom data), not native pointer-width-dependent keyframe sizeof. |
| `006269C0 _rwDeviceSystemRequest` | Do not call an original device table pointer. Map live subsystem requests to librw's differently numbered DeviceReq values and preserve original fallback behavior for 13–18. Request 22 writes the **16-bit** D3D9 dictionary device ID 2; unknown requests fail. The unused original input argument is retained in the signature. |
| `0062F950 RwD3D9SetTexture` | Texture/raster facade → native texture; production calls librw d3d::setTexture so stage addressing/filtering/cache/alpha updates remain native. Null unbinds, stages 0–7 accepted. NULL fixture only delegates stage-zero raster state; it is not a GPU test. |
| `006308E0 RwTexDictionaryStreamWrite` | Native dictionary/native texture codecs plus facade game plugin write/size callbacks, buffered destination and facade/null return. |
| `00630A20 RwTexDictionaryStreamRead` | Validate bounded D3D9 chunk envelopes, versions, dimensions, formats, mip counts and exact pixel payload sizes before native decoding. Compose native texture/plugin codecs with an **unsigned** dictionary count (upstream dictionary reader uses signed int16). Transactional allocation/ownership rollback, game plugin callbacks and facade dictionary/texture/raster adoption. Unsupported platform/cube-map formats fail. |
| `0063BD90 RwMatrixStreamGetSize` | 64-byte payload constant (outer header excluded). |
| `0063BDA0 RwMatrixStreamWrite` | Original MATRIX/STRUCT envelopes, 52-byte basis/translation/flags payload, original 3.7.0.2/build-55 stamp; normalize orthonormal-flagged matrices through the existing CPU slot, preserve translation; matrix/null return. |
| `0063CB30 _rwPluginRegistryReadDataChunks` | Translate original registry **identities**, not their original linked-list/function pointers. The two direct game callers use texture `008A6410` and dictionary `008A6428`. Native and game readers run in bounded transactional extension streams; unknown chunks skipped; failed/short callbacks fail; success returns the **registry** identity. Other direct registry identities explicitly fail. |
| `007C73A0 RtSkinSplitAtomicSplit` | **Not implemented.** Original mutates triangle/material grouping, builds bone-palette remapping/RLE and rebuilds meshes; librw has storage for split streams but no splitter or D3D9 consumer of per-mesh remap/RLE. Merely setting remapIndices, or returning success without that geometry/mesh transformation, is not this contract. See below. |
| `007F78EC DirectSoundCaptureCreate` | stdcall with three stack arguments (the generated fastcall caller's ECX/EDX are irrelevant to this import thunk). Return `0x88780078` / DSERR_NODRIVER and clear output; no dsound.dll dependency or COM capture facade. |

### Additional integration/fixes

* Engine facade +B0 now has a native-texture-read adapter, needed by the game's
  `006C1260` merge reader. It consumes the native texture struct but leaves the
  extension for the subsequent `0063CB30` call, exactly as that caller expects.
* Existing clump plugin adapters gained write callbacks and dictionary registry
  support. Their read allocation journal is reused for dictionaries and direct
  registry reads. Mutable codec version/build settings are restored on exit.
* Native TXD decoders allocate textures before assigning their raster. Tests
  found that exporting the early texture facade left raster null; the later
  inward conversion then erased the native raster pointer. Completed native
  raster adoption and outward texture synchronization now repair that lifecycle.
* Native fixture cells for existing geometry/sector ID globals were missing;
  tests/rw_globals.cpp supplies them along with the new PVS/registry identities.
* Morph/PVS plugin reservations widen only for 64-bit native fixtures; x86 sizes
  remain unchanged. Production header offsets and owning identity maps remain
  the existing ABI facade's responsibility.

## Tests / reproducibility

`tests/rw_misc.cpp` is wired as `csf_rw_misc_link`, discovered automatically by
`tools/check_all.sh`. It uses ASan/UBSan like the existing RW fixtures. It tests
non-symmetric world/view/projection composition and transpose, null world,
matrix stream translation/envelope, all MatFX effect kinds and allocation failure,
morph callback wrapping/interpolator fields, PVS leaf/callback fields, device ID,
DSERR_NODRIVER, shared-geometry clump write/read, dictionary native pixels,
game-owned extension read/write callbacks, direct merge native-texture and
registry paths, roundtrip ownership/teardown, and truncated dictionary rejection.
Additional real `.txd` filenames can be passed as positional arguments.

Synthetic D3D9 TXD input is a complete native-texture chunk with a 2×2 RGBA
payload, not a mock returning a precreated object. Native pixels are checked
before serializing and reading it again. It is **not retail asset evidence**.

The requested real mission `.txd` test could not be performed: neither the
three extracted trees nor the installed retail PAK indices contain a `.txd`
entry. Inventory is recorded in ignored `work/rwmisc-assets.json`: **29 PAKs,
29,158 indexed entries, zero TXD entries; zero TXDs in the extracted trees**. FR01 textures
in the supplied Ransom extraction are DDS files; the WAD is a different container,
not a TXD. No synthetic dictionary generated from DDS is misreported as a real
game TXD. A game-generated cache dictionary is still needed for that check.

Commands/logs (final results recorded below):

```sh
tools/check_all.sh --no-walk --offline
.venv/bin/python tools/linktest.py -j 8
cmake --build build --target csf_standalone -j 8
ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 \
  work/rwcheck-asan/src/boundary/rw/csf_rw_misc_link
git diff --check
```

Logs: `work/rwmisc-check-all.log`, `work/rwmisc-linktest.log`,
`work/rwmisc-standalone.log`, `work/rwmisc-native{,-build}.log`.

## Still open / unverified conclusions

* Skin splitter: implementing an honest split requires a mesh/palette policy,
  material/triangle regrouping and its render-path integration. The original
  `006C4DF0` caller ignores failure but checks `RpSkinIsSplit` on subsequent calls;
  fabricating that predicate would silently suppress real work. This patch does
  not resolve the outstanding split/pipeline-layout gap from RW_ANIMATION_REPORT.
* No Windows/mission/GPU/voice-capture execution. The matrix test checks supplied
  native device matrices, not a live D3D camera or shader. Production D3D9 compile
  evidence is distinct from native NULL-backend execution.
* Real TXD compatibility remains open; reader currently accepts D3D9 textures,
  not arbitrary D3D8/console/cube-map dictionaries. A hostile-input/native-plugin
  audit and exhaustive dictionary/writer OOM sweep are not claimed.
* Arbitrary original registry identities, original-error-stack side effects,
  and full native MatFX owning-header ABI publication are outside this slice.
* The morph slot updates the original interpolator/plugin state. It does not
  establish a render-time morph-target blending pipeline in librw; similarly,
  PVS state tests are not evidence of a live PVS culling/render pipeline.

## Final build / link evidence

All 15 native RW fixtures passed with ASan/UBSan in the final native-target
sweep (`work/rwmisc-final-csf_rw_*_link.log`); the misc fixture is also rerun
separately after its final edits (`work/rwmisc-native.log`). The seven changed
boundary translation units compile under linktest's production D3D9 flags
(`work/rwmisc-boundary-compile.log`). `git diff --check` passes and the MIT
librw checkout remains unchanged.

Full link/build command results pending completion.
