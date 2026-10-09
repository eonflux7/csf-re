# CSF-RE plan

Functional (not byte-matching) C++ reimplementation of *Commandos: Strike Force* PC (`CommXPC.exe`),
built hook-first: reimplemented functions are swapped into the running original game until nothing original
remains, then the same source builds a standalone exe (cutover).

This file is the living plan: **the loop** (how work is done), **current focus**, the **backlog** (what is left,
by area) and decisions. Progress per module is generated, not written here (`tools/status.py --modules`,
`tools/grunt.py wave --next`). Findings go to `docs/JOURNAL.md`. Open decisions for the user are at the bottom.

## Facts about the target (see JOURNAL for evidence)

- `CommXPC.exe`: MSVC 7.1 (VS2003), built 2006-02-23, imagebase 0x400000, not packed. d3d9, dinput8, dsound, winsock.
- **No RTTI** (`/GR-`), **no symbols** on any disc (PS2 proto/retail, Xbox). Names come from strings/asserts/structure.
- Developer: Pyro Studios. Identifiers and strings are **Spanish**. Original source root `c:\CommX\Src\Main\` (Xbox XBE).
- Statically linked libraries (never reversed, only their boundary API is named):
  RenderWare 3.x (core + D3D9 driver + toolkits), RenderWare Audio (DirectSound), D3DX9, Quazal NetZ,
  MSVC 7.1 CRT/STL, libpng 1.0.12, zlib. Dynamically: binkw32, vorbisfile.
- `RwEngineInstance` = `0x8b68c0`. Region map: `db/regions.yaml`.

## Environment

- Everything runs in WSL. IDA 9.3 Linux at `~/ida-pro-9.3`, driven headless via idalib from `.venv`.
- IDB: `ida/CommXPC.exe.i64` (gitignored, derived). Exe copy: `work/pc/CommXPC.exe`.
- Game builds (read-only originals): `/mnt/e/dev/csf-builds/`. Play copy for hook testing: `/mnt/e/dev/csf-play`.
- Toolchain: clang-cl 21 + lld-link, `i686-pc-windows-msvc`, MSVC SDK via `xwin`. x64dbg under `E:\dev\tools`.
- Third-party sources/tools go in `work/tools/` (gitignored). Never leaked RenderWare SDK source; librw (MIT) only.
- Models: grunts on their own Claude account (`~/.config/csf-re/claude-grunts`, Haiku/Opus via `claude -p`) and
  Codex; the captain (naming) on the main account. Quota guards in lanes (`--claude-cap`, `--codex-cap`).

## The loop (per module)

One command runs it: **`tools/grunt.py wave <module>`** (`wave --next` ranks modules by centrality: undone
bytes x how much other code calls into them). Each step also runs on its own.

1. **Types first** (wave prep, automatic): `captain.py family` names the module's unnamed classes (agentic Opus
   session); `fields.py apply` pools the field names every grunt proposed (consensus per class+offset);
   `rename_classes.py` + `fields.py fixbuild` rewrite src; then the IDB gets it all (`ida/sync.py`: names,
   class structs, typed vtables, member/global types) and the module is re-decompiled, so packets show
   `this->m_field` / `this->vName()`. Gauge: `fields.py coverage --module M`.
   Between waves (whole program, ~45 min, no tokens): `tools/ida/propagate.sh` pushes what the binary proves
   (cdecl arg counts from callers' cleanup) and what we know (`this`, parameter and member class pointers) into
   the IDB, re-decompiles everything and lets Hex-Rays carry the types further (`this_class.py`); after
   `classes.py`, re-run `plain_classes.py` (classes without vtable). Then `api.py build`, `gen_classes_h.py`.
2. **Produce** (`grunt.py lanes --module M`): leaf-first selection, clone-aware; size tiers S/M/L with model
   ladders (S: Haiku low x2 -> strong; M: Codex x2 -> Opus; L and hubs (>= 25 outside callers): Codex).
   Packet = pseudocode + disasm + class fields + typed virtual calls + the canonical `bind as` line per callee.
   `normalize()` auto-fixes (address macros, canonical names, includes); the **gate** checks includes, compile,
   references vs the original (semcheck), `ret N`, ABI of every binding against the binary, no raw addresses.
   Batches are linked and committed **pending** (never installed in a passing smoke yet).
3. **Verify** (`verifier.py`, async; lanes start one group per 300 commits as a canary): its own detached tree at
   HEAD, the verified baseline smoked alone first, then module groups of pending functions enabled on top
   (FR01; `--full` adds menu tour + savegame), crash report -> culprit (else bisect via ini toggles), diff
   mismatches -> quarantine (`queue/`, status todo). Every deploy keeps pending hooks off.
4. **Review** (`review.py sample --module M`): Opus reads ~5% of the module's functions against the original;
   the rejection rate is the module's quality estimate; rejections go to `queue/`.
5. **Consolidate**: `api.py build` (automatic per commit: one name + signature per address, grunt names as votes),
   `linktest.py` (what is left before the standalone exe links; shown by `status.py`).

Between waves the orchestrator (Opus, interactive) works `queue/` and the backlog below.

## Module board

Generated: `tools/status.py --modules` (bytes done per module), `tools/grunt.py wave --next` (what to wave next),
`tools/fields.py coverage` (types), `tools/verify.py report` (evidence tiers), `tools/review.py report` (quality).

## Current focus (priority order)

1. **Backlog crush (user, 2026-10-09)** — done (merged to master; the 13 in-game failures fixed on branch fix-13):
   queue 81 -> 2 (0x465310 / 0x52F8A0: real register inputs, need a usercall entry stub like HOOK_KEEP_ECX),
   mismatched 0, pending 0, the flaky `loadsave 1.sav` baseline crash fixed (0x42DA20). Follow-ups in the backlog:
   virtual-call argument counts (gate gap), the 8 "ecx-out" functions, 83 CSF_GLOBAL vtable bindings (cutover).
2. **Waves by centrality** with the new loop: `wave --waves -1` runs them in rank order with the stop rule and a
   full baseline regression smoke between waves. 2026-10-10: 9 waves done (scene_loader ... input_bindings, game code
   12% -> 22% by bytes, no stop rule hit); paused at gfx_matrix (all 60 deferred: grunts' Claude 95%, both Codex
   logins >= 90%). Resume with `wave --waves -1` once the 5h windows reset (`wave --next`: gfx_matrix,
   mp_spawn_points, bdd_character_defs, game_config, ...).
3. **Diff tier 3 v3**: indirect/virtual calls (862 functions blocked on them). v2 is merged; `T3 ... inconclusive`
   runs never count as compared and are now counted in csf_diff_summary.txt and the ledger (`inconclusive`).
4. **Manual playtest at ~25% of game code** (user; at 22% on 2026-10-10, ~2 waves away): the monitor shows live coverage (Progress `game` line, Areas
   tab table: hit / never run before); afterwards `verify.py ingest` records the hits.
5. **Types for the next waves**: field names of CMenuScreen, TCharTask, TAiState, CCharWeaponFsm; the engine family
   0x860178 and the 0x82E598 task subfamily (captain family at wave prep covers unnamed classes).
6. **Verification depth**: `verifier.py --full` over everything once (menu tour + savegame per group), then review
   samples per module (`review.py sample`) to get a defect rate for the 1278 gate-only functions.

## Backlog

### Loop & tooling
- [x] Monitor: verifier and review lines, per-area progress table (Live), area column (Modules tab)
- [x] Monitor tabs reworked: 1 Loop (stage pipeline from processes + grunt phase, NOW/NEXT/THEN, live stop rule,
  past waves, side loops incl. diff tiers 1-3 and the smoke pool), 2 Game (evidence/diff tiers, linktest, pace, ETA),
  3 Agents (orchestrator sessions, spend per model, forecast to finish; plan prices in ~/.config/csf-re/billing.yaml)
- [x] Wave stop rule (quarantine > 3%, Opus rejects > 10%, > 25% of the module left unverified -> exit 3) and
  `wave --waves N` with `verifier.py --baseline-only --full` between waves; reports in work/waves.jsonl
- [x] Functions over 600 B in the waves: tier X (Opus high x2, then Codex xhigh; 2 in flight), `--max-size`
  no longer caps; 639 functions / 697 KB were outside every wave
- [x] Spurious register args (2026-10-09, tools/regargs.py): all 124 done functions with a Hex-Rays ebx/esi/edi
  arg are right as plain C++ (590/604 program-wide are callee-save artifacts); two __regcall bridges rewritten; api,
  packet NOTE and a gate rule use db/regargs.json
- [ ] IDB: apply the corrected prototypes (db/regargs.json spurious regs dropped) in a sync.py stage + propagate
- [x] Fidelity redo of 0x72DE90, 0x729F60, 0x7292C0, 0x538A50 (2026-10-09): their double arithmetic stays (it stands
  in for x87's wider intermediate where an int is truncated from it: natural, not emulation); the volatile dead
  loads went. New output: `volatile` is rejected by the gate and named in the prompt's fidelity rule
- [ ] Rename pass guard: a wrapper whose target got implemented takes its real prototype; an int<->float flip of a
  parameter compiles silently in older callers (value conversion changes the bits). Diff old/new wrapper params
  in captain_prep and revert/flag on a flip with a non-literal argument (first one: 0x5266C0 +0x28, nullptr->0.0f)
- [ ] Tail-forwarding thunks (sets state, then `jmp [obj->vtbl+N]` with the caller's stack args untouched, count
  unknown): C++ can't forward them, grunts hand-write naked asm and name a C++ function in it (link error). 4/160
  escalated in audio_sound_manager (0x521D60, 0x523760, 0x527510, 0x521E80). Give hook.h a helper (like
  HOOK_KEEP_ECX) + packet NOTE, then re-grunt them
- [ ] 57 older src/auto files still use `volatile` (dead loads, uninitialized args passed on, forced global loads):
  rewrite at their next touch (gate now rejects them on re-grunt)
- [x] Review / in-game lessons in the grunt prompt (2026-10-09): a curated "mistakes that crashed or failed review"
  block (vptr vs vtbl[N/4], getter result as `this`, early pushes belong to the next call, CSF_AT value vs pointer,
  `test al, al` callees, signed __ftol2) instead of review.py's keyword buckets; extend it as reviews add kinds
- [ ] Re-test Sonnet for bulk function renames (captain functions) after waves name the callers
- [ ] Opus pass over the 44 held-back RW names (`rwname.py report`): neighbour pairs given the same name
      (Set/Get, Width/Height, Int32/Int16) and low-confidence toolkit internals (RpTeam, Rt2d, RtWorldImport)
- [x] Name the remaining ~80 unnamed modules just before their wave: automatic in wave prep since 2026-10-09 (captain family, then `captain modules` on the wave's unnamed modules, then grunts; below 0.7 confidence they stay unnamed)
- [ ] Locate Xbox XBE code, diff vs PC for structure confirmation (optional)

- [x] Gate: the original's constants and call order checked against Hex-Rays (semcheck.behaviour; 2/2358 false
      flags); compile fix-its without a model call; named Win32 constants measured and dropped (62 call sites)
- [x] Gate failure kinds logged per attempt (`fails`, `grunt.py cost`); packet: vtable references with the line
      to write (dropped vtable stores were 23% of rejections), no this-class on vtable stubs that never read ecx
- [x] IDA used fully: signatures for every function (api.json 2750 -> 21746 addresses; callee bind lines 40% ->
      98%), Hex-Rays feedback loop, typed pseudocode (member accesses 0 -> 8630 on a 3000-function sample)
- [x] The 4 ABI items the sharper ecx check found (bindings that dropped `this`): fixed, pending re-verification
- [x] Classes through Hex-Rays (`ida/propagate.sh`): undone methods with a class 42% -> 71%; A/B: named members in
      grunt output +43%, guesses -33%, pass rate and tokens flat (`CSF_AB_ARM`, `CSF_PACKET_LEGACY` for re-runs)

### Types & structure
- [x] Globals holding class instances / pointers, return values and stack objects passed as `this` typed through
      Hex-Rays (export_abi `arg0_src` g/r/l, arg_types.json, sync.py): 39 globals, 23 return values, 231 stack
      objects. Small effect (members +0.4%); returns need >= 2 agreeing callers (an allocator got typed from one)
- [ ] Unclassed methods left (29% of undone ones): what remains comes from lookups through containers / tables
      (untyped element pointers); needs element types of STL instances (db/stl.json) or table globals
- [ ] Member pointer types into src/types/auto_classes.h too (IDB only now: 227 members): needs a build check per
      member, code using them as uint32_t
- [ ] Field names of the menu/AI bases: CMenuScreen, TCharTask, TAiState, CCharWeaponFsm
- [ ] Engine family 0x860178 (86 classes: clump objects, HAnim animation, lights, cameras, 2D scenes) and the
      unnamed 0x82E598 task subfamily (factory 0x41E240, type ids 0..6)
- [ ] Core classes: layouts, vtables, sizeof with static_asserts (gen_classes_h already asserts known sizes)
- [ ] STL layout replicas for VC7.1 (vector/list/map/set) + template hook impls covering db/stl.json
      (element size per instance; nodes/buffers via game allocator sub_690DC0/sub_690E60) — excluded from waves
- [ ] Typed returns for virtual wrappers that return class pointers (now void*: header order)

### Verification
- [x] Diff: copies of `this` and contained callees (750 -> 788 functions); snapshot 16 KiB, 8 globals
- [x] Diff tier 3: functions whose callees have side effects (record the original's callee calls, replay them for
      ours; callee writes into `this` and thread safety are the hard parts; needs live iteration) - v2 2026-10-09
- [ ] AI combat coverage: the T* AI tasks and GUI input handlers only run in real play. Needs savegames taken in
      combat (user playtest) -> `CSF_SWEEP_SAVES="1.sav 2.sav ..."`, then `verifier.py --full`
- [ ] Executed coverage: 1278 done functions have only passed static checks (`verify.py report`)
- [x] Smoke pool (2026-10-09): 8 play copies (`tools/pool.py`, `playcopy.py`), several instances at once (mutex
      renamed in the DLL), fail-fast on CRASH, bisect runs capped at 2x the crash time + k-way across the pool,
      verifier starts side by side; default smoke 15 -> 30 s (adaptive end opt-in)
- [x] `9.sav` crashes ~40 s after load on the verified baseline: 0x461750 sized a 3-entry (0xE4 B) stack block from
      Hex-Rays' split `v19[38]` + `v20` (39 dwords); the callee's init overran it. Fixed (2026-10-09)
- [x] 0x42D3C0 HudQueryWidgetValue mismatches 200/200: returned eax where the original returns st0 (0x70DDF0 is the
      shared `return 0.0f` stub). Fixed + gate rejects int for st0 returns (api.returns_fpu, found 0x6FD180 too);
      the diff table compares st0 for them; `baseline()` quarantines MISMATCH lines too (2026-10-09)
- [x] Stack buffer sizes (0x461750): packets list every stack object whose address the original passes to a callee,
      with its room (to the next such object / the frame end) and the offsets accessed directly inside it (2026-10-09)
- [ ] Gate gap: a call through an indirect forwarder (0x4F5170 `mov ecx, [ecx+4]; jmp [eax+0x20]`) must pass the
      stack args the target pops; Hex-Rays attaches an early-pushed arg to the wrong call (0x45B570, 0x42DF80). The pop
      is exact from the ret N of the slot's implementations in the class of the member at +4: needs member pointer
      types (Types backlog). Meanwhile diff tier 3 catches it (0x42DF80 was its first find)
- [ ] Gate gap: argument count of a virtual call. 0x42DA20 and 0x42FF80 passed one stack argument too many / too few
      to a vtable slot (stack 4 bytes off -> eip on the stack, load hang); the gate checks bound functions' ABI, not
      slots. The original's pushes between calls are exact; which call pops them is decided by the slot
      implementations' ret N in the receiver's class (needs member / receiver types), or by diff tier 3 v3
- [ ] 8 done functions whose callers seem to read an ecx the callee *produced* (0x697330, 0x6973C0, 0x6AF8A0,
      0x71DF00, 0x722A40, 0x4F50D0, 0x542850, 0x6D2BE0; regargs.callers_keep_ecx with writes_ecx): check each,
      extend HOOK_KEEP_ECX to an "ecx out" variant if real
- [ ] Use the user's saves 2..24 (2026-10-09) in `--full` (`CSF_VERIFY_SAVES`) once each is known to pass alone

### Cutover
- [ ] Allocator shim (operator new/delete -> game heap; delete = sub_691110), rw.h shim
- [ ] Generated data for the standalone build: csf::gv<A> (346 globals: initial bytes from the exe) and
      csf::vt<A> (81 vtables: tables of csf::fn_slot pointers), from `linktest.py` (`work/linktest.json`)
- [ ] 83 files bind vtables with CSF_GLOBAL and take `&g_X` (right at runtime, resolves as data in the standalone
      build): rewrite to CSF_VTBL / CSF_ADDR_VT (the gate now rejects new ones)
- [ ] Rewrite the 14 naked asm thunks that embed original addresses (`linktest.py --list asm`)
- [ ] Library boundaries the standalone link needs: rw 33 (librw), crt 13, netz 34 (stub), rwa (see decision)
- [ ] NetZ boundary (320): name only if needed for stubbing — game net layer is stubbed at cutover

## Foundations (done)

Phases 0-3 of the original plan are complete: no symbols exist on any disc; the IDB, metadata/pseudocode/disasm
exports, region map, RW/CRT/RWA boundary names, STL instantiations, clone clusters, vtables + ctor hierarchy
(662 classes), modules (269, 187 named), globals (1136); the clang-cl hook DLL (dinput8 proxy, JMP/vtable/tramp
hooks, ini toggles, crash handler, dev starts, background/muted smokes), differential test harness, x64dbg
triage, negative controls (`controls.py`), verification ledger, waivers, full call transcripts. Phase 4 built
the grunt driver, tiers/ladders and quota guards. On 2026-10-08 the workflow was reorganized into the loop above
(JOURNAL): address-keyed cutover contract + link test, canonical bindings + ABI check, produce/verify split,
typed virtual calls, field consensus, fidelity rule, Opus review sampling, `wave`.

## Decisions made

- 2026-10-07: **Single-player first; multiplayer stubbed.** NetZ boundary is not reimplemented; game net code
  (net DDL classes etc.) gets stubbed at cutover. Revisit LAN after SP cutover.
- 2026-10-07: xwin x86 CRT/SDK installed at `~/.xwin` (license accepted by user). x64dbg + x64dbg-automate on
  Windows under E:\dev\tools (user OK), for crash triage and targeted runtime RE.
- Play copy of the game for hook testing: `/mnt/e/dev/csf-play` (E:\dev\csf-play). Never deploy into csf-builds.
  Smoke pool copies `/mnt/e/dev/csf-play-p1..p7` (verifier/bisect only); `/mnt/e/dev/csf-play-user` is the unhooked
  original for the user's own play and savegames.
- 2026-10-08: Grunts: Haiku 5.5 / Opus 5.5 on the grunts' own Claude account (never the main account) + Codex;
  OpenCode Go and DeepSeek dropped (allowance / quality). Ladders chosen from the run logs (`grunt.py cost`).
- 2026-10-08 (user): **progress is bytes per module**, waves per module, types first.
- 2026-10-08 (user): **production never waits on the game**: lanes commit gated functions as pending; the verifier
  checks them in game asynchronously; every deploy keeps pending hooks off.
- 2026-10-08: **the standalone link is keyed by original address** (src/globals.h), never by names; one canonical
  name + signature per address (db/api.json) for readability.
- 2026-10-08 (user): **use IDA fully and treat it as solid**: what IDA computes from the binary (prototypes,
  call-site arguments, stack deltas, xrefs) is the source of truth, refined only by exact binary facts (`ret N`,
  callers' cleanup, an entry-block register read); our knowledge goes back into the IDB and Hex-Rays re-derives.
- 2026-10-08: **fidelity is behavioral**: natural expressions, no emulation of x87 rounding or register/stack
  artifacts; the in-game diff tolerates last-bit float differences where the original stores floats.

## Open decisions (for the user)

- (not blocking until cutover) **RenderWare Audio replacement.** librw covers RW Graphics only. Options: reimplement
  the 40-function RWA boundary on our own backend (e.g. miniaudio/OpenAL + libvorbis), or reimplement RWA itself
  (92 KB). Recommendation: reimplement the boundary — the game only uses 40 calls.
