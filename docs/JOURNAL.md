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
