# RenderWare particle continuation (rw8b)

## Finding

The four requested address slots now have standalone boundary implementations,
not success stubs. Particle rendering is **still not implemented**. These changes
do not install library instruction hooks in the hook DLL.

Only the original executable, its exports/game callers and the unmodified MIT
librw checkout were used. No RenderWare SDK material was consulted. librw has
no counterpart for the particle toolkit or this compressed-float format.

## Per-slot changes

All four entries are **cdecl** (caller-cleaned returns in the image). Emitter
callbacks accept the callback table's third argument but these two original
emission routines only consume the first two.

* **0057CFE0**: standard particle update. Tests lifetime before advancing age;
  particles crossing their death time survive this update, then die on the next.
  Applies force/velocity, position integration, size interpolation, stable batch
  compaction, position/matrix/color/size/rotation/UV PTank clusters, cursor
  transfer, particle count and squared bounding radius. Retains the image's
  unusual in-place matrix/translation behavior rather than replacing it with
  an idealized integrator. Combined position/matrix flags reuse the last-selected
  matrix cursors and advance the shared output twice, as in the image.
* **0057DBA0**: scheduled standard emission. Wrapping LCG, count bias,
  tank capacity clamp, existing-batch/new-batch selection, batch-capacity clamp,
  lifetime/reciprocal, direction/speed/position transforms, interpolator payloads,
  identity matrices, randomized matrix axes and all six emitter cluster paths.
  Preserves RNG noncommit on early exits, allocation failure and first-new index.
  The image's matrix basis uses `at cross up`, then `right cross at`.
* **0057BBD0**: sphere shape emission over only the newest batch's new range.
  Radius/bias, angular/signed-radius RNG, LTM transform, optional radial speed,
  particle position and the PTank position overwrite relative to end cursors.
  Handles no batch, first-new sentinel -1 and empty new range.
* **0061F5E0**: original compressed-float quantizer, not IEEE half precision.
  Rounding increment, saturation, tiny-value flush, signed zero and the
  **full sign-extended EAX** result are retained. 006C2AD0 stores the low words
  but also returns the last full integer; masking the return to 16 bits is wrong.

The emitter's formerly pointer-typed reserved +24h word is now named
`boundingRadiusSquared`, with an x86 offset assertion. Ownership and class/table
layout machinery from rw8 are reused; native tests still widen pointer-bearing
properties. Original standard callbacks use offsets cached by their dispatcher;
the boundary looks up the corresponding table entries directly. Sphere's
577900 cache refresh has no need for a separate slot in this implementation.

## Verification

Original disassembly and check logs are scratch files in `work/rw8b-check/`.

* `tests/rw_particles_original.py` now includes the slice-8b fixture module.
  It compares fresh COFF callbacks against the original instructions and retains
  all rw8 cases. Each call checks cdecl stack cleanup and all nonvolatile GPRs.
  Compressed-float cases compare whole EAX exactly, including all exponent
  boundaries, signed values, NaNs/infinities and seeded random bit patterns.
* Simulation fixtures compare counts, metadata, cursor arrays and RNG **exactly**;
  floats use relative tolerance 3e-6 / absolute tolerance 2e-6. They cover each
  cluster independently, combined clusters (including position plus matrix), aliased and separate
  buffers, death/compaction and death-time crossing, all matrix transform paths,
  absent optional properties/tank, schedule early-out, full/empty batch,
  allocation failure and capacity clamps. Fresh allocation's undefined particle
  word/padding and preexisting matrix flag bits are excluded, not certified.
  Resumption added rotated, nonuniformly scaled LTM cases to distinguish point
  and vector transforms, rather than checking translation alone.
* These callback differential tests run **the original math helpers on both
  sides**. Original sqrt/inverse-sqrt lookup tables are initialized by executing
  6225B0/622690; original transform/matrix dispatchers are installed. CRT sin/cos
  use emudiff's native x87 stubs. This isolates these four entries: it does not
  establish equivalence of the earlier librw math boundary. Allocator and
  registry doubles remain explicit. No device, Windows, assets or game execution.
* Native particle fixtures exercise two complete engine lifecycles with actual
  boundary math: emission/sphere/update/compaction and matrix or non-matrix
  test-owned clusters. These are not production PTank owners or GPU draws.

Final command results are recorded in the addendum below.

## Conclusions and assumptions not verified

Game emitters are assumed to have an LTM and compatible velocity/position
properties. The image leaves position locals undefined without the relevant
velocity/LTM path; the boundary defines local/zero positions there instead of
reproducing uninitialized stack bytes. Invalid counts, arbitrary property payloads,
NaN/out-of-range color-to-integer conversions are not accepted by these fixtures.

The inherited CPU math bridge uses librw/exact square roots rather than the
original approximation tables. Native tests establish safety/lifecycle, not
original-identical trajectories through that bridge. This numerical difference
remains outside this four-slot callback verification.

Address-taken startup reachability is not proof that simulation runs at startup,
nor a prediction of the next runtime stop. Complete compressed animation
playback/stream codecs are not delivered by implementing this quantizer.

## Changed files

`src/boundary/rw/particles.cpp`, `particles.h`, new `particle_simulation.inc`;
`src/boundary/rw/plugins.cpp`; `tests/rw_globals.cpp`, `tests/rw_particles.cpp`,
`tests/rw_particles_original.py`, new `rw_particle_simulation_original.inc.py`;
this report. No changes to PLAN/JOURNAL/queue/db/export or librw, and no commits.

## Still open

No particle renderer was added. rw8's production PTank creation, format getter,
destruction and binding (582400/5821B0/5821A0/581250), PrtStd atomic create/update
(57C5D0/57C2D0) and world render setup (5865A0) remain traps. Advanced interpolation,
clone/stream callback slots also remain as before. A renderer needs a real PTank
buffer owner and librw draw conversion, not test-owned arrays masquerading as a
working backend. Windows/device/game acceptance and full animation playback
remain open.

## Final verification addendum (resumed job)

Completed with exit 0 on these sources:

* Native instrumented configuration:
  `cmake -S . -B /tmp/csf-rw8b-asan -G Ninja -DCSF_RW_CPU_SLICE=ON
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g'
  '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined'`.
  All eight `csf_rw_{cpu,life,image,object,device,toolkit9,plugins,particles}_link`
  targets build with `-j 8` and pass from `src/boundary/rw/` in that build tree
  with `ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1`.
  Logs: `asan-config-final.log`, `asan-build-final.log`, `asan-tests-final.log`.
  After updating the particle test's result label, its rebuild/run also passes:
  `asan-particles-final-build.log`, `asan-particles-final.log`.
  An initial test-run command used the wrong executable directory; it was
  corrected, not counted as a pass. LeakSanitizer remains disabled.
* `.venv/bin/python tests/rw_particles_original.py`: retained **25** rw8
  original/COFF cases and original schema/default/lock checks; **72** simulation
  cases (**24 per requested particle slot**); **5102** compressed-float bit
  patterns, all pass. Exact return/ABI/control checks and float tolerances are
  as described above. Log: `original-final.log`.
* `cmake --build build --target csf_hook -j 8`: builds `build/dinput8.dll`,
  exit 0. Log: `hook-final.log`. The RW boundary remains standalone-only.
* `.venv/bin/python tools/linktest.py -j 8 --out work/rw8b-check/final.json`:
  **14868/14868 files**, **15464 implemented function slots**, RW unresolved
  **188**. No compilation failures, duplicate definitions, raw-address records
  or assembly-address records. All four requested slots are defined
  (`slot-check.json`). Other groups: game 27, netz 154, not-a-function 4, png 1.
  Log: `link-final.log`. No claim is made about a baseline net reduction:
  the resumed tree's total inventory differs from rw8's earlier report.
* `cmake --build build --target csf_standalone -j 8`: links the PE32 i386
  GUI executable, exit 0. Log: `standalone-final.log`. Its internal source
  inventory matches `final.json`'s function/error sets. Expanded data/vtable/
  initializer inventory emits **350 RW fatal traps**, distinct from the
  source-only **188**. The four requested slots are defined in both inventories,
  not emitted as traps. The D3D9 librw backend is linked; no EXE execution.
* `git diff --check` and Python fixture `py_compile` pass. The four export
  conventions are cdecl, with two consumed emitter arguments, three update
  arguments and one float-compressor argument (`abi-check.json`); callback
  tables can still supply the emitters' ignored third argument. Original-byte
  calls independently verify stack/nonvolatile behavior. The unpatched librw
  checkout has an empty `git status --short`.
