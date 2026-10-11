# Journal

Dated findings, newest last. Keep entries short; put conclusions into PLAN.md.

## 2026-10-07 — Phase 0 symbol harvest

- PS2 prototype (TLES-52768, 2005-02-28): `TLES_527.68` and `COMMX_FR.ELF` are stripped loaders. Game code is in
  Metrowerks `MWo3` overlays `FINAL.BIN` (SP) and `FIN_RED.BIN` (MP, links Quazal NetZ). No symbols; a few assert
  file names (AudioDriverGeneric.cpp, Image.cpp, Quad.cpp, GfxLine.cpp, StlAllocator.h).
- PS2 retail (2006-03): `LOADER.00` has a symtab + .mdebug but it's a generic loader/newlib. `ENG.00` stripped.
  `FINAL.BIN`/`FINXX.BIN`/`FIN_RED.BIN` overlays again.
- Xbox retail: `eng.xbe`/`rus.xbe`/`default.xbe_orig` (game), `Default.xbe` is a CDX launcher. Strings give
  `c:\CommX\Src\Main\_out\Xbox-XBox Final\CommX_Final.exe`, PDB name `CommXPC.pdb`, ~100 .cpp names. No PDB.
  Extracted with extract-xiso (built in `work/tools/`).
- **Conclusion: no symbols anywhere.**

## 2026-10-07 — PC exe fingerprint and triage

- MSVC 7.1 (Rich: VC7.1 3077 / 4035 builds), linker 7.10, timestamp 2006-02-23. Sections: .text, _rwcseg,
  .rdata, .data, _rwdseg, .rsrc. Not packed.
- RTTI: only type_info/exception/std::logic_error family → game compiled with /GR-.
- FLIRT (auto) recognized only 524 CRT functions.
- Function alignment splits the binary: game, RW and NetZ are 16-byte aligned; D3DX9 (0x5a1d51–0x61de63) and the
  CRT are unaligned (prebuilt MS libs).
- `RwEngineInstance` = 0x8b68c0 (202 refs in RW D3D9 driver range); its users locate RW core (0x566000–0x5a1d50),
  RW D3D9 driver (0x61e0c0–0x646000) and RW toolkits (0x7c4000+ / 0x7d8000+).
- Draft totals: game ~14k funcs / 2.2 MB; rw 2.8k / 668 KB; netz 4.1k / 545 KB; d3dx9 1.2k / 481 KB.
  Game→lib boundary: ~470 RW, ~400 NetZ (inflated by misclassified ranges), 61 CRT.
- Leaked RenderWare SDK source exists on GitHub; deliberately not used (licensing taint). librw instead.

## 2026-10-07 — Bulk export, clones, vtables

- Hex-Rays export of all non-lib functions: 13,999 ok / 2 failed, ~2 min (`tools/ida/export_pseudo.py`).
- Clone clustering (`tools/clones.py`, exact normalized-instruction hash): 813 game-only clusters with 3,040
  members; one impl per cluster would save ~216 KB (~10% of game bytes). Biggest: 188× scalar-deleting
  destructor pattern → **`operator delete` = sub_691110** (game allocator in 0x690000 core region).
- No RTTI, but ~108 classes implement a QueryInterface-like virtual: `if (id == &TypeId) {*out=this;} else Base::QI()`.
  Chains give hierarchy (depth ≤ 3). Type ids are bare BSS addresses (no names).
- `tools/vtables.py` → `db/vtables.json`: 1,214 vtable candidates (1,049 game, 120 netz).

## 2026-10-07 — Region refinement

- `tools/regions.py` rewritten: call-graph closure (callers of game are game; callees of libs are lib) + string
  seeds + address-neighbour fill. 0x7c65c0–0x82c403 (420 KB, no seeds) calls out only to rw/netz/crt and is
  called by game → library (RW toolkits, libpng/zlib, NetZ tail).
- Final draft: game 14,422 funcs / 2.3 MB; rw 2,737 / 635 KB; netz 3,777 / 446 KB; d3dx9 1,241 / 469 KB.
- Boundary API (lib functions called directly by game): rw 455, netz 319, crt 105, png 1 → `export/boundary_api.json`.
- NetZ boundary is large (319): game net layer (0x740000 DDL classes etc.) is deeply tied to NetZ.

## 2026-10-07 — Hook DLL bring-up

- Toolchain: clang-cl 21 / lld-link, `--target=i686-pc-windows-msvc`, xwin x86 libs; static CRT. Linker map at
  `build/dinput8.map` (used to compare our compiled `ret N` with the original).
- Game imports only `DirectInput8Create` from DINPUT8.dll → `dinput8.dll` proxy is the loader.
- First hook: `sub_44FC30` = `CHandleTable::Resolve(const CHandle*)` (464 callers, generational handle lookup;
  entries @+0x10, count @+0x18, object generation @+0x20). Compiled output matches original instruction-for-
  instruction (signed compares, `ret 4`). Game uses frame pointers (EBP frames).
- Smoke test (`tools/smoke.sh`): PowerShell Start-Process/WaitForExit/Stop-Process by PID (tasklist-based
  version mis-detected exit and left the game running). User allowed free smoke launches.
- Hit counters: each hook routed via `inc [cnt]; jmp fn` stub; counts in `csf_hook_hits.txt` every 3 s → coverage.
- sub_44FC30 not hit in the first 20 s (gameplay only). Startup proof: sub_697300 (options flag setter, 79 hits)
  and sub_697450 (`std::string::c_str` on options object) reimplemented and hit.
- VC7.1 std::string layout confirmed from code: alloc@0, _Bx[16]@4, size@20, res@24 (28 bytes).
- Global options object pointer at 0x8BAF8C.

## 2026-10-07 — Grunt pipeline + model A/B

- Driver `tools/grunt.py` (packet → model → gate → src/auto/<ea>.cpp → link/deploy/smoke → commit). 117 functions
  reimplemented so far, all smoke-tested in game. Trivial ones ~1.5k tokens/task on DeepSeek; cost negligible.
- Found + fixed: bare string literals break pointer identity → `CSF_STR(addr, text)` + gate ban; missing `;` after
  HOOK auto-fixed; functions with <5 bytes room (8 total) can't be JMP-hooked → excluded.
- DeepSeek flash: thinking is on by default; `max_tokens=8000` starved medium functions (8000 reasoning tokens, no
  answer). Now `thinking:{type:enabled}`, `reasoning_effort` high (retry: max), `max_tokens` 64000.
- Claude backend = Claude Code CLI headless (`claude -p --tools "" --system-prompt …`, user's subscription; no API
  key). `--bare` breaks subscription auth — don't use it. Runs from ~/.cache/csf-re-claude (outside repo).
- A/B on 16 medium functions (250–580 B): DeepSeek 15/16 gate-pass, Sonnet 16/16. Manual review of 2:
  0x455C50 (vector<T48>::reserve) both correct. 0x72AE00: DeepSeek cached a vector element across a virtual call
  where the original reloads (risky: children removing themselves); Sonnet reloaded correctly but added a bogus
  extra read. → prompt rule "re-read memory where the original does"; asm now included up to 400 B.
- Gate now also checks every CSF_FUNC binding's convention/arg bytes against the callee's `ret N`.

## 2026-10-07 — FLIRT false positives, STL instances

- 61 tiny game functions (getters etc.) carried FLIRT names from VS2010 ConcRT (`Concurrency::...`) and were
  filed as `crt`. triage now lets the address region win over the FLIRT flag; bogus names cleared.
  CRT boundary count in `export/boundary_api.json` is stale (regenerate before naming it).
- `tools/stl.py`: seeds = refs to VC7.1 STL error strings; all `vector<T>` instances share one COMDAT-folded
  `_Xlen` (0x6CE410, 182 callers); std::string members are game-side instantiations calling the CRT's
  `_String_base::_Xlen/_Xran`. Plus clone expansion and template-shaped exclusive callees (uninitialized_fill_n,
  copy_backward, _Destroy ranges, tree _Min/_Max/_Lrotate). → 418 functions / 119 KB, module=stl, skipped by grunt.
- Tree node: _Isnil byte at +45 for one instance (value 33 bytes after 3 ptrs + color) — offsets vary per T.
- STL containers allocate via `sub_690DC0(size, file?, line?)` / `sub_690E60` (game allocator; cf. PS2
  `StlAllocator.h`), not operator new.
- CRT range had a 16-aligned island 0x7C1C70–0x7C2C2F: six clip helpers called only by game 0x6EBA90 (→ game)
  and two called only by RW 0x7FF600 (→ rw). regions.yaml updated. Remaining CRT boundary (58) is all named;
  hand-named: std::exception ctor/dtor (vftable 0x876A50), _wsystem, __security_check_cookie (/GS is on).

## 2026-10-07 — Codex backend

- DeepSeek account hit HTTP 402 (out of credit). Added `codex` backend: `codex exec --ephemeral
  --skip-git-repo-check --ignore-user-config -s read-only -o <file> --json -`, run from ~/.cache/csf-re-codex.
  ~15k prompt tokens of Codex overhead per call (mostly cached), ~6 s for a trivial answer.
- First batches: 6 tiny + 8 medium (200–600 B) all gate-passed, 7/8 medium on first try, ~2–3k output tokens.
  Manual review of 0x525060 and 0x427FD0: faithful, incl. x87 double intermediates and NaN-branch semantics of
  the original compare. Now the default main grunt.
- RW naming (`tools/rwname.py`) started; trial results in db/rw_names.jsonl. 0x624D10 (232 game callers) frees to
  a module freelist via RwEngineInstance+0x11C → RwMatrixDestroy-like.
- RW naming pass 1 (Codex, high effort): 392/456 answered before the subscription hit its usage limit (then
  403s). Core RW ranges look solid (RpHAnim block 0x56CD30–0x56DB50, RpMatFX 0x571270–0x571A80, conf 0.9+).
  0x7C6000+ answers were low-confidence and collided with core names (RwEngineStart at 0x626D60 and 0x7DB920)
  → that region is probably not RW proper. Applied 254 names; rest held in db/rw_names.jsonl.

## 2026-10-07 — RenderWare Audio found; region rules

- 0x7C6010–0x7F78F2 contains RenderWare Audio interleaved with RW toolkits and libpng: one call-graph component
  holding all DirectSound/ole32 (CoCreateInstance, CoTaskMem*)/timeGetTime users, plus `rws;1` string.
  581 functions / 92 KB → region `rwa`. The model had named its entry points RwEngineStart/Close/Term — wrong.
  0x7D8000–0x7DBE90 and 0x7F8000+ are genuine RW 3.7 toolkits (RtQuat, RtIntersection, RtDict, RpMTEffect, Rt2d).
- triage: per-function overrides (db/region_overrides.json) + "tiny lib stub calling game → game" rule.
  A naive "lib calls game → game" fixpoint flipped 2,560 NetZ + 620 D3DX9 functions: the game replaces global
  operator new/delete (every lib calls game allocators), and ICF folds 1–5 B nullsubs/return-0 shared by
  game and libs. Rule now: ≤ 24 B, one pass, ignore allocators and callees ≤ 8 B. 0x828000+ static-init stubs
  mostly call folded no-op ctors → left as lib.
- RW naming pass 2 (Codex, neighbour/callee names from pass 1 in context): 118 more applied → 372/416.
  44 held: mostly adjacent pairs collapsed to one name (e.g. 0x7FB000/0x7FB010 both "RwImageSetWidth").
- Grunt batches 3–4 (100 + 200 fns, ≤ 600 B): 300/300 gate + smoke. Leaf-first ordering still yields almost only
  < 48 B functions → DeepSeek did 298, Codex rescued 2. 200 functions in 3.7 min.

## 2026-10-07 — Direct-to-mission smoke test (devcmd), MinHook, semantic gate

- Semantic gate (`tools/semcheck.py`): compiled object's absolute references vs the original's callees, strings,
  writable globals, vtables, field displacements (follows tail jumps into shared blocks). 0 false positives on 440
  accepted files after tuning; found 2 real bugs (0x6A8320/0x6A8D00 allocated a NetZ object on the game heap —
  caused by our own prompt rule "use Mem_Alloc"; rule fixed).
- Game startup facts: "command line" comes from Juego.cfg `.commandLine` (CDevCommandLine 0x4FB8F0/0x4FB850:
  -scene/-log/-instance; -instance = offline instancing tool mode, exits). bFullScreen = byte +284 of
  dword_8BC1DC (read before window creation). Main loop exits when the menu manager has no active screen.
- Menu manager vtable 0x864508 (slot 22 GotoScreen 0x71C2A0, 23 FindScreen). Screens are .fbs names
  (Menus/P_*.fbs). P_Juego = the game screen object at app+24 (CGameScreen, vtable 0x843720), its update 0x538160
  (state +308: 0 start / 1 campaign menu / 2 in game). With CGameFlow (dword_8B0568) state +92 == 1 it prepares
  (0x501B90) and starts (0x504970) the scene; briefing gate = vslot 35 0x5364D0.
- CGameFlow: SetScene 0x501990 (+124, name = .scn path e.g. Maps/FR01/Parachut.scn), SetLoadGame 0x501A00 (+152,
  full path); save manager (vtable 0x840920, at game+4) slot 11 = select for load. Save folder at intro time is
  Documents\CSF Data\Games\ (profile subfolder Games<N> appended later).
- Video: render device vtable 0x861460 (slot 8/9 width/height, 25 HasModeFor, 27 SetupVideoMode 0x6DC000);
  windowed RW back buffer follows the client area; window sized by 0x508F50.
- `src/core/devcmd.cpp`: ini `scene PATH` / `loadsave NAME` (first screen change → set CGameFlow, go to P_Juego,
  no briefing) and `windowed WxH`. smoke.sh now starts in-mission: 142/444 hooks hit (was 54 at the menu).
- Hooks: MinHook vendored (third_party/minhook, BSD-2) for HOOK_TRAMP (wrap + call original) and `probe 0xADDR`
  (pass-through hit counter; great for tracing which originals run). tools/shots.sh = window-only screenshots.

## 2026-10-07 — Differential testing harness

- `hook/diff.cpp` + `tools/difftable.py`: leaf functions whose writes go only to `this`/absolute globals (423/440
  done) run original (MinHook trampoline) and ours on the same live inputs; return value (masked to the defined
  width: al/ax/eax/edx:eax/st0) and memory compared; ours stays. In-mission smoke: ~125 compared, 0 mismatches.
  smoke.sh enables it by default and prints MISMATCH lines; grunt sends mismatching new functions to queue/.
- Pitfalls found: (1) never read past the thread's stack top when copying args (TEB StackBase cap); (2) clang-cl
  miscompiles naked functions with non-void returns (adds prologue / sret code) — declare naked thunks `void()`
  and call through typed pointers; (3) stubs must restore every register except the return registers: write
  edx only for 64-bit returns (callers may keep live values in registers an MSVC leaf doesn't touch — the same
  hazard exists for normal JMP hooks whose reimplementation clobbers ecx/edx where the original didn't; watch).
- Byte/bool getters (`mov al,[ecx+X]; ret`) leave eax's upper bytes as garbage: compare al only.
- ini_string() now caches the ini once (the old 16-slot buffer silently ran out after 16 lookups).
- Crash triage: vectored handler logs CRASH lines; Windows Application log (event 1000) gives module+offset when
  the handler can't run; map offsets via build/dinput8.map.
- semcheck: skip hook-infrastructure symbols (Registrar ctor no longer inlined) for field offsets; collect
  addresses also from 4-byte .rdata pointer sections (CSF_GLOBAL bindings).
- Debugger harness: x64dbg 2026.05.27 + x64dbg-automate 0.8.1 (32-bit plugin in release/x32/plugins), Python
  client in a Windows venv (E:\dev\tools\x64dbg-venv). `tools/dbg.sh crash [secs]` runs the play copy under
  x32dbg, continues through non-fatal stops, and on the first fatal exception prints registers, asm at eip and
  the stack, symbolized (game via db/functions.csv, our relocated DLL via the logged base + build/dinput8.map).
  ini `crashtest 1` triggers a deliberate fault to test it.
- IDA code-view export (export/disasm/, game region, 9 s): 318 functions have switch tables (bytes linear capstone
  decoded as junk), 560 have multiple chunks. packet.disasm() now decodes only IDA's code heads, so packets,
  semcheck and difftable see real instructions; linear decoding remains for shared blocks / missing exports.
  Regenerate after IDB changes (export/ is derived).

## 2026-10-07 — Lanes trial, embedded-object bug class

- Lanes trial (DeepSeek tiny/clone lane + Codex GPT-6.1-Sol main lane): 45 functions, smoke crashed in-mission.
  `tools/dbg.sh crash` pinned both crashes to reimplementations that turned `add ecx, N` / `lea r, [ecx+N]`
  (address of an EMBEDDED sub-object at this+N) into a pointer LOAD from +N (0x6B2F40 +0x30, 0x6FD170 +0x10).
  Fixed by hand; after the fix smoke OK, 134 functions diffed, 0 mismatches. Both are hot (635k calls / 60 s).
- Prevention: grunt prompt rule spelling out add/lea = address-of vs mov = load; semcheck now requires an add/lea
  with +N in our code for every `add ecx,N` / `lea r,[ecx+N]` in the original (stack regs excluded: `add esp,N`
  is cleanup). Rejects the buggy variant, 0/485 false positives on the survey.
- x32dbg: default stops (system/entry/TLS breakpoints) are ini settings read at startup; crash.py turns them off
  in x32dbg.ini before launch (faster start, fewer continue round-trips).
- 15 lanes outputs had no grunt_log entry (from an interrupted run); names recovered from their HOOK lines.

## 2026-10-07 — Tail-jump thunks, BOOL width, STL in the pipeline, monitor

- Bug class: indirect tail jumps (`jmp [reg+N]`, `jmp [abs]`) forward the caller's stack args/registers to a target
  whose signature is unknown. Grunts wrote them as calls ("no stack args involved") → the target's `ret N` pops our
  frame (0x6C5B20, 0x4F5170 crashed in-mission; eip on the stack, ebp = a code address, so the debugger only shows
  the victim — found by bisecting with ini `disable`). 45 functions had it latently; converted to naked asm thunks
  generated from the original instructions. semcheck now requires an indirect jmp in ours; the gate allows asm only
  as a naked thunk (mov/lea/add/sub/call reg, then jmp).
- `xor eax,eax; setcc al` is a 32-bit BOOL return; `bool` leaves eax's upper bytes undefined (0x47F9B0, caught by
  the diff harness, 14/14 mismatched). Prompt rule added.
- STL (418 fns) now go through the grunt with a VC7.1 layout note. semcheck: VC7.1 `max_size()` is -1, so
  `cmp n,-1; jbe; call _Xlen` is dead code clang folds away → not required; compiler EH helpers not required; EH
  funclet rets ignored by the ret check.
- Codex slim harness: our system prompt as `model_instructions_file`, agent features off: overhead 14k → ~4k
  tokens/call, same or better pass rate (A/B 4/8 vs 3/8). pi agent not needed (DeepSeek is already direct API).
- tools/monitor.py: rich dashboard (Claude job state from ~/.claude/jobs, grunt live state work/grunt_state.json,
  Codex 5h/7d from the ChatGPT usage endpoint the CLI's /status uses, DeepSeek balance API).
- Lanes: the Codex lane starved (candidate window = 600 leaf-first, nearly all tiny) → window 4000.

## 2026-10-07 — Phase 2 started early: class naming pass 1, IDB sync; lanes run unattended

- Class names come from four kinds of evidence (tools/classinfo.py prints them per vtable):
  - GUI: the .fbs parser 0x71DBE0 maps keywords (BOTON, ESTATICO, SLIDER, LISTBOX, ...) to CGuiBuilder vtable
    slots +0x18..+0x44; each slot's factory allocates exactly one control class → 12 control types + their base.
  - Menu screens: each derived screen finds its controls by name under `.PANEL_PRINCIPAL` (BotonCampanya,
    ListboxSaveGames, SliderMusic, ...), which identifies the screen.
  - AI: the T* classes return their own name from a GetName virtual (slot 33 of 0x83DE60 tasks, slot 8 of
    0x83E050 states): TNavegando, TDisparar, TAlerta, TCombate, ... — the original names (attested). The type
    registry at 0x8442A0 (CVector3, SRect, CInterpolatorHermite3D) confirms the convention: C classes, S structs.
  - NetZ DDL: DOClass names in the generated code (NetPlayer, NetGame, NetGhost, RootDO, Session, ...).
- "Desconocido" in a method is the allocator's default tag (0x6910F0), not evidence. Grunt class_guess and
  field_names proposals are too generic to use ("CUnknownObject", "m_value0218").
- Grunts typed base-class methods with the first derived class they met (43 files said C_0082fbe8 for TCharTask
  methods). The rename pass prefixes each method with its owning class (most-base class containing it); functions
  in unrelated hierarchies' vtables (COMDAT-folded) keep their prefix.
- classes.json had 8 base cycles (a dtor with an inlined base dtor read as a ctor); `methods` is a sorted set
  including non-virtual this-calls, so slot order must come from db/vtables.json.
- Layout issue left open: a base absorbs fields accessed through derived objects (C_00830438 holds the GUI
  control's fields; CGuiControl itself is empty).
- First IDB sync: 1244 function names, 782 summary comments, 103 globals, 116 vtables, 662 local types; struct
  sizes match the static_asserts.
- Lanes: smoke failure → automatic bisect via ini toggles (control run first; ~11 smokes for 1 culprit in 60),
  `work/grunt_stop`, Codex quota guard, review verdicts logged. CSF_AT accepted no const pointer (2 grunt
  compile failures).

## 2026-10-07 — auto-re-agent review: negative controls, verification ledger, vacuous smokes

- Reviewed github.com/Dryxio/auto-re-agent (generic Ghidra + LLM reverser/checker loop; candidates only, text-level
  verifier, Unicorn diffs on hand-picked cases). Ours verifies far deeper (object-code semcheck, live diffs, hooks);
  borrowed: negative controls, coverage/staleness tracking; queued: bounded evidence requests, waivers, full logs.
- `tools/controls.py` (static ~10 s): all 36 fixtures caught after two gate fixes it exposed:
  - BOOL width was invisible statically: semcheck now reads the MSVC mangled return type (`YI_N` = bool) when the
    original zero-extends a setcc result. 0/841 false positives.
  - A function ending in a direct tail jump (`jmp 0xT`, no ret) skipped the `ret N` check: convention mutants
    survived 6/30. The gate now inherits T's ret N (also for CSF_FUNC bindings). Now 30/30.
  - Offset mutants survive ~20%: semcheck compares offset SETS (moving +0x104 to +0x108 passes when the original
    reads both) and ignores offsets < 8. Only the live diff catches that; known blind spot.
  - Historical fixtures rot when classes are renamed: the runner applies classes.yaml names (C_<vtable> -> name).
  - 13 src/auto files violated gate rules added after they were committed (missing globals.h, a CSF_AT redefine): fixed.
- **Vacuous smokes (found by the live controls, 0/4 caught at first):** startup had grown to ~18 s (one
  MH_EnableHook per diff/tramp hook, each suspending all threads), so 15 s lanes smokes often ended before the
  mission and passed "alive", while smoke.sh printed the PREVIOUS run's csf_diff_summary/hits files as if fresh.
  Lanes batches since startup slowed were smoke-tested only by their 60 s smokes. Fixes: MH_QueueEnableHook +
  one MH_ApplyQueued (startup 18 s -> 3-4 s); smoke.sh deletes result files before launch and waits for a readiness
  line in csf_hook.log (`devcmd: starting scene`, up to 120 s) before the N seconds start; never ready = SMOKE FAIL.
  Live controls now 4/4.
- `db/verify.json` (`tools/verify.py`): code hash = the function's own object code (bytes + relocation kinds; symbol
  names and the HOOK stub excluded, so renames keep evidence). First real numbers: of 841 done functions 217 diffed,
  64 executed, 560 never executed in the FR01 in-mission smoke.

## 2026-10-07 — Coverage starts: menu tour, savegame, scene sweep

- Gate-only functions by start (cumulative in db/verify.json): FR01 alone 560; + 3 other scenes 543 (idle missions
  run the same code); + menu tour 526; + loadsave 1.sav (mid-mission Fiord) 492; + blind autopilot movement 488.
  Movement scripts and more idle scenes aren't worth it; the rest is AI combat (T* tasks) and GUI input handling.
- devcmd `menutour MS [screens]`: GotoScreen is called once per frame from a RwCameraEndUpdate (0x628E40) trampoline
  on the game thread (GotoScreen loads the screen: never from another thread; re-entry guarded, loading renders).
  Menu manager pointer captured in the GotoScreen hook. P_Idioma, P_MeJueg and P_MeCaPa crash the ORIGINAL game
  when opened out of context (checked with `only_module core`): left out. Multiplayer lobby screens need NetZ.
- Probing the 0x864508 vtable slots in the menus: slots at 0x71A8E0/0x71A5E0/0x71A300 run per control
  (100k-300k calls / 20 s), not per frame — unusable as a frame hook.
- smoke.sh: `CSF_INI_EXTRA` adds ini lines for one run (autopilot steps, menutour, only_module).

## 2026-10-07 — auto-re-agent items finished: transcripts, waivers, evidence requests

- work/calls/<ea>/<time>-<write|review>-<backend>.json: every model call (also failed ones) with the full
  conversation and answer; system prompts by hash in work/calls/system/. Feeds the Opus review sample and A/B tests.
- db/waivers.yaml (`tools/waive.py <ea> <regex> <why>`): a gate finding waived for one function's exact compiled
  code (verify.py code hash); a code change voids it. File rules and compile errors can't be waived.
- Evidence requests: a grunt above 150 B may answer with {"actions": [...]} instead of the file (pseudo / asm /
  callers / class, <= 3 per function); the driver answers from packet.py data and the conversation continues
  without spending an attempt. First dry run (2 DeepSeek functions, 200-400 B): neither asked; both passed. The
  offer goes in the user packet, so the cached system prompt is unchanged.

## 2026-10-07 — Module map (Phase 1 item finished late)

- Phase 1 had stopped at "good enough to feed grunts"; without modules the grunt went leaf-first over the whole
  binary: 841 functions = 10 KB = 0.5% of the game bytes. Modules now exist; waves are per module.
- MSVC .obj contiguity confirmed on NetZ: its 105 __FILE__-named .cpp files (assert strings) are 109/109 contiguous
  runs, and their .cpp strings sit in .rdata in the same order. Game code has no __FILE__ strings except the 9
  generated net DDL files (PCGenerados\Net*DDL.cpp) and DataSetPerfCounters.cpp.
- Clustering (`tools/modules.py`): greedy adjacent merging fails (a function whose partner is 3 positions away can
  never merge: 6,000 singletons); dynamic programming over contiguous partitions (modularity, numpy banded prefix
  sums, 1 s) works. Signals that helped: calls, class methods, shared globals/strings, neighbouring .rdata items
  (+0.03). No signal: function padding (always 0xCC, inside and between objs). Scaling gamma with graph size was
  worse. The .CRT$XCU initializer table (196 entries, link order, stubs at 0x828xxx) anchors .data, not code.
- NetZ score 0.83 balanced; the misses are tiny files (2-3 functions) and generated near-clone files, which only
  their __FILE__ names separate (used as hard constraints where present).
- Finds while naming: a second zlib (inflate) inside the engine region at 0x6A02B0 (module core_zlib_log: link
  zlib, don't reimplement); RenderWare Physics (Rwp*.c $Id strings) is linked in, check its region; the game code
  is two blocks (0x401000-0x566000 game, 0x690B10-0x78D130 engine/core/GUI/net) with RW/D3DX9/NetZ between.

## 2026-10-07 — GUI wave step 1: class layouts, and the base-class attribution bug

- classes.py bug: a derived class's dtor with the base dtor inlined stores [derived vtable, base vtable]; it was read
  as a ctor of the base (last store), so the base absorbed every derived field and 36 base links were inverted
  (CGuiControl came out empty, its fields in the shared base C_00830438, list boxes starting at +0x1C8). Rules now:
  a multi-store function is a dtor of its first vtable when that vtable's slot 0 is or calls it, or when the slot
  count decreases (a derived vtable never has fewer slots than its base); a dtor's base comes from the base dtor it
  calls (ctors take priority: a dtor can restore a further ancestor's vtable, e.g. 0x860178 vs 0x860170); ctors
  take the nearest earlier store as base. Base links with more slots than the derived: 21 -> 6. src/auto built
  unchanged (fields keep their offsets/names when they move from base to derived).
- The .fbs builder is the Rosetta stone for GUI fields: the reader 0x71EBD0 maps each keyword to a virtual setter
  (.XY slot +0x98, .SIZE +0xA0, .ID +0x70, .ID_LEFT.. +0x78..+0x84, MOUSE +0xC0, DRAG +0xB0, QUAD_* +0x1E4..+0x1F4,
  DEBUG_RECT +0x180, FONDO +0xD8, VIDEO +0x18C); per-control readers 0x71FCB0 (buttons), 0x720420/0x7207E0
  (list boxes), 0x71F790 (panels), factories at builder slots +0x18..+0x44. Keyword strings are reached through
  pointer tables (off_8A86xx), resolved from the PE.
- CGuiControl: 5 legacy::string ids, parent, int rect, float RGBA, flags, input state bits, three
  (elapsed, interpolator*) animations (colour/pos/size, events 9-12), listener vector (VC7.1: allocator word
  first), effects list made by 0x713B20(type). Button state = 28-byte record (quad, timed, time, text RGBA) x4.
  CGuiEdit holds three VC7.1 std::wstring (capacity 7 at init) -> legacy::wstring added.
- gen_classes_h: `fields:` values may be "type name[N]" (needed for strings/arrays and for fields the access scan
  never sees, e.g. written through computed indices). IDB sync maps legacy::(w)string to C structs (12 type errors
  -> 0). Menu-tour smoke: OK, 0 diff mismatches; the rewritten CGuiControl_GetId ran 1737 times.

## 2026-10-07 — GUI wave 1 (lanes over the 15 gui_* modules); background smokes

- `grunt.py lanes --module gui_... --smoke-menu`: 417 ok, 7 escalated, 7 smokes (menu tour) all OK, no bisect.
  GUI modules: 464/853 functions, 27.8/110 KB = 25% of the bytes (gui_control 56%, gui_panel 44%). The run
  ended when the Codex quota guard paused the main lane (cap 90% of the 5h window); 347 eligible functions
  (<= 600 B) and 23 larger ones remain.
- The game pauses on focus loss: the window proc 0x508D40 handles WM_ACTIVATE(inactive)/WM_ACTIVATEAPP(0) via the
  app's vslot +0x10 and 0x50D7C0(0) (app+136). `background 1` (default in smoke.sh) subclasses the window to drop
  them, moves it off-screen and gives focus back; verified unfocused: menu tour reached its last screen, FR01 ran
  37 AI functions 30M times.

## 2026-10-07 — Codex effort A/B (Sol low / medium / high)

- 12 GUI functions (50-260 B) that Sol-high had passed first try; dry runs (`CSF_CODEX_EFFORT=low|medium`).
  First-try gate pass: low 4/12, medium 5/12, high 9/12; after the xhigh retry: 12, 11, 12. Tokens per run:
  low 145k uncached in + 17k out (20 calls), medium 121k + 16k (17), high 76k + 28k (16). Lower effort halves
  the output per call but the retries resend the whole packet, so the total is about the same and slower.
  Keep high. The bigger lever is input: ~8k prompt per call with 20-30% cached.
- The monitor's "max" was grunt's retry label; Codex gets xhigh (never a max model). Logs now show xhigh.

## 2026-10-07 — Grunt cost/speed: retries, packets, batching, direct Codex backend

Same 12 GUI functions (50-260 B) each time, dry runs; tokens for all 12 (uncached in / cached in / out):

| setup | ok | first try | in | cached | out |
|---|---|---|---|---|---|
| CLI, single, full class dump (old default) | 12 | 9 | 76k | 58k | 28k |
| CLI, single, compact class view | 12 | 12 | 76k | 16k | 25k |
| CLI, batches of 4, compact | 12 | 12 | 31k | 3k | 20k |
| direct backend, single, compact | 12 | 12 | 36k | 8k | 14k |
| direct backend, batches of 4, compact | 12* | 11 | 22k | 0 | 11k |
| DeepSeek + Codex review (>100 B) | 10 | 8 | DS 57k/175k out; Codex review 21k in, 3k out |

(*) all 4 files in every answer once batches got their own system prompt; 1 needed a retry.

- Retries were ~45% of Codex tokens; 132 of ~220 were a missing #include "globals.h" -> fixed mechanically
  before the gate (`normalize()`), no model call.
- Packets: the `this` class section (whole struct + base chain + asserts) was ~50% of GUI packet chars. Now only
  the declared fields at the offsets the function's memory operands use (stack operands excluded); `CSF_CLASS_VIEW=full`
  restores the old view. Fewer first-try failures, not more.
- Codex CLI harness floor: 4.3k tokens/call with every feature off (environment/permissions text ~180 more, now
  off). Cache hits were a coin flip (per-run session id). The ChatGPT Codex backend (same subscription/endpoint
  as the CLI, what pi's openai-codex provider uses) takes our prompt as `instructions` (no harness: 1.4k fixed)
  and caches reliably with a fixed `session_id` header + `prompt_cache_key` (3712/3893 cached on repeat;
  `prompt_cache_options` is rejected for this model). `CSF_CODEX_TRANSPORT=direct` (default) | `cli`. Token from
  ~/.codex/auth.json (~10 days; near expiry one CLI call refreshes it). pi itself not needed.
- Batching (`work_batch`, lanes `--batch-fns 4 --batch-max 300`, grouped by class then module): one call, each
  file gated alone, failures continue single from their own part of the answer. Through the direct backend the
  single-file output rule in the system prompt beat the batch note (only the first function answered) -> batches
  use SYSTEM_BATCH. Lanes validation (CLI): 28 batched functions, 28 ok, 3.4k in/fn vs 10.6k single.
- Effort: low/medium Sol fail first try more (4-5/12 vs 9/12); retries eat the savings. Keep high.
- DeepSeek for mid-size functions: 10/12, the reviewer caught one bad file, the gate the other. Costs Codex only
  a review (~2k in, 0.25k out). Option for when the Codex quota is the bottleneck; not the default yet.
- A 429 usage_limit now pauses Codex (functions deferred, not escalated). These tests used up the 5h window.

### 2026-10-08 — Batch size A/B, retry causes

Same 24 todo GUI functions (4 classes, 48-300 B), direct backend, high effort, dry runs:

| variant | ok | first try | Codex in (uncached) | Codex out | 5h quota | wall |
|---|---|---|---|---|---|---|
| A: batches of 4 (current) | 23* | 22 | 55k | 30k | 5% | 228 s |
| B: ≤8 fns / ≤800 B per call | 24 | 21 | 46k | 33k | 6% | 390 s |
| C: DeepSeek <100 B (+review), rest as B | 21 | 18 | 81k (+DS 27k) | 40k (+DS 67k) | 7% | 425 s |

(*) the miss was a dropped stream (transport), now retried in place.

- Output (reasoning, ~1.2k/fn) drives the quota and doesn't shrink with batch size; bigger batches only trim the
  already-small input and make calls slower. Keep 4. `--batch-bytes` (run + lanes) stays available, default off.
- DeepSeek for 48-100 B loses: its failures fall back to single Codex calls plus reviews, costing more Codex than
  doing them in a batch. Keep DeepSeek at <48 B + clones.
- No cache hits when all calls start at once (A); input is cheap, not worth staggering.
- Retry causes (216 rejections in transcripts): 60% missing globals.h (auto-fixed since 4c31145). Of the rest:
  indirect tail jumps written as calls (~29 incl. inline-asm follow-ups) -> packet now warns up front;
  `__ftol2` "dropped call" (MSVC float->int helper; models faked references + hand x87 code) -> semcheck exempts
  MSVC code-gen helpers (__ftol2, __allmul/div/rem/shl/shr, __alloca_probe, _chkstk), packet says write the cast;
  EH-frame handler stubs declared as CSF_FUNC -> packet notes the fs:[0] frame; gate's new/delete check fired on
  comments ("the new block") -> comments/strings ignored; Hex-Rays `_DWORD`/`_BYTE` -> auto-fixed to <cstdint>.
  20 accepted files re-gated after these changes: 20/20 pass.
- Transport: dropped stream / 5xx / timeout retried up to 2x (5 s, 10 s) instead of escalating.
- Monitor shows the run's settings (transport, effort, batching, DeepSeek tier, review, quota cap), batch size
  of in-flight calls, Codex tokens per function, cache %, first-try rate, batched share, auto-fixes, deferrals,
  transport retries; `codex exec` count only when the CLI transport is used.

### 2026-10-08 — Captain (naming worker), globals

`tools/captain.py`: one-instance naming/RE worker beside grunt, Claude via `claude -p` (`--tools "" --setting-sources ""
--strict-mcp-config --disable-slash-commands --system-prompt ... --json-schema`): ~560 fixed tokens, cached across
calls (`--bare` needs an API key). Monitor has a Captain panel (work/captain_state.json).

- Packet per global = mechanical facts (initial data with pointer targets: string text / function / vtable;
  neighbouring globals' values; extent; writers) + *distinct* usage lines with counts (0x8AFC90: 812 accessors in
  ~1.6k chars), IDA's bogus library types (Concurrency::...) stripped, short consumer functions inlined.
- Sonnet vs Opus on the same 12 (no apply): Opus right where they differed (0x8B68C0 = RwEngineInstance, zero
  vector, MP network type), typed them, held 5 vs 9. ~3x cost ($0.07 vs $0.02 per 6-global call) and time (20 s
  vs 6.5 s); absolute cost trivial -> Opus for globals. Re-test Sonnet for bulk function renames.
- Run 1 (87 cross-module globals, 11 calls, $0.66): 38 applied, 44 held. Held = packet gaps: string-pointer
  globals showed only the pointer (model guessed "MissionBriefing" for "Menus/P_Help.fbs"), char* types refused,
  one 26-global over-merged cluster. Fixed (pointer targets shown, string types, clusters cut at --batch); retry
  of the held: 29 more applied ($0.46), 19 held.
- Names applied in a run are now visible to its later calls (families came out in four styles before).
- Mechanical `captain.py strings`: never-written globals pointing to a string are named after it, no model:
  921 (config/asset keys `.TOP` -> g_pszKey_TOP, Menus/P_Help.fbs -> g_pszMenu_P_Help, PFX names; duplicates
  qualified by module). globals.yaml: 1136 entries, 962 pinned by the captain.
- globals.py: takes work/db.lock (captain writes concurrently), keeps src captain pinned, drops .text addresses
  (18 EH-stub "globals").

### 2026-10-08 — Captain: module names; module line in packets

- `captain.py modules`: per-module packet built live (named classes, strings incl. captain's string-pointer keys,
  real function names, library calls, caller/callee modules by name, address neighbours); existing 148 names in the
  (cached) system prompt as style reference; modules ordered evidence-richest first so their names appear in later
  packets. 121 unnamed -> 16 Opus calls, $0.76: 17 applied at >= 0.7, 22 more held at >= 0.6 applied after review
  (RW-API based gfx_* names, MP/HUD). 187/269 named.
- The other ~60 are 0.2-0.5 guesses from call-graph position (unnamed classes, placeholder function names, no
  strings; e.g. m45AAF0 47 KB). Left unnamed: name them just before their wave, once classes/fields are recovered.
- Grunt packets now carry `Module: <name> (<note>)`; non-class functions are prefixed by the module subsystem.

### 2026-10-08 — Captain: classes / placeholder functions don't fit batch naming (yet)

- `captain.py classes` (classinfo sheet + creation-site code + ctor body + method names + module): 12 classes,
  Sonnet vs Opus twice. Both agree on families (CGuiFont, CGuiEffect*, CGfxClumpObject) but at 0.35-0.65;
  1-2/12 clear 0.7. Opus ~2.5x cost, no better. Class identity needs behaviour across many functions ->
  agentic per-family sessions at wave prep (as done for GUI). Nothing applied; proposals kept as hints.
- `captain.py functions` (290 placeholder names: GetField0050, IsField90Null, ReturnMinusOne...; packet = body +
  per-caller call line and the two after it): 1/30 applied for either model (only string-attested
  GetTSubirName -> TSubir_GetName). Tiny accessors' meaning lives in callers that are unnamed themselves.
  Re-run after waves name the callers (Sonnet: ~$0.002/function). Nothing applied.
- What batch naming is good for: self-contained evidence (strings/keys, data tables, RW API, module evidence):
  globals and modules worked; classes/accessors don't.
- classinfo.py read the old db/class_names.yaml (names never shown); now db/types/classes.yaml.

## 2026-10-08 — captain family: agentic class naming per module group
`captain.py family <module globs>`: one headless Opus session per ≤6 unnamed classes, cwd = repo, read-only tools
(Read/Grep/Glob + Bash limited to classinfo.py / evidence.py, `--permission-mode dontAsk`, `--max-budget-usd 3`).
The batch class packet stays the starting sheet; the session follows creation sites itself. Menu modules: 16
unnamed classes, 3 sessions, 27-49 turns, 90-150 s, **$1.84 total; 12 applied (conf ≥ 0.7), 4 held** — vs 1-2 of 12
for the one-shot batch job. The deciding evidence was what batch packets lack: the CMenuManager registry
(0x511450 allocates every screen into a manager slot, 0x511810 registers it under its g_pszMenu_P_* panel name),
which separates e.g. CMenuLoadGameInGame / CMenuLoadGameMainMenu. Names: CMenuManager, CMenuScreenFader,
CMenu{LoadGame,SoundOptions,Controls,VideoOptions}InGame, CMenuGameOptionsInGamePC, CMenuProfilesPC,
CMenuMpSessionsPC, CMenuDemoExit, CGuiCharacterPreview. Held: CMenuIntro 0.65, CMenuMissionInfoInGame 0.6, two
DO-filter classes (0x8453a4/0x845368) < 0.5. Use it at every wave prep (module wave = its classes first).

## 2026-10-08 — Smokes are muted
csf_hook.ini `mute 1` (written by tools/smoke.sh unless CSF_MUTE=0) mutes the game's WASAPI audio session on the
default output device (src/core/devcmd.cpp, from CDevCommandLine::Set, on a separate MTA thread). That catches
whatever API the game plays through and leaves the saved sound options alone. Windows remembers per-app session
state, so a missing `mute` (or `mute 0`) explicitly unmutes. Tested on a hardlinked private play copy
(CSF_PLAY=/mnt/e/dev/csf-play-mute) because a grunt lane was using the shared one: SMOKE OK, SetMute hr=0.

## 2026-10-08 — menu wave 1: link failure, crash, background smokes no longer take focus
- Lanes stopped on a *link* error (gate compiles each file alone): 0x564B80's naked asm called a `__fastcall`
  helper by its unmangled name. Lanes now maps build/link errors to the batch's files (`build_culprits`: compile
  errors by path, `undefined symbol` by the file that uses it), quarantines them and carries on.
- The remaining batch crashed the menu tour (EIP in .rdata 0x865B44); bisect (11 smokes): 0x540870 (menu_mp_stats).
  58 committed; 45 in-flight files the stopped run never logged were dropped (regenerated next run).
- Smokes stole focus and the mouse (an active game window acquires the DirectInput mouse exclusively). Background
  mode now redirects the game's ShowWindow / SetFocus / SetForegroundWindow imports to no-activate versions for its
  window, adds WS_EX_NOACTIVATE and answers WM_MOUSEACTIVATE with MA_NOACTIVATE. Smoke OK with menu tour.
- Module waves now include the captain steps (`grunt.py lanes --module ...`, `--no-captain` to skip): before
  grunting, `captain.py family <modules>` names unnamed classes, then the rename pass + build + commit (reverted if
  the build fails); after a run that committed functions, `captain.py functions --module <ids>` renames the
  wave's placeholder functions. Held classes are not re-asked on later runs (`family --retry-held` does).

## 2026-10-08 — Model A/B over OpenCode Go; grunt lanes become one tiered lane
- OpenCode Go ($10/mo, per-model monthly $ allowances; 5h window = 20 %, week = 50 %) via a service-account key
  (~/.config/csf-re/opencode_go.key). grunt backend `go`: OpenAI chat/completions or Anthropic /messages (Claude
  models: adaptive thinking + output_config.effort), `model@effort` pins effort, `x-opencode-session` header
  required, DeepSeek needs the workspace's Privacy = Global. Streamed with a 240 s idle timeout: non-streamed, a long
  MiMo think and a hung request looked the same for 20 min.
- `grunt.py ab` (work/ab/, nothing committed). Track a, 25 new functions under 150 B: deepseek-flash 25/25 $0.0050/fn
  16 s; **Haiku 5.5 low 24/25 (96 % 1st try) $0.0014/fn 8 s**; GLM-5.3-Flash 25/25 but 80 % 1st try, 40 s. All three
  8/8 on 48-150 B: the cheap tier can take far more than the old 48 B cut. Track b, 12 functions Codex had won
  (6-319 B): **Haiku 5.5 high 12/12 100 % 1st $0.0053 20 s**; MiMo 2.6 Pro 12/12 75 % $0.0111 130 s (long thinks on
  the big ones); Haiku xhigh no better than high at 2x cost; Kimi K2.7 / DeepSeek V4 Pro / MiniMax / Qwen dropped.
- Caveats: the gate is a weak oracle for big functions; code similarity to Codex's file (0.79 for all three) stops
  meaning anything above ~250 B (Haiku high vs Haiku xhigh: 0.52); set b is survivors of Codex, with richer packets
  than Codex had. Codex still reaches answers with ~1.8k output tokens vs 7-20k: the real gap should show on big
  functions. So the decision is validated by a real wave (smoke + in-game diff), not by the A/B.
- Lanes are now one lane with size tiers (see PLAN Phase 4); per-tier stats and Go allowances in monitor.py.

## 2026-10-08 — First tiered batches (menu wave): 136 committed, one copied bug, MiMo dropped
- 140 results: S 120/120 gate (Haiku low 114, 88 % first try), M 16/17 (Haiku high 15; Codex review rejected 5 of
  20 Haiku-high answers, each then fixed), L 3/3 Codex. Go spend for the lot: $0.55.
- Link failure 0x53CC70 (asm naming its own __fastcall helper, again): gate now rejects undefined self-references
  (llvm-nm); lanes also searches in-flight files for batch-build culprits (one landed after the batch was cut).
- Menu-tour crash, bisect 33 smokes: 3 Haiku-low clones of one shape — the first one's file became the siblings'
  example, so its bug was copied: a generic VCall helper indexed the vtable with the byte offset (slot 0x30 -> 0xC0).
  /Od /Ob0 build: the helper hid the offset from semcheck. New rule: every vtable slot the original calls must
  appear as a constant displacement; if not, rechecked on an /O2 compile (a correct helper folds back). Over all
  2040 files: 2 more latent copies (quarantined), 5 Codex runtime-slot files waived after checking every N / 4.
- MiMo 2.6 Pro: 3 rescues in 140, 6-12+ min per call. Ladders now S: Haiku low x2 -> Haiku medium -> Codex high;
  M: Haiku high x2 -> Codex high x2; L: Codex high x2; review at medium. No xhigh (not worth its quota).

## 2026-10-08 — OpenCode Go dropped; grunts get their own Claude account
- Go's Haiku allowance ran out after ~280 functions (5h window; $10/mo plan, per-model allowances): the last batch
  ended 121 ok / 2 escalated / 18 deferred, and before the stop Codex had quietly absorbed the S/M work.
- Grunts now call Haiku 5.5 (and Opus 5.5) through `claude -p` on a second subscription (CLAUDE_CONFIG_DIR =
  ~/.config/csf-re/claude-grunts); the captain stays on the main account, and the grunt backend refuses to run if
  the grunts' login is missing (never falls back onto the main account). CLI overhead per call ~560 tokens
  (cached) with --strict-mcp-config --setting-sources "" --disable-slash-commands, like the captain; without
  them ~4k (the account's claude.ai connectors alone ~3.4k). --bare would be leaner but takes API keys only.
- Quota: lanes polls the grunts' account usage (same oauth/usage endpoint as the monitor, shared cache file) and
  pauses it at 95 % of 5h/7d. Once Codex's 5h window passes 50 %, S/M Codex rungs run on Opus 5.5 medium, so
  Codex high stays for L. The Go backend, spend ledger and the A/B tooling are removed (results stay in work/ab/).

## 2026-10-08 — Haiku thinking via claude -p
- Same request settings as over Go (adaptive thinking, effort low), but ~2x the output via claude -p at equal size
  (40-150 B: median 5.0-5.4k vs 2.5-3.0k tokens, 23 vs 12 s); ~90 % of it is thinking (modelUsage.thinkingTokens).
- --effort does reach Haiku (one packet: low 6k, high 12k, max 60k thinking); MAX_THINKING_TOKENS and
  alwaysThinkingEnabled=false change nothing (adaptive thinking ignores them).
- A one-line "keep thinking short" system note at effort low: median packet 7.1k -> 4.0k tokens (30 -> 17 s), a
  hard one 24k -> 17-20k; all 18 test answers passed the gate. On for claude calls at effort low (CLAUDE_BRIEF).

## 2026-10-08 — Workflow review: the loop reorganized (cutover contract, produce/verify, types first)
Fresh-eyes review of the RE flow (user request), then implemented end to end. Findings and what changed:
- **The output only worked hooked.** Callers bound callees by per-file invented names (1,090 addresses, 276 with
  more than one name, 39 with conflicting conv/argc), implementations were `static`, constructors stored original
  .rdata vtable addresses, and the planned link test never existed: the standalone build could not have linked.
  Now the standalone link is keyed by address (src/globals.h: csf::fn_slot<A> defined by HOOK, read by CSF_FUNC;
  csf::gv / csf::vt for data). linktest.py compiles all 2337 files standalone: unresolved game 399, rw 33,
  netz 34, crt 13; 346 globals + 81 vtables to generate; 14 asm thunks to rewrite. addrfix.py moved 238 raw
  addresses (168 files) into CSF_VTBL / CSF_ADDR_*; api.py gives one name + signature per address (832 files
  renamed, 276 -> 8 multi-name addresses; 411 grunt callee names harvested as votes) and an ABI check against the
  binary (callee reads ecx before writing it => must be bound __fastcall): 6 real bugs, all of one shape — a
  tail-jump forwarder or caller dropped `this` (4 NetZ forwarders; std::_Tree::_Erase 0x739B80 + 2 callers).
  Harmless while hooked, a crash whenever bisect turns the hook off.
- **Smokes gated production but covered one start.** Lanes stalled during smokes and bisects (11-33 smokes), and
  `--smoke-menu` waves only ran the menu tour: master's tip crashed at the FR01 scene start. Production and
  verification are now separate: lanes commit gated functions as *pending* (ledger `ok` = installed in a passing
  smoke, from csf_hook.log); every deploy disables pending hooks; verifier.py builds HEAD in its own detached tree,
  smokes the verified baseline first, then module groups, quarantines culprits. First run: 8 menu-wave functions
  broke the baseline -> queue/, found in 8 smokes.
- **Crash triage had never worked**: crash_suspects' regex wanted a lower-case DLL base (printed with %p) and LF
  line ends (the log is CRLF), so every bisect was a blind binary search. Fixed: one smoke per culprit.
- **Types**: 4,231 field_XXXX / 6,313 CSF_AT / 2,299 raw vtable casts. Grunt field-name proposals agree once
  spelling is normalized (CGuiPanel +0x138 childrenBegin 29/31): fields.py pools them (17 names at >= 3 votes and
  60%, merged under the curated classes.yaml names); gen_classes_h emits 942 typed virtual-call wrappers
  (`obj->vName()`), no layout change. Field coverage gauge: 43.9% of self accesses are named.
- **Fidelity drifted to bit-exact**: 0x53C6E0 emulated x87 80-bit division in 100 lines. Prompt rule: behavioral
  equivalence, natural float expressions; the in-game diff tolerates last-bit differences where the original
  stores floats (difftable records the offsets). 4 more emulating files in the backlog.
- Diff tier 2: callees that are pure (no writes outside their frame, transitively) may run twice: 727 -> 750
  diffable. review.py: Opus samples 5% per module against the packet (6 GUI + 2 hud reviewed, 0 rejected).
- `grunt.py wave <module>` = prep -> lanes -> verifier -> review -> link test; `wave --next` ranks modules by
  centrality; hubs (>= 25 outside callers) go to the strong ladder; S ladder lost its Haiku-medium rung
  (9 rescues of 330); `grunt.py cost` shows p50/p95 latency and calls per verified function (1.10).
- First wave on the new loop: hud, 12 functions, all Haiku-low first try, verified in game (20 s per group).
- PLAN.md: phases replaced by the loop, current focus, backlog by area, foundations; status.py/monitor read it.

## 2026-10-08 — Groundwork: IDA used fully (signatures, classes, typed pseudocode)

- **What retries cost.** Gate failure kinds are now logged per attempt (`fails`; `grunt.py cost`, older runs
  backfilled from transcripts): of 476 past rejections, 26% were the missing globals.h include (normalize fixes it
  since the overhaul), 23% a dropped vtable store (packets now list every vtable reference with the line to
  write), 14% disallowed inline asm, 14% compile errors (mostly type names that don't exist).
- **We used a fraction of IDA.** Hex-Rays ran once (Oct 7) as text; none of our names/classes/types went back.
  Now: `ida/export_abi.py` reads Hex-Rays' prototype objects, the argument count at each call site, the caller
  cleanup after each call, and what each call's `this` is made of. Precedence: exact binary facts (`ret N`,
  callers' `add esp`, an entry-block register read) > Hex-Rays data flow > heuristics. Findings on the way:
  - Hex-Rays builds a prototype from the body alone (drops unused cdecl args) and **trusts callee prototypes when
    decompiling callers**: 0x42E3B0 with 3 args instead of 5 made it drop the `this` 0x431CF0 passes on (stdcall).
    `sync.py --protos` applies the 482 cleanup-proven prototypes; re-decompiled, 0x431CF0 became the thiscall it is.
  - A jmp thunk gets its target's prototype from Hex-Rays (`mov ecx,[ecx+30h]; jmp` lost its `this`), and IDA may
    type a thunk from a library signature (NetZ 0x7A3190 `thiscall` -> `xor al,al; ret 4`): thunks follow targets.
  - `push ecx` reserves a slot only in the prologue; after it, it passes `this` on (0x431CF0, 0x544390). The gate's
    ABI check had it wrong; with the fix it found 5 bindings that dropped `this` (4 files, fixed; one passed a
    global helper object in ecx by luck: 0x73C6C0 -> 0x68F560).
- **Signatures for every function**: api.json 2750 -> 21746 addresses (sig_src abi, name_src addr for unnamed:
  never forced onto a file's own name); callee calls with a bind line 40% -> 98%. Against the 2170 gate-passed
  implementations: 94% the same ABI; every sampled disagreement went to the binary-backed side.
- **Classes through Hex-Rays** (`ida/propagate.sh`, ~6 min/round, no tokens): seeds (vtable slots, ctors, dtors)
  typed in the IDB, Hex-Rays types what callers pass, `this_class.py` votes (nearest common ancestor); backwards,
  a function passing its own `this` to a classed method is one of its methods, parameters and 4-byte members passed
  as `this` point to the class (arg_types.json). `plain_classes.py`: classes without vtable from methods calling
  each other on the same `this` (STL instances excluded: they glued unrelated classes): 422 classes, 1449 methods.
  Converged: 9798 methods with a class; **undone methods with a class 42% -> 71%**; 1198 parameters and 227 members
  typed as class pointers.
- **Typed IDB**: 691 `<Class>_vtbl` structs (slots named like the C++ wrappers, signatures from api.json) +
  `__vftable`, member pointer types, 1215 global types. Pseudocode on a 3000-function sample: `->member` 0 -> 8630,
  raw `*(_DWORD *)(x + N)` -19%, `sub_` names -30%, `this->vIsShown(this)` instead of vtable arithmetic.
  gen_classes_h: 942 -> 2703 virtual-call wrappers. Wave prep now syncs the IDB and re-decompiles the module.
- **A/B** (40 functions whose packet changed — 61% of a 600-function sample —, Haiku low x2): old context, after
  the signature/vtable stage, after all rounds. Pass rate flat (38/38 each; first try 34/35/32 = noise at n=40),
  tokens flat (14.2k/13.6k/14.7k), notes admitting a guess 33 -> 24 -> 22, **named members in the output
  87 -> 91 -> 124**; in classed methods raw offsets 106 -> 91 and named 95 -> 126. The stage-1 arm showed the raw
  offsets sit on untyped objects; typing parameters, members and plain classes is what moved the output.

## 2026-10-09 — Behaviour checks in the gate; the play copy loads the hook again; last context rounds

- **Gate beyond references** (`semcheck.behaviour`, from `export_abi.py` `consts` / `order`): the original's
  numbers (literals, case values, float literals and .rdata floats, not pointer offsets) must appear in ours, and
  calls Hex-Rays shows as consecutive statements of one block keep their order (each callee called once in both).
  Equivalent forms accepted (x*8 / x<<3, x/255 / x*(1/255), case 203 vs `x - 200` case 3, a float global read by
  address, 0x24C bytes vs 0x93 dwords, a sign-bit mask vs `< 0`). Survey over the 2358 verified files: 2 flagged,
  both equivalent rewrites. Mutation tests: a changed constant and swapped calls are caught (small constants like
  3 -> 10 slip: too many other matches).
- **Compile fix-its**: `reinterpret_cast<T*>(nullptr)` (21 retries), clang's did-you-mean for unknown type names
  (old class names) applied and re-gated without a model call; hint for C array assignment.
- **In-game diff was already there** (hook/diff.cpp, verifier quarantines mismatches): widened with copies of
  `this` (`mov esi, ecx`) and contained callees: 750 -> 788. The rest is blocked by virtual calls (862) and callees
  with side effects (513): that is diff tier 3 (record/replay), still in the backlog.
- **Smokes were fullscreen and audible**: processes started from WSL inherit the "prefer System32 images"
  mitigation (`PreferSystem32Images=1`, also through powershell Start-Process), so the game took System32's
  dinput8.dll and never loaded our proxy (no hooks, no windowed/off-screen/muted mode). `tools/playexe.py` renames
  the play exe's import to CSFHOOK.dll; `smoke.sh` kills a game whose hook log doesn't appear within 10 s.
- **Prototype line**: export_pseudo writes comment lines above the prototype; difftable read conventions/returns
  from the comment (every diff entry cdecl/int: crash) and packets showed it as "Hex-Rays prototype". `proto_of`.
  FR01 60 s: 283 functions diffed, menu tour 150, 0 mismatches.
- **Last context rounds**: globals, return values and stack objects passed as `this` now typed (39 / 23 / 231).
  First attempt typed 112 return values from single callers: the allocator 0x6910F0 got a class from one ctor call
  and Hex-Rays spread it (190 functions worse). With >= 2 agreeing callers and half the call sites: 113 functions
  better, 10 slightly worse; members +0.4%. The propagate loop also stopped early (it counted only new methods).
  Named Win32 constants: 62 call sites in all game code, and grunts can't include windows.h: dropped.
  Conclusion: context from IDA is near its limit; checks (gate, diff tier 3) are where quality still grows.

## 2026-10-09 — script_commands; Haiku spot check; wave stop rule

- **m45AAF0 = script_commands**: the mission-script command set. sub_465310 switches on an int16 id (360 ids,
  1000-5999, banded) and calls one stdcall (ctx, cmd, args) handler each (328 handlers); arguments are evaluated
  through sub_467BC0, which recurses into the dispatcher (id -1 = literal/variable). m467AD0 (parser of .SCRIPTS
  .VARIABLES .ACCIONES .CONDICIONES .EVENTOS .TRIGGER + evaluator) renamed object_class_defs -> mission_script.
- **Haiku spot check** (4 S Haiku low, 3 M Haiku high, against pseudocode + disassembly): all 7 correct. Twice more
  faithful than Hex-Rays: a NaN branch (`test ah,5; jp` sends NaN down the >= 0 path; Hex-Rays says `>= 0.0`) and
  a callee's hidden ecx argument. Weak spots: cast-heavy, inverted names, one wrong comment. All grunt runs so far:
  S 99% accepted (83% first try), M 95% (44%), L 92% (Codex only, n=38).
- **Stop rule** (`wave_report`): after verify + review a wave counts quarantines since it started (canary groups
  included), Opus rejects and the module's done functions still unverified; over threshold -> exit 3, and
  `wave --waves` stops. A smoke failure the verifier can't attribute isn't a defect but leaves functions pending,
  so it trips the pending threshold. Between waves `verifier.py --baseline-only --full` smokes the verified
  baseline with menu tour + savegame (FR01's short start never reaches most code).
- The script_commands canary group: 0045bba0 named by a crash report and quarantined, then the re-smoke and a
  control run with the whole batch off also crashed, though the baseline had passed 30 s earlier: unexplained.
- Waves only take functions up to 600 B: 646 functions over that (~700 KB) are untouched (backlog: L pass).
- **Tier X**: instead of a separate L pass, waves now take every size; over `--l-max` 600 B the captain's model
  writes (Opus high x2, Codex xhigh x1, `--x-jobs 2`), on the grunts' account like every grunt rung. Codex isn't
  the bottleneck (11% of its 5h window after a 30 min run). Claude call timeout 900 -> 1800 s for big functions.
- **Tier X works**: Opus wrote 004640d0 (676 B) and the 8 KB / 343-case dispatcher 00465310 first try (gate ok).
  The dispatcher was later quarantined with 00463fa0 (same crash, read at address 1 inside our DLL): in queue/.
- **Double dereference** (`*CSF_AT(T*, p, off)`: CSF_AT already IS the value): 38 files, 33 in script_commands,
  spread through clone examples; 18 of them were "verified" (never executed, or read valid memory by luck). The
  gate now rejects the form; all 38 back to todo, script_commands' 33 redone clean. Offset/reference/constant
  checks can't see a dereference too many: only the game (or a diff) does.
- **Verifier quarantine leak**: a quarantined function becomes todo, so write_pending_ini stopped disabling it,
  but the deployed DLL still held it: every re-smoke crashed on it, control runs included, and the group was
  declared "not attributable". This morning's canary and the redo wave's group both hit this. Now quarantined
  functions get their own `# quarantined` disable lines until the next deploy.
- **Bisect**: a crash at an EIP in no module names no suspects (stale frames on a smashed stack).
- script_commands result: 348 done, 0 pending; quarantined 0045bba0 00461a80 00462580 00465310 00463fa0; Opus
  review 3/37 rejected (8%: a missed __ftol2 truncation, a wrong `this` to a callee, an int return tested as a
  byte). The stop rule fired on the redo wave (2/2 quarantined, nothing verified yet in that wave): correct call.
- **X tier ladder flipped**: Opus-first took the grunts' 5h window from ~21% to 78% (then HTTP 429) on the
  game_data_defs wave, nearly all X-tier data loaders. Now Codex high x2, Opus high x1 only as escalation. The 3-wave
  loop was stopped after that wave (its Opus review step skipped too: review.py runs on the same account).
- **hud wave + loop stop**: hud 127 written / 124 verified / 3 quarantined, Opus review 0/7 rejected. The first full
  regression smoke between waves (FR01 + menu tour + savegame, verified baseline) failed: blind bisect (no crash
  names) quarantined 7 (0078ce30 00544920 004350c0 0042df60 0042db20 0078ce20 00544980), then "full batch passed
  on re-run: flaky". With a flaky crash, blind-bisect culprits may be false: re-check them before trusting.
  The loop stopped as designed (`waves: regression check failed`).
- **Diff tier 3 v1** on branch `diff-tier3` (worktree work/t3, play copy /mnt/e/dev/csf-play-dev, not merged):
  difftable.py picks 420 functions whose own writes stay in this/globals/stack but which make direct calls; 497
  callees intercepted (251 over existing hooks, 246 MinHook). hook/replay.cpp records the original's calls (callee,
  ecx/edx, args, return incl. st0, bytes changed in the snapshot regions and in stack buffers passed by pointer)
  and answers ours from the record. First smoke (FR01 60 s): crash after 2 s in original code 0x739BC0 (not a
  tier-3 function: harness side effect). Mismatches seen are mostly harness artifacts: edx compared for callees
  whose Hex-Rays `edx` param is spurious (0x429B60), stack-address returns not treated as equal. Next: compare edx
  only for real fastcall callees (callers' cleanup + edx actually set before the call), stack-pointer returns
  equal, `diff_t3 0` A/B to confirm the crash is the harness, then bisect tier-3 entries with diff_limit.

## 2026-10-09 — M ladder: Codex first
- grunt_log (198 M results): Haiku 5.5 high wrote 3 of the 189 passes in 267 calls (~20k output tokens each, 5.3M
  total; 0/102 on 10-08, 3/165 on 10-09), Codex high 186. The two Haiku rungs only delayed every M function and
  spent the grunts' Claude window. `--m-ladder` default now `codex@high:2, opus@high:1` (as X). S stays Haiku low
  (688/735 passes, 82% first try). 75% of the remaining game bytes are M/L/X, so Codex quota sets throughput now.

## 2026-10-09 — Smoke pool: parallel instances, fail-fast, capped k-way bisect
- Several game instances at once: WinMain 0x509250 exits when the mutex "CommXPC Pyro Studios 2005" is held; the hook
  DLL gives it a per-process name (IAT 0x82D19C, src/core/devcmd.cpp). Play copies are cheap (tools/playcopy.py:
  97 big files hard-linked, 13 s each): csf-play + p1..p7. 8 instances on this box (i5-12400F, 32 GB, RTX 2060):
  ready in 5-9 s (3-4 s alone), a round of 8 x 30 s took 27 s wall. A fullscreen game of the user's blocks the
  others' D3D9 device (they exit during video setup); the user's own copy csf-play-user is the unhooked original.
- Interrupting a pool run left 3 orphaned games and ~2100 bisect `disable` lines in every ini, csf-play included
  (killing smoke.sh never stops the game): pool.run now stops its instances on any exit, SIGTERM runs the cleanup.
- smoke.sh fails on the first CRASH line (the game catches some faults and keeps running; those smokes used to run
  out their full time) and reports stop= / ran= / fail_at=. The adaptive "no new hooks for 6 s" end reached the same
  940 executed / 279 diffed functions in 10 s instead of 30 s, but real crashes came at 17 s and 44 s: opt-in only.
  Default smoke 15 -> 30 s.
- Bisect: runs capped at 2 x fail_at + 3 s; control and re-run side by side; k-way search on the pool. The verified
  baseline crashed deterministically 17 s into FR01 (EIP in no module, ebp = a code address: no crash report);
  8-way blind bisect over 2830 functions: 0x45B570 in 33 smokes / 215 s. Its ledger said ok=54 hits=0: installed in
  54 passing 15 s smokes, never executed. Cause: the original pushes the angle for 0x4F5170 (forwarder to
  provider->vtbl[8], ret 4) before calling getter 0x4FBDE0 (plain ret); Hex-Rays attached it to the getter, the
  grunt followed, the virtual's ret 4 popped our frame. Fixed by hand; 4 x 60 s FR01 clean. The gate passed both.
- The user's saves 2..24 load. In the first round: 9.sav crashes 44 s after load; 0x42D3C0 HudQueryWidgetValue
  mismatches 200/200 in every start although verified (baseline() ignores MISMATCH). Both in the backlog.

## 2026-10-09 — 9.sav crash and the 0x42D3C0 mismatch
- **9.sav**: reproduced 3/3 on the verified baseline (crash 36-66 s after load), 2/2 clean with `disable_module auto`
  (control: core dev-start hooks stay). Two of the three crashes had EIP in our DLL at the same offset: linker map ->
  0x461750 BuildLookupFromThreeStrings (script_commands; ledger hits=0, never executed before). Its stack object
  is three 0x4C-byte entries (0xE4 B = the whole frame), all initialized by 0x45BD10; Hex-Rays split it into
  `v19[38]` + `v20`, the grunt sized the buffer at 39 dwords, the init ran 0x48 B past it. Fixed (57 dwords): 4/4 x
  120 s clean. The gate can't see buffer sizes (backlog).
- **0x42D3C0 mismatch**: callee 0x70DDF0 is `fld [0x82D4CC]; ret` (the COMDAT-folded `return 0.0f` in ~100 vtable
  slots); Hex-Rays typed its call `int` inside 0x42D3C0 and 0x42D3C0 itself `int`, so ours returned eax and the
  diff compared eax (both garbage). Now `api.returns_fpu` (Hex-Rays float/double for the callee itself, or a ret path
  ending in an x87 result / a call or tail jump to such a function) feeds the gate: a binding or own definition
  returning non-float for an st0 function fails. Sweep of all sources: one more, 0x6FD180 (void, but returns
  RpLightGetConeAngle's st0). difftable.py compares st0 for both (only these 2 entries changed). 9.sav: 0x42D3C0
  200 compared / 0 mismatched.
- `verifier.baseline()` now quarantines MISMATCH lines of verified functions (it ignored them, so 0x42D3C0 stayed
  verified while mismatching in every start).
- The fixed functions are pending again (code hash changed); these were investigation runs (`CSF_NO_LEDGER=1`), the
  verifier verifies them as usual.

## 2026-10-09 — Diff tier 3 v2: smokes clean (branch diff-tier3-v2)
- v1 (diff-tier3) merged with master, then fixed until FR01 60 s and `loadsave 9.sav` 60 s run clean with tier 3 on
  (378 / 373 functions diffed, ~90 of them tier 3). The harness bugs, in the order they crashed:
  - Divergence ran ours' remaining callees for real (their side effects twice). Now the replayer abandons ours
    (`__builtin_longjmp` back into run(), SEH head and x87 stack put back), the original's snapshot is restored
    and its result returned. Same for any trace mismatch or inconclusive run: the callees' effects are the
    original's, so the original's results stay.
  - Stack-buffer deltas were "bytes that changed": 0x739FA0 stores a heap pointer into an uninitialized local whose
    stale high bytes already matched, ours got half a pointer (0x73A060 returned 0x2820, crash in 0x7354D2). Now
    the dword-widened span first..last changed byte is replayed, and a window ends at the next watched pointer
    (bytes went to the wrong buffer in ours' frame).
  - Callee pop from the table: 0x48E4D0 is thiscall with no stack args but tail-jumps to a virtual that pops the
    caller's one (`ret 4`); the replay popped 0 and ours returned into garbage. The real pop is now measured per
    recorded call (esp after the return vs at entry) and replayed; it also widens the compared args.
  - A pointer return equal to an arg / ecx replays ours' own arg (vector ops return `out`); an unmatched stack
    address is inconclusive (abandoned).
  - difftable.py: CRT callees (FLIRT lib: __ftol2, memcpy, _chkstk) aren't intercepted (part of F); ecx/edx are
    compared only when the callee reads them before writing at entry (0 edx callees left, 6 before); pop falls back
    to the convention when a callee has no `ret`; st0 returns from api.returns_fpu. The diff lock is only tried while a
    tier-3 diff runs elsewhere (its callees may wait on the loader thread).
- Heap-read artifact (3 functions): the original's callee changes a heap object ours then reads (0x6BF280 tests
  frame->parent after RwFrameRemoveChild cleared it; 0x726C10 tex+0x54, 0x45D4D0 entry+0x18). Undoing heap
  writes is unsafe (freeing callees), so 256 bytes behind every heap pointer arg / ecx are watched; a mismatch after
  such a change logs `T3 ... inconclusive (a callee changed heap memory ...; <the mismatch>)`, not a DIFF.
- First real find: 0x42DF80 passed 0 instead of a1 to 0x50D7F0 (the 0x45B570 pattern again: arg pushed before a
  no-arg getter, the tail-jumped 0x536AB0 pops it). Fixed. Gate and tier 1 had passed it.
- Noise left: 0x544310 / 0x727250 return uninitialized eax in the original.

## 2026-10-09 — Monitor: RE loop first, game, agents + forecast

- `tools/monitor_loop.py` (new): the loop stage comes from the processes (`grunt.py wave` / `--waves` parent / lanes,
  verifier.py, review.py, linktest.py) plus grunt's live phase; the verifier now writes its own phases (build tree,
  baseline smoke, group i/n) to work/verifier_state.json, which before only had bisect phases.
- Diff tiers on the Loop/Game tabs: functions in the generated diff_table.inc / replay_table.inc (verify tree's
  build) vs ledger-diffed; tier-3 inconclusive count from the play copy's csf_hook.log.
- Spend measured (7 days, 172 KB): the orchestrator's Claude jobs are the largest API-equivalent cost (~$0.85/KB
  of game code, mostly cache reads of long contexts) against grunts' Claude ~$0.09/KB; at list prices finishing
  costs ~$2k API-equivalent, Codex excluded (plan-only). The subscription forecast needs a few days of
  work/usage_history.jsonl (sampled by the monitor every 10 min) before it shows weeks per account.
- Fixed on the way: `monitor.procs()` used an undefined PROC_PAT (the tools line was always empty);
  `grunt.claude_usage` crashed on a usage window with `resets_at: null`; jobs started from `work/*` worktrees
  (work/t3) were not matched to the repo.

## 2026-10-09 — Backlog crush (a): diff numbers made trustworthy

- Inconclusive tier-3 runs already never counted as `compared` (hook/diff.cpp), so they were no evidence; they were
  just invisible. csf_diff_summary.txt now writes `K inconclusive` (also for functions with 0 compared) and the
  ledger keeps `inconclusive` per function.
- The 5 "mismatched" functions were diff noise, not code bugs: 0x45D4D0 / 0x6BF280 / 0x726C10 are the heap-read
  artifact (mismatches recorded by v1, before v2 made them inconclusive; same code hash, so the ledger kept them);
  0x544310 / 0x727250 are MSVC's void `std::string::_Tidy` (`jb` past the free straight to `ret`): Hex-Rays says int,
  the original's eax is the caller's. New exact checks in api.py: `eax_undefined_on_return` (a path reaches `ret`
  without writing eax) and `ret_unused` (every path after every direct call writes eax before reading it; a `ret`
  defers to the caller's callers or its Hex-Rays void prototype; any function whose address appears in the image is
  excluded: tables, vtables). difftable.py compares no return for either (~195 table entries, ~15 of them
  undefined-eax). `verify.py reset` dropped the 5 functions' diff evidence; the two _Tidy impls now initialize
  their result (was an uninitialized read).
- Worktree cleanup (5 merged ones) was refused by the permission classifier in this background job: left to the user.

## 2026-10-09 — Backlog crush (b/c): the edi cluster and a semcheck false positive

- **0x42F4F0 is plain cdecl(widget, rect).** Hex-Rays' `a1@<edi>` is MSVC's shrink-wrapped save of edi: `push edi`
  only after the null check (the early exit jumps past `pop edi` to `pop esi`); slot 0x30 takes no argument. The
  grunts had invented "anchorIndex" for it and bridged it with `__regcall` placeholders / naked asm that put garbage
  or `this` into edi. Rewritten from the disassembly; 0x431530 / 0x431680 / 0x431950 (done) called it with a shifted
  argument list (0x431950 passed widget = 0, i.e. never drew) and are fixed; 0x42F820 0x431350 0x431780 0x431840
  0x432F70 0x4337E0 from the queue resolved the same way.
- **semcheck false positive: outgoing args via an esp copy.** clang at /O2 writes call arguments through
  `mov eax, esp; mov [eax + 0xC], 0`, and `our_refs` counted [eax + 8] / [eax + 0xC] as field accesses ("field offsets
  the original never uses"). Registers copied from esp/ebp (`mov r, esp`, `lea r, [esp/ebp ...]`) now count as stack
  bases until overwritten or clobbered by a call. With it 0x53EA30, 0x5422D0, 0x72D120 passed as written.

## 2026-10-09 — Backlog crush (c): spurious register arguments settled (tools/regargs.py)

- `regargs.py`: exact, path-sensitive "is ebx/esi/edi an input" over the call graph (push R with a pop R in the
  function = callee-save; direct calls / tail jumps defer to the callee; indirect calls reach address-taken, hence
  standard-convention, functions; switch tables followed from the image; library regions standard; `lea r, [r]`
  padding, sub-register writes, `xor bl, bl` / `or r, -1` are writes; anything else unknown = real). Of 604 functions
  with a Hex-Rays ebx/esi/edi argument, 590 are spurious, 14 real (LTCG usercalls reading esi/ebx/edi directly,
  among them two queued smoke failures: 0x465310, 0x52F8A0).
- **All 124 done functions with such an argument: spurious.** The backlog's "90 done functions plain C++ although
  Hex-Rays gives @<reg>" are right as plain C++. The ones that bridged the phantom were wrong: 0x533D90 passed a 2nd
  arg to vtable slot 0x38 (the `push esi` was the save); 0x539340 read its stack args via __builtin_frame_address and
  fed 0x538F40 an edi it never reads. Both rewritten.
- db/regargs.json feeds `api.abi_sig` (proven-spurious register args dropped, usercall -> thiscall/cdecl/stdcall by
  what's left): 348 more addresses get a binary signature. Packets print a NOTE under a Hex-Rays prototype with a
  phantom register; the gate rejects `__regcall` (the grunts' bridge for it).
- Controls: mutant catch rates identical to master's gate (offset 19/40, global 37/40, rest 100%), fixtures and
  positive controls pass: the three semcheck relaxations today (esp copies, vtable-slot calls, base+disp) cost nothing.
- Left for a propagate run: push the corrected prototypes into the IDB (sync.py stage) so Hex-Rays stops showing them.

## 2026-10-09 — Backlog crush (d): the queue, 81 -> 6

- Codex (sol high, no Claude rungs) re-ran the 28 gate-exhausted items with the fixed gate / packets: 24 passed.
- In-game failures, root causes found statically:
  - 9 menu "release sub-object" clones (0x534B70 & co, 0x540870): `sub[0](sub, 1)` called the object's vptr instead
    of vtbl[0] (EIP 0x865B44 = a vtable in .rdata). The gate can't see it: slot 0 has no displacement and both
    versions end in `call [reg]`; only the load depth differs.
  - 5 VCall clones (0x52F950 & co): a helper indexing the vtable with the byte offset (slot 0x30 -> 0xC0); 0x52F950
    also called slot 0x160 twice. Offsets are template parameters now (constant displacements the vslot rule sees).
  - 0x4350C0 (CHud_Render) passed the phantom "anchorIndex" as 0x42F4F0's first arg; 0x45BBA0 read its +0/+4/+8
    fields as pointers (`CSF_AT(int*, self, 4)` then `*`): both fixed.
  - 13 more (HUD/menu, bisected with 0 crash-report names, i.e. likely victims of the 0x42F4F0 callers or flaky
    bisects) restored as pending for the verifier to judge on the fixed baseline.
- Review rejections fixed: 0x45AD20 (getter result as `this`), 0x45DFE0 (signed float->int64), 0x4624E0 (bool callee).
- Left in queue/ (6): 0x465310 and 0x52F8A0 take REAL register inputs (regargs.py: esi/edi read at entry, LTCG
  usercalls) that plain C++ can't receive; 0x535900, 0x55F550, 0x564CF0, 0x725390 still fail semcheck.
- Two more semcheck blind spots fixed on the last 4: CSF_GLOBAL references packed several to one .rdata section were
  invisible to the reference scan (it only reads 4-byte .rdata sections, since strings look like addresses): the
  object's COFF symbol table now gives the first dword of every named non-string data symbol (0x55F550, 0x564CF0
  pass as written). Chains of base adjustments fold (`lea eax, [esi + 0x14C]` + `add eax, 0x1C` twice: 0x725390).
  0x535900 rewritten from the disassembly (the Codex answer's control flow diverged). Controls identical to master's
  gate on the same sample. queue/ is down to the 2 real-usercall functions.
- Verifier on the result: the 13 in-game items restored without a fix were wrong to restore (group bisect: more than 4
  culprits, 0x42FF80 0x431F80 0x435F80 0x462580 named); re-quarantined. Everything else verified in game (pending 0).
  `--full` is blocked by a flaky `loadsave 1.sav` crash of the verified baseline itself (eip=0x19EBAE, on the stack;
  a return address near it points into 0x4360A0; 18 bisect smokes all passed): the verifier now keeps that smoke's
  output (work/baseline_fail_*.log) for the next occurrence.

## 2026-10-09 — The 13 in-game failures

- **The flaky `loadsave 1.sav` baseline crash** (eip on the stack, ebp = 2) was verified code: 0x42DA20 passed an
  argument to vtable slot 0x18 that is really 0x6C8740's second (float) one (0x6C8740 pops 8: formatted + the record's
  y), so ours left the stack 4 bytes off whenever the debug-stat path ran. Every "breaks the verified baseline"
  quarantine with eip 0x0019EBAE (0x42DB20 0x42DF60 0x544920 0x544980 0x78CE20 0x78CE30) was a bystander of it.
- Real culprits, found from fresh crash reports once the baseline held:
  - 0x547CB0 bound its vtables with CSF_GLOBAL and stored the *value* (the first slot) as the vtable: eip 0xCCCCCC00
    in the menu tour. Gate: CSF_GLOBAL on a vtable address is an error (83 files bind vtables that way but take the
    address; correct at runtime, wrong for the standalone build: cutover churn).
  - 0x462580 passed a lone uint32 where 0x4F6E90 fills a 0x30-byte record (index, two handles, weight, ...): the callee
    overwrote the frame, saved ebp = 0.3f. Same class as 0x461750; the packets' stack-object list covers it now.
  - 0x431F80: slot 0x58 fills a 4-int rect into `int bounds[3]`.
  - 0x435F80 / 0x778860: a by-value string declared as the 12-byte C_0082d410: clang copies such a small struct into a
    local, so &fillValue (passed on to 0x435290, which reads all 28 bytes) pointed at a local, not the caller's copy.
    Declared legacy::string (28 bytes, passed in place). 0x778860 had never run (hits 0).
  - 0x463FA0: correct code, but its LTCG caller 0x468310 keeps `this` in ecx across the call (no reload before
    0x467CC0) and the original never writes ecx. New `HOOK_KEEP_ECX(addr, fn, nargs, module)` (hook/hook.h): a naked
    stub saves ecx, copies the stack args, calls fn, restores ecx. regargs.callers_keep_ecx / needs_keep_ecx find
    them exactly (direct callers reading ecx after the call before writing it, incl. through the next thiscall /
    virtual call): 19 done functions needed it, all switched; packet NOTE + gate error for new ones. 8 more done
    functions look like callers reading an ecx the callee *produced* (0x697330 & co): not handled yet.
- semcheck follows esp/ebp copies through -O0 spill slots.

## 2026-10-09 — waves by centrality restarted; the rename pass never got through

- `wave --waves -1` from master: scene_loader (127 ok, 3 quarantined incl. 2 by the regression smoke),
  char_action_fsm (265 verified, 0 quarantined, 1/14 review reject), m44FC80 = scene_manager (103/104).
- Module naming is a wave step now: captain family -> `captain modules` on the wave's unnamed modules -> grunts; a
  held name is retried at wave end (function names then exist; packet shows 40 named functions, was 16).
  m44FC80 was held at 0.6 three times (mixed module, cohesion 0.28): named scene_manager by hand.
- The wave-prep rename pass (rename_classes + fields.py fixbuild + build) failed every wave and was reverted silently;
  the revert left field_consensus.yaml modified, and an unchanged consensus then skipped the pass for good. Four bugs:
  1. virtual-call wrappers are named after their slot's target; a target named since the last header regen
     renames the wrapper (vSetPosition_54 -> vScalePositionAndSize): fixbuild maps old wrapper -> slot -> new name.
     ~400 such call sites had drifted; all checked slot-identical.
  2. consensus names at an offset inside the embedded base (CCharActionFsm +0xC) were rewritten into src but never
     declared (the derived struct starts after the base): load_names files them under the owning base.
  3. classes.yaml field keys are hex strings *or* ints; fixbuild and rename_classes read ints as hex. Silent miscompile
     found: 0x50D4C0 CMenuManager_ReleaseOwnedScreens released +0x1C twice, never +0x28 (int 28 read as 0x28).
     A scan of all `->m_x ... // +0xNN` comments against the header's offsetof finds no other.
  4. rename_classes' "single class type" test ignored plain classes (S_<ea>): `buttonArray->field_000c` (an
     S_0041e5f0) became CGuiControl's m_pRenderer.
- 2026-10-10, end of run: waves m4FDC70, m47F930, m44FC80, m467AD0, m521C10, m496080, m4E8C60, m783430, m7685A0:
  ~1100 functions produced, per wave quarantine <= 2.6%, review rejects 3/56, 0 pending; game 21.9% by bytes.
  - Rename pass case 5: a wrapper's target implemented since the last header takes its real prototype
    (CSoundInstance +0x28: (int, uint32_t*) -> (int, float)); a caller's nullptr no longer compiles -> fixbuild
    writes 0.0f/0 (same bits). int<->float flips with non-literal args would compile silently: backlog guard.
  - Class 0x833EE8 = CPlayerCharacter (user): captain held CCommando / CPlayerCharacter at 0.6 before and after
    the wave; a class retry at wave end is not worth it (same confidence, $0.15).
  - One regression smoke crashed in RW (0x7FED10 null+0x68, callers original 0x70A6A0/0x6DFE00), passed 3 re-runs:
    flake, first sighting. A failed regression check stops `--waves`; restarted by hand.
  - Codex stand-in for S/M (when reserved/paused) is Sonnet medium now (user). Quota ran out at wave 10
    (gfx_matrix: 60 deferred); the loop would have swept every module deferring everything: stopped by hand.

## 2026-10-10 — Loop simplified (user); emulated diff spike

- **Names out of the wave.** The wave-prep rename pass had failed every wave, needed five fixes in two days and
  miscompiled once (0x50D4C0); the A/B showed names don't move the pass rate. `tools/naming.py` now does all naming
  at milestones under one invariant: every object's code hash (verify.code_hash) is the same before and after, or
  src/ and the names are put back. Negative control: a pure rename keeps the hash, a vtable slot change is caught.
  First run (no captain): header regenerated (2 wrapper names had drifted), 3995 objects identical.
- **Review**: one rolling sample weighted to functions that never ran (gate 5x, diffed 0.2x), report per evidence
  tier; out of the wave and the stop rule (3-14 reviews per module said nothing about the module).
- **In-lane Codex review, 14 rejections read**: 4 real (dropped argument 0x45F560, wrong virtual slot 0x71CBF0,
  undersized stack block 0x4ED030, ecx argument 0x71A510), 10 not behavior by our own fidelity rule (load/store
  order without a call between, INT_MIN, x87 precision, NaN order, and 3x the phantom edi of 0x42F4F0, which pushes
  grunts to the __regcall bridge that crashed). Kept; REVIEW_SYSTEM now excludes those kinds.
- **Wording**: "installed in a passing smoke" is not "verified". 1917 of 3976 done functions never ran.
- **grunt.py** 1882 -> 1621 lines: DeepSeek client, batching, `run` reduced to redo/A-B; 21 policy flags ->
  db/lanes.yaml. Setup-era scripts -> tools/setup/.
- **tools/emudiff.py (spike)**: Unicorn runs the original's bytes and our COFF object from the same state (exe
  image mapped, everything else filled deterministically on first touch, 8 input profiles); outside calls are
  mocked events; compared: call sequence + arguments + ecx, final non-stack memory, eax/st0, esp, callee-saves.
  `tools/ida/export_calls.py` exports what each call site pops (export/calls.json, 76145 sites, 2 s).
  - Speed: 426 functions in ~35 s, no game.
  - Against the in-game diff (426 diffed functions): 410 same, 15 inconclusive, 1 DIFF (0x728160: 4th cdecl
    argument uninitialized where the original pushes ecx; not judged).
  - Known bugs, every historical revision compiled (20 functions): buggy revision flagged and fixed one clean for 6
    (0x547CB0 vtable value, 0x435F80 by-value string, 0x534B70 vptr vs vtbl[0], 0x45AD20 getter result as this,
    0x431950 shifted arguments, 0x539340 callee-saves); 6 inconclusive in every profile (0x462580, 0x431F80,
    0x45DFE0, 0x4624E0, 0x461750, 0x45BBA0); 7 current revisions still flagged, of which looked at: 0x42DA20 real
    (binds 0x6C8620 as int, the original tests `al`), 0x42D3C0 false (ecx passed to the shared `return 0.0f` stub),
    0x533D90 false (IDA's pop count at a virtual call is the old wrong belief); 0x50D4C0, 0x42DF80, 0x4350C0,
    0x42FF80 not judged.
  - Samples of 300: never-ran tier 247 same / 30 inconclusive / 23 DIFF; executed tier 238 / 38 / 24. Of 3 looked
    at: 0x544C60 real (MenuSaveGame_CopySaveEntry passes src+0x1C to the string assign; the original passes src and
    adds 0x1C afterwards; gate-passed, never ran), 0x52C6A0 false (undefined eax in both), 0x53D8B0 unclear
    (naked asm helper; possibly the loader).
  - Verdict: a cheap new evidence source that already found defects nothing else did; not a replacement for
    semcheck or the in-game diff yet (false-alarm kinds and the inconclusive share are listed in the backlog).
- Second pass (user): diff tier 3 v3 dropped in favour of emudiff; canary verifier groups removed (a wave ends with
  its verify step and a full baseline smoke anyway); concluded A/B switches removed (CSF_PACKET_LEGACY, CSF_API_JSON,
  CSF_PSEUDO_DIR, CSF_CLASS_VIEW, CSF_CODEX_SLIM); the csv statuses `verified` / `escalated` never existed in data.
  Kept on purpose: the Codex CLI transport (fallback if the direct endpoint changes) and captain's batch `classes`
  job (not in the loop, but not shown dead). 10 merged branches and 8 worktrees removed; .xwin-cache untracked.

## 2026-10-11 — Docs and leftovers audit

- PLAN.md checked against status.py, tools/, queue/ and db/: the finished "backlog crush" focus removed (its
  queue 2 / pending 0 were long wrong: 25 / 3), the escalation queue is a focus item, the tier 3 decision moved to
  Decisions; backlog: the three "what a call pops" items merged, the two savegame items merged, tier 3 removal is
  its own item with its footprint, counts that status.py prints dropped (they had drifted: globals 346 -> 757,
  vtables 81 -> 140). "Module board" folded into the intro, Foundations shortened, decisions in date order.
- status.py counted numbered open decisions only (the one decision is a bullet: it printed 0).
- Removed: tools/setup/rtti.py + db/rtti.json (the game has no RTTI; nothing read the file), tools/sweep.sh (no
  callers; `verifier.py --full` with CSF_VERIFY_SAVES does it), the DeepSeek balance panel (limits.py, monitor.py),
  canary wording (verifier.py, monitor_loop.py). Kept: the deepseek / flash / deep mappings in the monitor and the
  gate's failure kinds (work/grunt_log.jsonl still has 1364 such rows), tools/classinfo.py (the captain uses it),
  the manual IDA helpers (decomp.py, decomp_err.py, check_types.py, init_db.py: only tools/ida/ touches the IDB).
- Found, not fixed: the monitor's Loop tab still shows captain naming as wave stages (backlog).

## 2026-10-11 — OOM: Unicorn instances wait for the cyclic collector

- Two global OOM kills (10:48, 10:51) took the Claude session and the detached wave loop with them: a scratch sweep
  (8-worker pool over emudiff.Run, thousands of ids) reached ~1.9 GB a worker, 15 GB RAM + 4 GB swap in seconds.
- Cause: unicorn's Python hook closures refer to themselves and to the Uc, so a finished Run (exe image mapped,
  ~10 MB) is garbage only the cyclic collector frees, and that one counts objects, not megabytes. `hook_del` does
  not break the cycle. Plain `emudiff.py --tier diffed --n 40` peaked at 3.5 GB in one process.
- Fix: Run.__init__ collects every 16 Runs. Same 40 functions: 257 MB peak, same time (4.0 s), identical verdicts.
- The lanes' "link" phase was `api.py build`, not the linker: ninja takes 1-2 s, api.py build took 98 s (4+ min
  next to a busy machine) after every batch. packet.disasm asked pefile for the memory-mapped image on each of its
  38,681 calls (2 ms each, 81 s). The image is built once now: 7.9 s, db/api.json byte-identical.
- Where a wave's time goes (11 waves of 10-09/10-10 rebuilt from grunt_log, verifier_log, work/calls and commits;
  18-30 min each, nearly independent of size):
  - production 8-21 min at an average of 2-9 calls in flight out of 23 slots. The last 20% of results take
    2-13 min: X runs 2 at a time (a cap from the Opus-first days), 13 X functions = 9-13 min alone. Outside model
    calls a function costs 1 s (median); the p90 of ~100 s was results waiting behind a batch commit (api.py build).
  - fixed part then: prep 2-3 min (captain + rename pass; now sync + re-decompile, 6 s), last batch commit ~100 s
    (now ~10 s), verify tree build + baseline smoke 45-130 s, group smoke 34-54 s, link test 96-136 s.
  - link test: 4,049 llvm-nm calls ran one after another (35 s with nothing to compile); in the pool now: 5.8 s,
    same content (ties in the counted lists come out in another order).
  - a change to a shared header (hook.h, globals.h, auto_classes.h) recompiles all ~4,060 files in each of three
    trees: build ~100 s, verify tree 98 s, standalone 134 s. The per-wave rename pass did that every wave.

## 2026-10-11 — Rolling waves

- `wave --waves N` is one lanes run now, not a subprocess per module. Up to `--overlap` (3) modules are open; the
  next one is admitted when the open ones have handed out all their S/M work and an S/M slot idles; the older
  module's candidates keep priority. A module with nothing in flight and nothing submittable is closed: commit,
  then verifier --module + link test + report + regression check on one background worker (one module at a time:
  the verifier's lock is non-blocking), while production goes on. A stop rule ends the run like the stop file.
- Why: 78-97% of a wave's tail had X at its cap of 2 with ~20 slots idle, and verify + link test (2-4 min) held the
  next module back. Dry run with fake calls (6 modules, real tier mix): overlap alone -13% (X stays the limit),
  so `x_jobs` 2 -> 4 as well. Not measured: Codex at 4+ concurrent high-effort calls a login.
- Changes of behaviour: a module can close while a clone sibling of another module is in flight (its remaining
  clones stay todo for this run); wave_report counts verifier events since the module's verify step started.
- First rolling run (10-11, 47 min until stopped by hand): 910 functions through the gate (19-27 a minute, against
  5-10 before), 5 escalated, 9 modules admitted; modules closed out of order and verified in the background
  without touching production. First module: 561 s from admission to report. The loop is no longer the limit:
  the run took Codex from 9% to ~70% and the grunts' Claude account from 4% to 95% of their 5h windows.
- Found in that run, fixed for the next start:
  - Codex past its reserve sent S/M rungs to the Claude stand-in also while that account was paused: every M
    function came back deferred with Codex at 70%. The stand-in is used only while Claude has room.
  - A module whose remaining work waits for quota (deferred functions, L/X while Codex is paused) stays open, and
    when nothing can run the loop waits for the window and offers the deferred functions again. Before, it would
    have gone through the whole ranking closing module after module with holes.
- The full regression smoke after m4053E0 crashed 3 s into the savegame start (null read at 0x473F09, original
  code); the bisect quarantined 0x407450 (new) with 0x496080 and 0x470B20 (older, verified; both passed the same
  regression twice earlier in the run). Open: which of the three is wrong (queue).
- Restarted on the fixed code at 12:52 with Claude at 96% and Codex at 79/87%: 238 more functions (S on Sol low
  as Haiku's stand-in, M/L/X on Sol high), then both Codex logins at 100%: 126 functions deferred, the three open
  modules stayed open, no further module admitted, "waiting" printed. Windows reopen ~15:15; the run resumes by
  itself. Sol low as S writer so far: 50 functions, 92% first try at the gate (Haiku low 94% in the same run).
- One 5h window of both accounts lasts about an hour of rolling waves (~1,150 functions); a Codex window is ~12%
  of the weekly limit.

## 2026-10-11 — Queue: undersized stack locals (a bug class), first fixes

- 0x4223F0 (broke the verified baseline) built a 12-byte vector in `S_00526d40 heading_vector;`: that struct is 4
  bytes in auto_classes.h (sizes there are lower bounds), so it overwrote its saved esi and the stack cookie.
  emudiff showed it as "ours changes ebx/esi/edi/ebp". Scan of src/auto: 31 by-value locals of generated struct
  types in 28 files, 5 provably reaching past sizeof through CSF_AT (0x41D270, 0x4227B0, 0x4345E0, 0x4346B0,
  0x464E00: emudiff ran away at 2000 calls before, "same" after); the rest hand the local to callees.
- Fix for the 26 that were plain locals: a buffer of the original's frame size (`sub esp, N`: an upper bound of
  any object in it) and a reference of the struct type. Two files lay a local struct out by hand (0x4EA770,
  0x553270): left as they are.
- gate: by-value locals of S_/C_ types are rejected (kind local_struct; members of a struct defined in the file
  are not locals); the writer prompt says how to size the buffer.
- emudiff gaps seen while triaging the queue: HOOK_KEEP_ECX / HOOK_TAIL wrappers read as runaway calls (their
  `call [impl]` goes through an unrelocated pointer); a CRT helper the original calls and ours inlines reads as
  a different call target (0x624D10).

## 2026-10-11 — Queue 42 -> 18: what the items were

- Triage: every queued file compiled and run through emudiff first. It named the defect outright for most of
  the real ones; "N calls vs 2000" on a HOOK_TAIL / HOOK_KEEP_ECX function is a real runaway (stack imbalance),
  not a wrapper artifact as first assumed.
- Fixed and installed again (26): wrong/missing `this` (0x406020 forwarder dropped ecx, 0x41E380 and 0x4686F0
  passed the wrong object, 0x4FE130 missed `lea ecx, [eax+0x22A4]`, 0x5011E0 passed a field for the object),
  swapped stack arguments of a virtual call (0x407450), a dword argument typed char (0x521CF0), frame smashes by
  undersized struct locals (0x4223F0, 0x470B20), tail thunks (0x521D90/DA0/DB0 called through the address of a
  function-pointer variable; 0x48E5D0, 0x475B60, 0x476300 needed `ret N`), a callee bound without `this`
  (0x46E590 -> 0x46BE40), x87 precision (0x6AB8D0), and the bystanders of those crashes.
- Tooling the queue asked for:
  - hook.h: hook::tail_ret(eax, N) for `ret N` paths (a third parameter made fn pop the target's argument; also
    in 0x48E5F0 and 0x48E710, accepted today); HOOK_KEEP_ECX_CDECL (the wrapper assumed fn pops its copy of the
    arguments; 0x4068E0 is __cdecl).
  - gate: by-value struct locals; tail functions pop nothing and name the original's pops; keep-ecx wrapper vs
    the original's ret; a catch handler's `mov eax, <label>; ret` is not a return of the function (0x410130 and
    0x486550 were rejected for it); the ecx/edx read scan follows branches.
- Two of my own slips, both caught by the verifier: the mechanical local rewrite also hit members of hand-laid-out
  local structs (0x4344A0 crashed at startup, 0x41C960; both restored), and 5 of 9 functions resubmitted as
  bystanders were culprits after all (0x4068E0, 0x46BE40's caller, 0x490790, ...).
- Left in queue/ (18): 0x490790 (fails in game, cause not found), 0x410290, 0x419B50, 0x4ED1C0, 0x6A2510,
  0x6A2910 (crash bisects, not looked at), 0x454070 and 0x465310 (don't compile: unknown class type / usercall),
  the four zlib register-argument functions, three review findings, four gate failures.
- OPEN: the full regression is flaky since ~13:10 (failed 13:13 and 14:21, passed in between): crashes in the
  savegame start at different places (a jump into heap data; a null read in library code with NaNs on the stack).
  Not attributable by bisect. emudiff over the 474 functions installed 12:40-13:14: 32 DIFF, most of those looked
  at are noise (last-bit float rounding, garbage upper bytes of a pushed byte); not narrowed down further.

## 2026-10-11 — Queue 18 -> 0; register arguments; the deflate half of zlib reopened

- All 18 remaining escalations resolved and installed in passing smokes. What they were:
  - Hex-Rays attributes a pushed argument to the wrong call when a push for call B is emitted before call A
    (`push x; call A; ...; call B`): 0x410290 (the `3` belongs to the collection's +0x34 query, not the hit's +0xC),
    0x490790 (the stored index is the +0x8C virtual's argument; the invert call gets the matrix holder: ours
    dereferenced an index, the failure nobody could place earlier). Two review findings (0x480E60, 0x4ECDB0) were
    the same artifact read the other way round and were dropped. The asm's pushes and each callee's `ret N` decide.
  - wrong object: 0x419B50 (`this` of the lookup is the member at +0x30), 0x4ED1C0 (subobject at +0x22A4).
  - one stack object written as two buffers (0x6A2510), a 16-byte descriptor passed as a lone float (0x6A2910).
  - forwarders that tail-jump to a virtual are bound `int __cdecl ()` by the ABI export; 0x454070 called two of
    them without `this` and the argument. The packet now says to bind such a callee by what the call site passes.
  - 0x465310 (script command dispatcher, 369 cases): commands 2003, 3002, 4110 read the argument one dereference
    short. Found by emulating every command id through both versions (scratch dispemu.py: pre-map the page the
    command pointer lands on and write the id), after checking all 344 call sites against their bindings.
  - gate noise: Hex-Rays' `memset(p, 255, n)` for a run of -1 dwords counted as a missing constant (semcheck);
    an aggregate initializer hides a vtable address in .rdata (0x4EA140: store it by an instruction); a pointer
    into a stack array shows its indexes as field offsets (0x4523C0, 0x468EE0: index the array itself).
- Register arguments (LTCG): hook.h has HOOK_REGS(addr, fn, retN, module) with
  `uint32_t __cdecl fn(hook::Regs* regs, const uint32_t* args)` and hook::call_regs(addr, &regs, stack, n) for
  calling such a function. api.real_regs = regargs.json "real" (ebx/esi/edi, exact) + a read-before-write scan
  for eax; the gate requires HOOK_REGS (or regparm when it is eax alone) and rejects a CSF_FUNC of such a callee;
  the packet says both. Only 22 game functions are affected, 17 of them zlib's deflate half.
- zlib deflate never runs in a smoke (hits 0 in ~200 passing smokes), so 10 functions there were accepted with
  register arguments on plain conventions (0x6A33A0 takes eax+edi+1 stack dword and was `__cdecl (3 args)`).
  They are todo again (the game runs the original); 0x6A4610, 0x6A46A0, 0x6A48A0 are written with HOOK_REGS.
  Lesson: "installed in a passing smoke" is no evidence for code that never executes; `hits == 0` functions rest
  on the gate alone. Worth a look before the 50% playtest: how much accepted code has never run.

## 2026-10-11 — Half of the verified code has never run; emudiff sweep over it

- db/verify.json: 5211 functions installed in passing smokes, 2494 of them with hits == 0 (never executed in any
  smoke). For those the smoke proves only that installing them is harmless.
- emudiff over the 2494 (4 processes, 2 minutes, no tokens): 2029 same, 268 inconclusive, 197 DIFF, 4 errors.
  The list is work/emu_nh_diff.txt (gitignored; regenerate with emudiff over the hits == 0 entries). By kind:
  82 call arguments, 24 call counts, 21 call targets, 18 ecx at a call, 37 return values, 4 memory, 3 ours runs
  past the cap, 2 ours changes a callee-saved register (0x73C670, 0x73C6C0).
- Looked at the ecx ones only. Real: a call that passes nullptr as `this` where the original leaves ecx as it
  came in (Hex-Rays shows the call without it): 0x49BAF0, and the forwarder 0x4053E0 (`jmp 0x4059D0`) written
  __stdcall. Both fixed. Statically: 8 files pass a null `this` to a callee that reads ecx (0x45BAC0, 0x462DA0,
  0x462DC0, 0x462DE0, 0x4C5E50, 0x551700 still open), and 22 implementations are not __fastcall although they
  pass their incoming ecx on to a callee that reads it; 5 of those 22 run thousands of times without trouble, so
  that second rule is not exact yet and is NOT in the gate (tried in api.reads_before_write, reverted).
  Noise among the ecx diffs: ecx left over at a call of a function that ignores it.
- OPEN: the other ~175 DIFFs are untriaged. A playtest reaches code no smoke does, so this list is where its
  crashes will come from; cheapest next step is the 24 call-count and 21 call-target differences.

## 2026-10-11 — The flaky regression: reproduced under load, two culprits fixed, one rare crash left

- The waves stopped again at 16:05 on the full regression (savegame start, passed on re-run). Running that start
  on all 8 smoke copies at once makes it fail 13 of 16 times; with every src/auto function disabled 0 of 16. So
  it is ours, and load turns a rare failure into a frequent one: bisectable by repetition (tools/flaky.py).
- Bisect with 4 runs per half (16 rounds, 20 min, no tokens) -> 0x4BFDD0 fails 8/8 alone: a 12-byte result
  written through `&a3` of three separate dword locals (Hex-Rays splits the object), stack cookie failure
  (exit 0xC0000409). Then the crash report's stack named 0x4BE970: `mov ecx, esi; call` keeps the linked object
  as `this`, ours passed the resolved base (crash at 0x4DBFF9). Both were accepted at 15:54 (ai_core), so they are
  NOT the 13:13 / 14:21 failures. After both fixes: 2 of 24 under 8x load.
- Left (OPEN): eip 0x7FED32 (RenderWare) reading index 0xFFFF with NaN vectors on the stack, called from the
  original 0x70A6A0 (skinned model update: RpHAnim matrices). Something of ours leaves NaNs in animation /
  matrix data now and then. ~8% under 8x load, far less alone; too rare for the halving bisect (a test would
  need ~36 runs). This is the 14:21 failure; the 13:13 one (jump into heap data from the 0x4DB686 recursion) did
  not show up in 64 loaded runs.
- The ai_core wave report said "executed 3, never ran 87" for functions that crash the savegame start under
  load: per-function hit counts come from single smokes and understate what a longer or slower run reaches.
- Also from this run's queue: 0x513BA0 (address of a member vs the pointer stored there), 0x516980 (arguments
  swapped), 0x6C58D0 (push emitted one call early, again), 0x4F8150 (missing wrappers). 0x76A540 (binding
  catalog, 7300 lines of asm, the answer covers 11 of its calls) stays in queue/.

## 2026-10-11 (evening) — Waves stopped 17:20-21:xx: four culprits by load bisect, one quarantined, diffing found off

- The 16:52-17:15 waves (cheats, ai_attack_tasks, game_bdd_loader) broke the savegame start for real; the stop
  rule ended the run. Found with tools/flaky.py + a "disable half, is it cured?" search on the 8 smoke copies:
  0x47D990 (a virtual of the path object called without `this`: Hex-Rays' `__stdcall **` cast copied),
  0x47B950 (`push edi` before four pushes is the callee-save, not a fifth argument: stack imbalance),
  0x47E350 (Hermite constants swapped: 3.0 scales t^2, -2.0 scales t^3), plus 0x4BFDD0 / 0x4BE970 before.
- 0x47E350 (spline position) is QUARANTINED, not fixed: with its hook on, the NaN crash (eip 0x7FED32) is
  55-80% under 8x load, with it off 1/16; after the constant fix it reads identical to the asm. Notes in
  queue/0047e350.md. With it quarantined: 6/24 under load, all the same NaN crash, so a second, weaker
  contributor exists (the 14:21 flake, ~1/16 before today's waves). OPEN.
- Lessons for the search itself: a test needs enough runs for the failure rate (4-6 runs at 50% sent two
  searches down the wrong half; 16 runs with a threshold worked); several crashes at once need the signature in
  the pass criterion; `tools/deploy.sh` + smoke ingest from the main build dir rewrote db/verify.json (reverted;
  flaky.py now sets CSF_NO_LEDGER).
- FOUND, NOT FIXED: in-game differential testing has been off in every verifier smoke. work/verify-tree's
  export / ida / .venv links (made 2026-10-08) point into a worktree that no longer exists, tools/difftable.py
  fails there, verifier.build_tree ignores it and the build gets an empty diff table ("diff (0 functions
  compared)" in every smoke log today). And with the tables present (main build dir, 1078 diffable + 1077 tier-3
  functions) the game dies at startup with 0xC0000409 even in a single smoke. So the "diffed" tier has had no new
  evidence for days, and switching it back on needs the harness (or a diffed function) fixed first.
- 0x76A540 is generated (tools/gen_script_catalog.py: symbolic walk of the straight-line asm, 423 registration
  calls, emudiff same 8/8) and installed; module m7685A0 renamed mission_script_catalog (it is the script
  language's type/command catalog; function prefixes follow at the next naming pass). 0x50F060: one 0x28-byte
  object split in two buffers (startup hang) fixed.

## 2026-10-11 night — in-game diff repaired (tiers 2 and 3), 51.5% game code, playtest dropped

- **The diff harness had been off in every verifier build**: `work/verify-tree` had dangling `export`/`ida`/`.venv`
  links, `difftable.py` failed silently and the table was empty. `prepare_tree` repairs the links, a failing
  table generator now fails the build. Tier 2: nested diffs overwrote the static snapshot (nested calls now run
  ours). Tier 3 startup crash (0xC0000409): a 1-byte out flag at `[ebp-1]` replayed as a dword into ours' frame;
  watched stack windows are capped at the recorded function's frame pointer.
- **Tier 3 false alarms, all in the table generator / replayer** (17 functions quarantined by the first full
  baselines, 12 of them unchanged code): `this` used after a call on another branch was outside the snapshot
  (0x716490); thiscall decided by Hex-Rays' prototype instead of the code (0x52B770: no snapshot, timer advanced
  twice); CRT `memcpy` on the heap ran for both (0x4BFE70 family: such functions are out of tier 3); a frame dword
  read before it is written = stack garbage as an argument (0x47EE50, 0x488FF0, 0x4696C0: out of tier 3); stack
  window was 64 bytes (0x415AE0 copies a 0x200-byte settings block: now 1024, deltas 1024).
- **Real bugs tier 3 found**: 0x6AB9E0 (value read before the normalize, original reloads it after), 0x41EA70
  (stored its own sum instead of the callee's st0), 0x7123E0 / 0x447940 (wrong call's eax returned), 0x70F3B0
  (two arguments are the previous call's, still on the stack), 0x6AB930 (x87 keeps the quotient unrounded).
  Lesson again: `test ah,5; jp/jnp` reads as "not below" / "below"; I misread it once and emudiff caught it.
- Queue notes now carry the function's own DIFF lines (smoke prints up to 3 per function; the log tail never held
  them). emudiff: `long double` returns are st0 too.
- 0x508F50: a wave reimplemented the address devcmd hooks by hand (`Window_FitClientArea`) -> visible fullscreen
  smokes. Lanes and gate now refuse addresses hooked by non-auto `src/` code. WinMain (0x509250) parked.
- **Naming pass**: with the captain it spent ~$1.35 of Opus on 12 classes, all held below the confidence bar:
  stopped; class naming is skipped until more of each class is implemented (user). `--no-captain` failed and put
  everything back: after applying 29 consensus field names `fields.py fixbuild` left `field_0008` uses in
  0x4D9170 and 0x421B30 (classes C_0083dbb0, C_0082f9f8). Open: fix fixbuild, then prefix the ~150 unprefixed
  functions whose class is known from the binary (`db/types/this_class.json`).
- User: no playtest milestone any more, run the waves to 100%. 27 functions pending (the fixes above), waves stopped.
