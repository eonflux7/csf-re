# FR01 world framing: real-asset regression (2026-10-15)

## Finding

Both extracted FR01 worlds load through the native boundary with ASan/UBSan.
This is **not a Windows mission-load or rendering verification**.

The failure is a game-plugin size bug, not short pak reads or a new RW STRUCT
version. Sector plugin `0xFFFFFF00`'s size callback (`006B8D20`) returns
`12 + optional numTriangles bytes`; its writer (`006B8510`) and reader
(`006B8D50`) actually write/read **two** words plus that optional array.
Every sector EXTENSION and every enclosing BSP envelope inherits four phantom
bytes. Bulk-reading the declared top-level plane therefore consumes the world
extension and then runs off EOF. Retrying short reads cannot supply those bytes.

The original `0059F990` walks two children by their types. `0063CB30` counts
declared extension lengths while callbacks advance the actual stream. That is
why the original can consume these files despite the inconsistent envelopes.

## Chunk evidence

Full dumps (not checked-in asset data): `work/rwworld3/chunks.txt` and
`chunks-summary.txt`. Reproduce with:

```sh
.venv/bin/python tests/rw_chunk_tree.py \
  /mnt/e/dev/csf-paks/Ransom/Maps/FR01/{FR01.rws,FR01_col.rws,Ransom.wad} \
  > work/rwworld3/chunks.txt
.venv/bin/python tests/rw_world_chunks.py --tree > work/rwworld3/chunk-tree.txt
```

All parsed headers use stamp `1C020037`: RW **3.7.0.2**, build **0x37**.

| File | WORLD header | Declared payload | Physical payload | Difference |
|---|---:|---:|---:|---:|
| FR01.rws | `002C79D6` | 8,114,744 | 8,114,584 | 160 = 40 sectors × 4 |
| FR01_col.rws | `00000000` | 4,085,070 | 4,083,766 | 1,304 = 326 sectors × 4 |

FR01 WORLD has STRUCT(64), MATLIST(186616), a PLANE tree, then empty
EXTENSION. It has 560 materials, 39 planes, 40 leaves, 178532 vertices,
109855 triangles, flags `400200B0`. Collision has 14 materials, 325 planes,
326 leaves, 84884 vertices, 99653 triangles, flags `40000040`.
Plane STRUCTs are 24 bytes. Sector STRUCTs match the existing CPU-array formula.
Sector extensions include bin-mesh `50E`, UserData `11F`, properties
`FFFFFF00`, and (depending on world) collision `11D` / rights `1F` / `120`.
No native-sector decoding is required for these two files.

The first render leaf's property header is at `003230B3`, declares 12 bytes,
but its two words end at `003230C7`, where the next PLANE header starts.
The declaration's end (`003230CB`) is already inside that next header.

**Ransom.wad is audio, not the world container.** Its 44,318,620 bytes parse
exactly as 378 `802` containers, each with `803` metadata and `804` sample
data. The first metadata includes 44100 Hz. It contains no WORLD chunks.
`0070FC30` explicitly enumerates these `802` chunks as audio effects.
Its open appearing nearby in a trace does not identify RpWorldStreamRead's input.
`006BEC60` explicitly calls `RwStreamOpen(2,1,filename)`, consumes WORLD's
header and passes that same ordinary stream to the world reader; no bounded
WAD substream is constructed there. The native filename test follows the same
top-level traversal, including game placement records (whose own lengths omit
a 12-byte matrix header), instead of searching the file for a WORLD signature.

## Fix and immediate consumers

`world_stream.inc` stages BSP containers structurally, validates their logical
declared child totals, and normalizes only private buffered envelope lengths.
The evidenced version-1 sector property record is read at its actual length;
registered game callbacks still receive its original declared length and a
bounded actual-payload stream. Physical offsets remain unchanged, so failure
cursor restoration and following-parent-chunk preservation still work.
Unknown plugin payloads remain exact-length; this is not a scan-to-next-header
heuristic. The existing facade/material/mesh/plugin ownership path is retained.

`0x50F` sector IDs are constructed at the registered ABI offset and can be set
and retrieved after real streaming. Its geometry/sector setters were corrected
to return the assigned ID, as the original does, rather than void.

`004FF2F0`'s callback data is a contiguous 12-byte descriptor. Previously two
computed filter fields were discarded as unused locals while material/atomic
callbacks read descriptor+4/+8. They are now stored in the same buffer as the
flag byte. This is separate from fixing the null world. Boundary visitors now
reject unknown/null handles and trace their material/clump counts; they do not
fabricate a loaded world or claim success for a failed read.

Native tests exercise material/sector traversal, meshes and indices, 0x50F,
sector property callbacks/destructors, clump add/visit/remove, and teardown.
`rw_world_fr01.cpp` also tests fragmented reads and facade texture adoption with
test-double texture lookup. Neither test runs real game property managers or
proves mission texture upload. `CSF_FR01_DIR` overrides the asset directory;
absent assets print explicit SKIP messages.

## Verification

Logs are under `work/rwworld3/`.

Before/after evidence: a separate native executable linked against the HEAD
`world_stream.inc` (`baseline/fr01-before`, without changing the working source)
reproduces the **line 254** rejection on real FR01 and fails its non-null-world
assertion (`baseline/before.log`). The pre-fix `004FF2F0` source also fails the
targeted descriptor test: callback `0058CF30` sees stack garbage at +4/+8
(`baseline/descriptor-before.log`). The corresponding patched runs pass.

* `csf_rw_fr01_link` under ASan/UBSan: both real files pass; expected mesh index
  totals are 329565 and 298959. `CSF_FR01_DIR=/nonexistent` confirms the skip path.
* `csf_rw_world_fr01_link`: both files pass filename and fragmented-read modes.
* `tools/gate.py 0x004FF2F0 src/auto/004ff2f0.cpp`: `ok`.
* `tools/emudiff.py 0x004FF2F0 --src src/auto/004ff2f0.cpp`: same, 3/8 ordinary
  profiles. `tests/rw_scene_finaliser_original.py`: same 8/8 targeted profiles,
  **32** original/compiled visitor checks of the descriptor's two filter fields.
* `git diff --check` passes; the MIT librw checkout is unchanged.
* Initial NetZ reproducibility check detected only two shifted caller line
  numbers caused by the finaliser edit. `src/boundary/netz/manifest.json` was
  refreshed for those lines (35→39, 41→45); generated stub code is unchanged.
  The subsequent `netzstub.py --check --object ...` passes, including 238 COFF
  entry-sequence checks. NetZ emulation passes 1044 calls.
* The asset-independent BSP fixture still passes all 28 allocator-failure
  points, and now covers the short property record with a **present**
  per-triangle byte array, not only FR01's absent arrays.

The complete `tools/check_all.sh --offline` result is recorded in
`work/rwworld3/verified-check-all.log` (see final worker report for disposition).

The source-cache build reuses validated same-HEAD objects from the local parent
checkout for unchanged sources: `cache-seed.json` records source/object SHA256s,
all public headers matched byte-for-byte, compiler flags matched after checkout
root normalization, and parent objects passed the normal freshness check.
The hook cache additionally validates Ninja's recorded includes, input mtimes,
and full compile commands (`hook-cache-manifest.json`). Changed sources and the
changed world-stream include's consumer are compiled locally, not substituted.
These are incremental builds, not a claim of compiling every object from scratch.

The first full check found a reproducibility-only NetZ manifest drift: the two
`LookupObjectById` call-site line numbers moved in `004FF2F0`. Running
`tools/netzstub.py` refreshes those generated metadata lines; the generated NetZ
stub source and target list are unchanged.

Changed files: `src/boundary/rw/{world_stream.inc,world.cpp,plugins.cpp,CMakeLists.txt}`,
`src/auto/004ff2f0.cpp`, `tests/{rw_globals.cpp,rw_world_stream.cpp,rw_fr01.cpp,
rw_world_fr01.cpp,rw_chunk_tree.py,rw_world_chunks.py,rw_scene_finaliser_original.py}`,
`tools/check_all.sh`, and this report. Two optional real-file tests cover
different seams: full game-style filename traversal/clump membership, and
isolated WORLD reads with texture test doubles/fragmented streams.
The only NetZ file changed is the caller-line metadata described above.

## Next Windows run / open work

Expected trace for the ordinary extracted-file stream:

```text
RW startup 0x0059FEA0 RpWorldStreamRead (CPU BSP)
RW WorldStreamRead input-position=0x2c79e2 (WORLD header consumed)
RW WorldStreamRead loaded materials=560 planes=39 sectors=40 vertices=178532 triangles=109855 short-properties=40
```

Collision, if read through the same path, starts at `0xc` and reports
`materials=14 planes=325 sectors=326 vertices=84884 triangles=99653 short-properties=326`.
An enclosing pak view may report a different tell offset; counts should agree.
The former payload-read rejection and resulting null-world material traversal
should disappear. Later Windows-only failures remain possible.

Rendering slots `005903F0` and `0058CBB0` remain traps, intentionally.
`006BC310` first prewarms all sectors through `006BC2B0 > 005903F0` for six
frames, then uses `0058CBB0`. The former tail-dispatches the engine's sector
pipeline renderer. The latter walks the current camera's visible-sector cache
and renders tied atomics with per-frame deduplication/frustum tests. This librw
checkout has neither BSP sector pipelines nor that visibility-cache producer.
The boundary's one-node Rx carrier is not a sector resource-entry instancer;
passing its game pipeline into librw, or rendering all clumps unconditionally,
would not implement these contracts. Sector GPU resource translation, camera
visibility, game shader/Rx callbacks, and real GPU testing are still required.

PLAN/JOURNAL, db, queue and export were not edited; no Windows/game, network,
RenderWare SDK source or upstream librw modifications were used.
