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
