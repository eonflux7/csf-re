# RenderWare start-up cutover (resumed rwstart job)

## Finding

Standalone selects unmodified librw's D3D9 backend, retains the game's HWND,
creates its device during EngineStart, and accepts CAMERA/ZBUFFER rasters.
The loading-clock clear/draw/present path has concrete implementations.
**No Windows execution or visible frame has been verified.** Of the requested
31 static-reachable RW traps, **two are resolved, 29 remain**; eight additional
slots reached indirectly by screen setup/presentation are new. The first loading
frame precedes the remaining particle/world initializers.

The interrupted tree was reviewed first. Its last native build failed on an
RW_D3D9-only Im2D vertex type; its hooked build was interrupted. Neither counted
as a final pass. Corrected that compilation and its fullscreen Present call:
D3D9 forbids a destination HWND override in exclusive mode.

## Changes, in requested order

1. **life.cpp:** EngineOpen **00627290 cdecl** copies HWND from the argument
   record, not its address; translates to native EngineOpenParams. Check native
   DEVICEOPEN before upstream Engine::open (which ignores its return); suppress
   only the duplicate callback and restore dispatch. EngineStart **00626D60
   stdcall** similarly checks native DEVICEINIT once, then calls Engine::start
   for module/driver construction and DEVICEFINALIZE. Librw creates/owns the
   IDirect3DDevice9, backbuffer, auto-depth surface, shaders and immediate buffers.
   Bind borrowed device/caps after Start, before game plugin constructors and
   game's post-start vtable +78. Unbind before Stop; clear HWND at Close; reject
   Stop with pipeline carriers still alive. Install slice-7 render-state/Im2D
   dispatch at Open (+20..+34), never expose native engine/object headers.
   INIT failure and FINALIZE-failure rollback tested; arbitrary upstream OOM
   is not transactional.
2. **image.cpp:** **00627770 cdecl** accepts CAMERA=2, ZBUFFER=1,
   CAMERATEXTURE=5 on D3D9 and delegates native callbacks. Preserve old facade:
   width +0C, height +10, depth +14, compact type/format. CAMERA refers to default
   render target; ZBUFFER uses default depth surface when dimensions match,
   otherwise creates a native depth surface. Zero-size camera/Z headers and
   subsequent subrasters delegate to librw on D3D9. NULL still rejects allocated GPU
   rasters, rather than pretending it has a device.
3. **CMake:** standalone links **csf_rw_d3d9_backend**, never both D3D9 and NULL.
   Boundary targets exist for Windows standalone even with test option off;
   probes remain EXCLUDE_FROM_ALL. **tools/linktest.py** compiles production RW
   slots with RW_D3D9 and checks per-object flag markers to avoid old NULL objects.
   NULL native fixtures still use librw::librw. DLL still excludes boundaries
   and librw.
4. **startup.cpp:** ten new original-address slots below. Mode output is
   **24 bytes**, not native librw's 16: bridge refresh +10 and raster format +14
   from native D3D mode list. MSAA clamps against selected colour/depth mode,
   then native setMultiSamplingLevels (upstream's query uses startMode too early).
   Only default refresh 0 is supported. Dialog policy is limited: windowed TRUE
   sets native MSAA=1; exclusive TRUE returns failure, not fake dialog support.
   Game's void pre-start vtable ignores that result. Ordinary exclusive device
   start and flags=0 presentation are not intentionally rejected by that policy;
   fullscreen dialog composition itself remains open.

### New slots in operational order

| Address | Convention | Operation |
|---|---|---|
| 00626BE0 | cdecl | six-word video-mode output |
| 0062B200 | cdecl | default refresh request (zero result as original zero input) |
| 0062B170 | cdecl | pre-start MSAA selection |
| 0062B2A0 | cdecl | limited dialog/MSAA policy above |
| 006274B0 | cdecl | current camera-raster context clear |
| 00627510 | cdecl | NORMAL/TEXTURE to camera: librw Im2D textured quad |
| 006276E0 | cdecl | immediate Present, borrowed device, HWND/HRESULT bridge |
| 00627470 | cdecl | scaled version of same composition |
| 0063CA30 | cdecl | inclusive POD BBox point query (later particle helper) |
| 00626C90 | stdcall | real available-texture-memory query; -1 unbound |

Native renderFast is NOT forwarded: it copies CAMERA to CAMERATEXTURE, not the
clock's NORMAL to CAMERA direction. Bridge binds native surfaces/viewport,
brackets BeginScene/EndScene, supplies a temporary PRIVATE native Camera only
for librw's Im2D transform, calls librw rendering, restores changed RW states and
currentCamera. Carrier never game-visible or entered in owning maps.
Clear calls native device.clearCamera with a native carrier and packed
888/8888/565 colour translation. Present checks COM HRESULT rather than treating
librw's void showRaster as success. Retained HWND is destination in windowed
mode; exclusive mode accepts only that destination and passes NULL to COM.
Device loss, resize/reset/rebind, flags=1/vsync explicitly unsupported.
Native Clear/Draw callbacks are void: BeginScene/EndScene/Present are checked,
but successful GPU Clear/DrawPrimitive is not established by those returns.

## All 31 requested traps: disposition

docs/WINMAIN_REACH.txt is absent here; this tree's initreach.py has no --root.
Carried-over work/rwstart-check/reach-before.txt has exactly 31 RW entries.
Recomputed WinMain static direct/reference closure against compiled baseline:
sets match exactly (reach-audit.log). CSF_ADDR_FN callback references included;
storing a callback is NOT executing it. Arbitrary vtable calls remain invisible.

| Phase (source-derived) | Requested addresses | Disposition / why |
|---|---|---|
| After clock, collision texture callback registration/later invocation | 0063A4D0 | Leave DDS reader: needs decoder, stream/native texture adoption; registered callback may invoke it later, not RAW-clock setup. |
| After clock/camera, 006DF810 → 006E2760 → 006E26E0 particle initialization | 005784E0, 005785B0, 0057C770, 0057C9A0, 0057CE10, 0057CE90, 0057CED0, 0057F970, 0057FA90, 0057FCF0 | Leave PrtStd/PrtAdv owning class/property/callback storage (absent in librw). Success/empty-class stubs break immediate offset accesses. First direct call in 006E0920 is 005784E0. |
| Callbacks installed there; later emission/render/batch operations | 0057BBD0, 0057C8B0, 0057CFE0, 0057DBA0, 0057EDA0, 0057EF90, 0057F050, 00580160, 005821D0, 005823E0 | Leave toolkit callbacks, property lookup, particle traversal, PTank locking: real class/batch/native-data prerequisites missing; no counterpart in librw. |
| Same callback path | 0063CA30 | Done: inclusive POD BBox query, not a particle implementation. |
| Particle callback RNG / later menu seed | 005867D0, 00586860 | Leave original engine-plugin generator/state/seed, no librw equivalent. std::rand is not its policy. Menu seed in 005113E0 follows successful 00506F70. |
| Following particle setup, streaming/visibility controller | 0058CCB0, 0058CC80 | Leave WorldCreate/sector-callback getter: sector/BSP ABI facade absent; librw's simplified World incompatible. |
| Following world/resource setup | 0058DF50, 0058DFA0, 0058DFD0 | Leave world-light membership/getter; depends on world facade. |
| Later capability query, 006DC750 | 00626C90 | Done: query real bound device, not invented NULL budget. |
| Sound/file-manager setup after AppMain_InitializeVideo | 007C5F60 | Leave mislabeled filesystem-manager initialization: globals/critical section, not resource arena. Requires subsequent filesystem-handle/archive/default-table boundary; no safe successful no-op. |

31 distinct addresses: 2 done + 29 left. Eight new slots in previous table are
absent from static trap set because screen setup/presentation is vtable-reached.
No NetZ slots or missing toolkits were stubbed.

## Expected first Windows run (NOT executed here)

Use `CSF_RW_TRACE=1 .venv/bin/python tools/dryrun.py --full` in runnable WSL/
Windows copy. dryrun now forwards optional trace variable through WSLENV.
Assuming NetZ/earlier game traps resolved, valid mode, and original asset
filesystem opens RELOJ.RAW, expected winmain.out order:

1. Static initializers finish; **00509250 WinMain** creates/shows HWND;
   **0050FBD0 → 00506F70 → 00627360 EngineInit**.
2. **006DBFA0 → 006DC000**: **00627290 EngineOpen**, `HWND=... (copied)`,
   **00626B30 SetSubSystem(0)**, **00626B90 GetNumVideoModes**,
   one or more **00626BE0 GetVideoModeInfo**, **00626C40 SetVideoMode**.
   Mode 0 is native desktop/windowed. Exclusive mode must match requested
   width/height/depth.
3. **006DC250**: optional **0062B200 refresh(0)**, then either
   **0062B170 MSAA** or **0062B2A0 dialog mode**.
4. **006DB9A0 → 00626D60 EngineStart**: nonnull librw-owned D3D9 device trace,
   actual backbuffer dimensions/colour format, auto-depth format, windowed flag,
   MSAA type. Windowed size from GetClientRect; exclusive size from mode.
5. **00627770 RasterCreate** twice: configured dimensions/depth type **2 CAMERA**;
   same dimensions/depth **0**, type **1 ZBUFFER**. Facade/native identities and
   resolved depth/format print.
6. **006DC290 → 0062B670 device**, **0062C110 caps**. Game caches real COM pointer
   at renderer +5C and adjusts writable caps snapshot +EC.
7. **006DBDB0 → 006DBA70** opens MENUS\\TEXTURES\\RELOJ.RAW via GAME file IO:
   **00625060 ImageCreate**, **006250C0 ImageAllocatePixels**, direct opaque RGBA
   writes at image +14, **00627770 NORMAL raster** (depth 0, flags 0),
   **00633040 SetFromImage**, **00625F10 ImageDestroy**.
8. **006275B0 PushContext**, **006274B0 Clear(0)**, **00627510 RenderFast**,
   **00627650 PopContext**, **006276E0 ShowRaster(window=0, flags=0)**.
   Expect `Present HWND=... HRESULT=00000000`. Black background, loading clock
   at x=8% framebuffer width, y=92% height minus clock height. Repeat sequence
   twice (three presentations), then upload RELOJ1/2/3 without presenting them
   in this routine.
9. Only afterward **00728000 → 00726C70 → 006290B0 CameraCreate**, two zero-size
   camera/Z headers, **00627710 subrasters**, frame attachment.
   Later particle setup should encounter **005784E0**, unless an earlier asset/
   indirect trap/game error interrupts it. Source expectation, NOT runtime trace.

RAW open failure skips clock presentation entirely. Separate black-frame
fallback 006DB850 uses pixel conversion/push/clear/pop/show three times; not
proved to precede clock path. Successful link/device creation is not asset IO
or frame evidence. Slice-4 Camera BeginUpdate/Clear remain NULL-only: not needed
for first clock frame, but block subsequent scene rendering.

## Verification

Logs/artifacts: work/rwstart-check/. No Windows executable, dryrun, game or
verifier was run. No export/db/queue/PLAN/JOURNAL edits; librw unmodified.

Native fixtures: instrumented librw + boundary, all six executables pass
ASan/UBSan (rw_globals.cpp is shared fixture, not another executable):

```sh
cmake -S . -B /tmp/csf-rwstart-asan -G Ninja -DCSF_RW_CPU_SLICE=ON \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  '-DCMAKE_CXX_FLAGS=-fsanitize=address,undefined -fno-omit-frame-pointer -g' \
  '-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address,undefined'
cmake --build /tmp/csf-rwstart-asan --target csf_rw_cpu_link csf_rw_life_link \
  csf_rw_image_link csf_rw_object_link csf_rw_device_link csf_rw_toolkit9_link -j 8
for t in cpu life image object device toolkit9; do
  ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=halt_on_error=1 \
    /tmp/csf-rwstart-asan/src/boundary/rw/csf_rw_${t}_link
done
```

LSan disabled as in preceding jobs. Recording fixture tests HWND lifetime,
24-byte mode ABI, INIT once/failure, FINALIZE rollback, camera/Z/RTT raster
delegation, three clear/draw/present sequences, actual librw Im2D dispatch/
vertex/state restoration, scaled composition, failing end/clear/begin/present,
flags rejection, device/caps identity and budget. GPU raster callbacks and
DeviceBackend are recording substitutes on NULL: **does not execute Windows
Direct3DCreate9/CreateDevice/COM under sanitizers**. Actual unmodified D3D9
backend/boundary are separately cross-compiled/linked.

Compiled baseline: reused interrupted job's exact COFF manifest, substituting
HEAD device.cpp object and excluding startup.cpp. Other lifecycle/raster changes
add no slot symbols. Re-inventoried:
`tools/linktest.py -j 8 --objects work/rwstart-check/before-objects.txt
--out work/rwstart-check/before-resumed.json`:
**14792/14792 files, 15256 implemented, RW unresolved 198**.
Final after/full-build results are in verification addendum below.

All ten new CCs match export/abi.json (nine cdecl, one zero-argument stdcall).
Original pseudocode/callers inspected for mode record, context/return policies,
unsupported dialog branch. ABI/source checks, not original runtime equivalence.

## Still open / unverified conclusions

Windows first device/clock frame, real asset/archive IO and indirect traps;
nondefault refresh/fullscreen dialog/vsync/reset/device loss; D3D9 camera
begin/end/clear and full scene/native-data adoption; 29 traps above, world/BSP,
particles/PTank, built-in plugin payloads; mixed game COM/native cache
coordination; arbitrary reentrancy and upstream allocation failure recovery.
No known RW fatal slot remains on audited *successful* first-clock path,
but actual first frame is unverified.

## Files changed

CMakeLists.txt; cmake/standalone.cmake; src/boundary/rw/{CMakeLists.txt,
life.cpp,life.h,image.cpp,device.cpp,device.h,startup.cpp,startup.h};
tests/rw_life.cpp, tests/rw_device.cpp; tools/linktest.py, tools/dryrun.py;
this report. Most were carried over/reviewed. Resumed work fixed native
compilation/exclusive Present, expanded failure tests/trace forwarding/details,
and completed verification.

## Final verification addendum

All commands completed with exit 0 on final sources:

* Native configure/build/test loop above: all six executables under ASan/UBSan;
  logs native-build-final.log, native-tests-final.log. INIT failure and failed
  FINALIZE/retry assertions pass, as do draw-end failure/state restoration.
* `.venv/bin/python tools/linktest.py -j 8 --out
  work/rwstart-check/after-resumed.json`: **14793/14793 compiled, 15266 implemented,
  RW unresolved 198 → 188**. Other groups unchanged: crt 2, game 86, netz 183,
  not-a-function 7, png 1. Failed/duplicate/raw/asm maps empty before and after.
  Exact-set check proves the ten removed RW slots are precisely startup.cpp's
  ten HOOK definitions; no newly unresolved RW slots (set-check.log). Baseline
  device source compared equal to HEAD with `cmp ... <(git show HEAD:...)`.
* `cmake -S . -B build -DCSF_RW_CPU_SLICE=ON`,
  `cmake --build build --target csf_hook csf_rw_d3d9_link -j 8`,
  `cmake --build build --target csf_standalone -j 8`: successful DLL, real D3D9
  backend/probe and standalone links. No concurrent Ninja builds share build/.
  build.ninja's actual standalone link inputs contain csf_rw_d3d9_backend.lib,
  not the NULL archive; all 62 backend units compile with RW_D3D9, as do private
  cached RW production slot objects. `file` identifies PE32 i386 DLL and EXEs.
* Expanded data/vtable/initializer inventory still has **350 RW fatal traps**
  (different set from source-only 188). All ten new definitions are absent from
  those traps. A linked standalone is not a full rendering/menu cutover.
* New-slot calling-convention audit passes (abi-check-resumed.json); recomputed
  31-address closure equals saved baseline, report covers each exactly once by
  disposition; `git diff --check`, Python py_compile for both edited tools pass;
  `git -C work/tools/librw status --short` empty.

No Windows/GPU/real asset execution, emulated original differential or general
leak-free claim. The first frame remains an explicit Windows acceptance check.
