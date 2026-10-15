# RenderWare particle/PTank continuation

## Finding

All **46 requested address slots** have standalone boundary definitions. PTank
now owns real CPU cluster buffers and an atomic/geometry/material facade, rather
than relying on test-owned lock descriptors. Its render callback assembles
immediate-mode quads and delegates to unmodified MIT librw. **No Windows, GPU,
mission-load or visual acceptance is claimed.**

`005865A0` is actually **PVS sector-render callback registration**, not a particle
simulation function. Registration is supplied; the CPU-world callback forwards
to the saved sector renderer without implementing PVS visibility filtering.
This is deliberately registration-only, as requested. No world traversal,
world streaming or world/sector render implementation was edited.

References: original executable, typed exports and game callers, plus MIT librw.
No SDK source, database/export changes, original-library instruction hooks, or
hook-DLL replacements. The added code is under `CSF_STANDALONE`.

## Per-slot identification and implementation

Names below describe the binary contracts; they are not claims of recovered SDK
symbol spelling. Every row is implemented (the last row is registration-only).
All entries are cdecl. Emitter callbacks have atomic/emitter/opaque-data arguments;
particle callbacks have emitter/batch/opaque-data arguments. Create/emit callbacks
whose original consumes only two arguments ignore the third cdecl argument.

| Address | Identified contract |
|---|---|
| 00578670 | Multi-step-UV particle key-switch update |
| 00578760 | Multi-step-UV emitter emission initialization |
| 00578BA0 | Multi-step-UV emitter clone/rebase |
| 00578C10 | Multi-step-UV emitter stream size |
| 00578C60 | Multi-step-UV emitter stream read |
| 00578CE0 | Multi-step-UV emitter stream write |
| 00578D40 | Multi-step-UV emitter property create |
| 00578DE0 | Multi-color/UV/size particle key-switch update |
| 00579000 | Multi-color/UV/size emitter emission initialization |
| 0057A090 | Multi-color/UV/size emitter clone/rebase |
| 0057A1B0 | Multi-color/UV/size emitter stream size |
| 0057A230 | Multi-color/UV/size emitter stream read |
| 0057A380 | Multi-color/UV/size emitter stream write |
| 0057A450 | Multi-color/UV/size emitter property create |
| 0057A600 | Sub-emitter particle update/nested emission |
| 0057A9A0 | Sub-emitter particle scheduling initialization at emission |
| 0057ACD0 | Sub-emitter emitter clone, clear cached child pointer |
| 0057AD40 | Sub-emitter emitter stream size (16) |
| 0057AD80 | Sub-emitter emitter stream read |
| 0057ADE0 | Sub-emitter emitter stream write |
| 0057AE40 | Multi-point emitter emission |
| 0057B280 | Multi-point emitter clone, preserve destination inline pointers |
| 0057B330 | Multi-point emitter stream size |
| 0057B3A0 | Multi-point emitter stream read |
| 0057B4A0 | Multi-point emitter stream write |
| 0057B5B0 | Multi-point emitter property create |
| 0057B640 | Circle emitter emission |
| 0057BA00 | Circle emitter clone |
| 0057BA70 | Circle emitter stream size (20) |
| 0057BAB0 | Circle emitter stream read |
| 0057BB40 | Circle emitter stream write |
| 0057BF10 | Sphere emitter clone |
| 0057BF70 | Sphere emitter stream size (12) |
| 0057BFB0 | Sphere emitter stream read |
| 0057C020 | Sphere emitter stream write |
| 0057C2D0 | PrtStd atomic update/dispatch and bounding-sphere update |
| 0057C5D0 | PrtStd atomic create, emitter/empty geometry ownership |
| 0057EDE0 | Standard emitter property clone, retain texture/copy tank configuration |
| 0057F0F0 | Standard emitter property stream read |
| 0057F450 | Standard emitter property stream write |
| 0057F7A0 | Standard emitter property stream size |
| 00582400 | PTank atomic create (capacity, format flags, platform flags) |
| 005821A0 | PTank atomic destroy (normal atomic/plugin destructor chain) |
| 005821B0 | PTank borrowed data-format getter |
| 00581250 | Emitter takes PTank ownership, allocates cursor/stride block |
| 005865A0 | PVS world sector-render callback registration, guarded/idempotent |

`work/rwprt/slots.json` lists hooks and direct game caller files;
`work/rwprt/contracts.txt` holds the original pseudocode audit (scratch, not a
checked-in dump). Many addresses are callback identities rather than direct game
calls. Existing default callback tables are preserved, not replaced by no-ops.

## ABI mapping

* Existing class/property/emitter/batch ABI headers remain in `particles.h`.
  Advanced emitter properties are `01000001` sub-emitter (20 bytes), `01000002`
  multi-color (20 + 40*n), `01000003` multi-UV (28 + 40*n), `01000004` step-UV
  (28 + 40*n), `01000005` multi-size (20 + 24*n), `01000006` point lists,
  `01000007` circle (20), `01000008` sphere (12).
* Advanced key headers contain count at +0, inline-data pointer at +4 and
  particle-record offsets from +8. Clone rebases the inline pointer; streams
  serialize count/payload, never pointers. Legacy (<3) reads repair offsets.
  The step callback switches **one** key per invocation, not a while-loop over
  every crossed key. Initial slopes and per-segment intercepts/slopes retain the
  executable's RNG order, including its initial advance before multi emission.
* Point-list +0/+4 are count/cursor, +8/+C are normal/random flags and +10/+14
  are two inline vector pointers. Disk presence/boolean words are canonicalized
  with `==1`, not general nonzero conversion. The allocation-derived point count
  follows the binary's `(propertyBytes-24)/24`, even for asymmetric schemas.
* Native fixtures widen embedded pointers without changing numeric x86 table
  offsets or stream bytes. Packed pointer accesses use memcpy, not unaligned
  pointer casts. Production x86 needs no widening.
* PTank extension is a pointer in atomic plugin 303. The pointed-to 0x138-byte
  x86 header has capacity/count +0/+4, allocation +C, lock/dirty +3C/+40,
  descriptor pointers/strides +4C..+90 and the **three-word public format at
  +98: cluster count, stride, flags**. `005821B0` does NOT return +48.
  Both structure-of-arrays (10000000) and interleaved (20000000) layouts have
  real, 16-byte-base-aligned backing allocations. Single-cluster locks preserve
  the existing original lock/unlock and dirty-mask contracts.
* Cluster bits/element sizes: 1 position/12, 2 color/4, 4 size/8, 8 matrix/64,
  10 normal/12, 20 rotation/4, 40 four colors/16, 80 two UVs/16, 100 four UVs/32
  (bits in hex). The old header's `texCoords`/`colors` field names are misleading:
  their ABI order is retained, but rendering correctly interprets bit 40 as
  vertex colors and bit 80 as UV2. Standard simulation already uses bit 80.
* Emitter PTank binding returns null if already bound, allocates input/output
  pointer arrays plus int32 strides as one block, and transfers the atomic to
  emitter teardown. Plugin destruction also handles ordinary atomic destruction.
  EngineStop's existing particle-ownership guard now includes live PTanks.
* Dispatch performs begin/update/emission/schedule columns under the original
  emitter flags, rebuilds active/inactive batch rings, initializes actual tank
  cursors and publishes count/bounds. Nested emission temporarily changes the
  parent frame and child scheduling state, then restores them. Atomic rendering
  separately visits render/batch/end-render columns.
* PTank drawing builds six vertices per quad in <=1000-quad batches (below
  librw's fixed 10,000 immediate-vertex buffer), handles matrix/billboard, size,
  rotation, per-particle/per-corner color and UV clusters, binds the facade's
  material texture, and restores changed render state. The NULL platform refuses
  to report a GPU draw. This is an independent CPU conversion, not the original
  D3D PTank pipeline or a frame-equivalence claim.
* Plugin 298's saved sector callback is at +28h; its native reservation grows
  to `40+sizeof(void*)`. Production remains 44 bytes. No PVS bitset walker or
  sector counters were implemented.

## Verified

Commands/results are supplemented by the final verification addendum below.

* Native ASan/UBSan target `csf_rw_prt_link`, automatically found by
  `tools/check_all.sh`, exercises two complete lifecycles: advanced property
  clone/rebase and stream roundtrips/truncation, standard stream fields/matrix,
  multi-key emission and one-key update, point arrays, PTank SOA/AOS formats,
  lock descriptors, actual quad coordinates/colors/UVs, allocation failures,
  emitter ownership, auto-created tank dispatch, nested emission/frame restore,
  and guarded world callback registration. Existing `csf_rw_particles_link`
  still passes. Expected upstream allocation diagnostics occur in OOM tests.
* Retail `GlobalEK/Gfx/Textures/humo02.png` was available: the native test
  decodes its 128x128 pixels, creates its raster/texture, attaches it to a real
  PTank material and checks retained ownership through destruction. The test
  explicitly skips that asset-only part when extracted data is absent.
* `tests/rw_particles_original.py` retains the 25 older callback/batch cases,
  5102 compressed-float patterns and 72 standard simulation cases, and adds
  **56 original-image/fresh-COFF cases** for seven advanced kernels: step and
  linear multi emission/update, point/circle emission and sub-emitter scheduling.
  These compare cdecl cleanup/nonvolatile registers and RNG exactly, with float
  tolerance 3e-6 relative/2e-6 absolute. Cases include schedule early-out,
  first-new -1, pre-aged particles, random/sequential point selection and normal
  directions. They use the original math helpers on both sides, and the
  existing explicit allocator/registry doubles; not Windows or a driver.
* Function-type UBSan is suppressed only at two generic callback dispatch
  sites: the binary columns intentionally carry ABI-compatible pointers with
  different C++ opaque-data types (e.g. const float* vs void*). The callback
  bodies and memory accesses remain instrumented.

## Conclusions/limits not verified

* No mission frame, GPU command capture, visual parity, or Windows run. Quad
  winding, nonidentity tank-frame/camera combinations and all device state
  interactions still require acceptance. Native fixtures verify CPU assembly,
  not D3D9 output. Other startup/world blockers are outside this task.
* The nested-emission branch and dispatch are native-tested, not original/COFF
  differential fixtures. Legacy property stream repair, texture-bearing standard
  streams and clone texture retention have narrower coverage than the main
  simulation tests. Generic emitter atomic plugin streaming/cloning is not
  supplied by these property callback slots; the existing populated clump clone
  rejection is preserved.
* The boundary rejects malformed counts, overflow and short streams instead of
  reproducing the original unchecked writes/ignored stream failures. Missing-LTM
  shape locals are defined rather than reading uninitialized original stack
  storage. Class schemas, valid pointers/cursors and non-reentrant lifecycle are
  assumed. Arbitrary callback ring mutation/destruction is not certified.
* PVS filtering remains absent: registration wraps/delegates only. This is not
  an implementation of the original 005864C0 visibility filter.
* The inherited native math bridge differs from the original approximate math
  tables. Original/COFF kernel comparisons deliberately isolate those helpers.

## Files changed

`src/boundary/rw/particles.cpp`, `particles.h`, `plugins.cpp`, `CMakeLists.txt`;
new `particle_{properties,stream,advanced,tank,dispatch}.inc`;
`tests/rw_globals.cpp`, `tests/rw_particles_original.py`;
new `tests/rw_prt.cpp`, `tests/rw_particle_advanced_original.inc.py`; this report.
The test globals also supply two missing existing plugin-ID cells (008B1404/08)
needed to link this worktree's native probes. No world source files changed.

## Final verification addendum

Pending completion of the full offline build/inventory commands at report draft.
