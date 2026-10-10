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
- Python deps in `.venv` beyond idalib: pefile, capstone, pyyaml, unicorn (tools/emudiff.py).
- Toolchain: clang-cl 21 + lld-link, `i686-pc-windows-msvc`, MSVC SDK via `xwin`. x64dbg under `E:\dev\tools`.
- Third-party sources/tools go in `work/tools/` (gitignored). Never leaked RenderWare SDK source; librw (MIT) only.
- Models: grunts on their own Claude account (`~/.config/csf-re/claude-grunts`, Haiku/Opus via `claude -p`) and
  Codex; the captain (naming) on the main account. Quota guards and ladders: `db/lanes.yaml`.

## The loop (per module)

One command runs it: **`tools/grunt.py wave <module>`** (`wave --next` ranks modules by centrality: undone
bytes x how much other code calls into them; `wave --waves -1` runs them in rank order). Each step also runs alone.

1. **Prep** (automatic, ~1 min): `ida/sync.py` pushes what db/ holds into the IDB and the module is re-decompiled.
2. **Produce** (`grunt.py lanes --module M`): leaf-first selection, clone-aware; size tiers S/M/L/X with model
   ladders, quota guards and stand-ins, all in **`db/lanes.yaml`**. Packet = pseudocode + disasm + class fields +
   typed virtual calls + the canonical `bind as` line per callee. `normalize()` auto-fixes; the **gate** checks
   includes, compile, references vs the original (semcheck), `ret N`, ABI of every binding, no raw addresses; Codex
   reviews gate-passed Claude answers. Batches are linked and committed **pending**.
3. **Install** (`verifier.py`, async; lanes start a canary group per 300 commits): its own detached tree, the
   installed baseline smoked alone first, then the module's pending functions enabled on top; crash report ->
   culprit (else bisect), diff mismatches -> quarantine (`queue/`). Every deploy keeps pending hooks off.
4. **Consolidate**: `api.py build` (per commit), `linktest.py`; stop rule = quarantine rate or functions left pending.

Evidence per done function, weakest to strongest, always reported by these words (`verify.py report`, status):
**never ran** (static checks only, even if installed in a passing smoke) < **executed** (ran in a smoke) <
**diffed** (results compared with the original's in game). "Installed" only means the hook was on in a passing smoke.

Between wave runs (milestones, e.g. while quota resets):
- **`tools/naming.py`**: the only place names change. Captain names classes, modules and placeholder functions;
  field-name consensus; `rename_classes.py` + `fields.py fixbuild` rewrite src. Invariant: a rename changes no
  code - every object's code hash is compared before/after; a build break or a changed hash puts everything back.
- **`tools/ida/propagate.sh`** (~45 min, no tokens): types through Hex-Rays (`this`, parameters, members), then
  `api.py build`, `gen_classes_h.py`.
- **`tools/review.py sample`**: one rolling Opus sample over all done code, weighted to functions that never ran.
- **`tools/emudiff.py`** (spike, see backlog): original bytes vs our object in an emulator, no game needed.

The orchestrator (Opus, interactive) works `queue/` and the backlog below.

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
6. **Evidence for the functions that never ran** (1917 of 3976, `status.py`): `emudiff.py` triage and hardening
   (backlog, Verification), `review.py sample` (weighted to them), `verifier.py --full` over everything once.

## Backlog

### Loop & tooling
- [ ] IDB: apply the corrected prototypes (db/regargs.json spurious regs dropped) in a sync.py stage + propagate
- [ ] Rename pass guard: a wrapper whose target got implemented takes its real prototype; an int<->float flip of a
  parameter compiles silently in older callers (value conversion changes the bits). Diff old/new wrapper params
  in captain_prep and revert/flag on a flip with a non-literal argument (first one: 0x5266C0 +0x28, nullptr->0.0f)
- [ ] Tail-forwarding thunks (sets state, then `jmp [obj->vtbl+N]` with the caller's stack args untouched, count
  unknown): C++ can't forward them, grunts hand-write naked asm and name a C++ function in it (link error). 4/160
  escalated in audio_sound_manager (0x521D60, 0x523760, 0x527510, 0x521E80). Give hook.h a helper (like
  HOOK_KEEP_ECX) + packet NOTE, then re-grunt them
- [ ] 57 older src/auto files still use `volatile` (dead loads, uninitialized args passed on, forced global loads):
  rewrite at their next touch (gate now rejects them on re-grunt)
- [ ] Re-test Sonnet for bulk function renames (captain functions) after waves name the callers
- [ ] Opus pass over the 44 held-back RW names (`tools/setup/rwname.py report`): neighbour pairs given the same name
      (Set/Get, Width/Height, Int32/Int16) and low-confidence toolkit internals (RpTeam, Rt2d, RtWorldImport)
- [ ] Locate Xbox XBE code, diff vs PC for structure confirmation (optional)


- [ ] Run `tools/naming.py` at the next milestone (first full run with the captain steps; then propagate.sh)
- [ ] The in-lane Codex review after the sharper prompt (2026-10-10): re-read 15 rejections; 10 of 14 were not
  behavior before (load order, INT_MIN, x87, phantom edi)

### Types & structure
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
- [ ] AI combat coverage: the T* AI tasks and GUI input handlers only run in real play. Needs savegames taken in
      combat (user playtest) -> `CSF_SWEEP_SAVES="1.sav 2.sav ..."`, then `verifier.py --full`
- [ ] Executed coverage: 1278 done functions have only passed static checks (`verify.py report`)
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

- [ ] **emudiff triage**: fix 0x544C60 (passes src+0x1C to the string assign; the original passes src, then adds)
      and 0x42DA20 (binds 0x6C8620 as int, the original tests `al`); read the other ~45 DIFFs of the 600-function
      sample (`emudiff.py --tier gate --n 300`, `--tier executed --n 300`), 0x728160, 0x53D8B0 (harness suspect)
- [ ] **emudiff to gate grade**, in this order (each is a measured false-alarm or blind spot of the spike):
      eax of functions whose callers ignore it (difftable knows them); ecx only for callees that read it
      (regargs); what a virtual call pops (IDA's guess is wrong at 0x533D90: needs receiver types);
      inconclusive runs (10% of small, half of large functions: int-typed values that are pointers; type inputs
      from class layouts). Then: an `emulated` evidence tier in the ledger, a gate step for new functions, and
      only after that retire parts of semcheck and skip diff tier 3 v3

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
- 2026-10-10 (user): **simpler loop**: names never change inside a wave (tools/naming.py at milestones, code-hash
  invariant); "types first" now means structure from the binary (propagate.sh), not names. Review is one rolling
  sample, not a wave stage. Model policy is db/lanes.yaml. One word per evidence level.
- 2026-10-08: **fidelity is behavioral**: natural expressions, no emulation of x87 rounding or register/stack
  artifacts; the in-game diff tolerates last-bit float differences where the original stores floats.

## Open decisions (for the user)

- (not blocking until cutover) **RenderWare Audio replacement.** librw covers RW Graphics only. Options: reimplement
  the 40-function RWA boundary on our own backend (e.g. miniaudio/OpenAL + libvorbis), or reimplement RWA itself
  (92 KB). Recommendation: reimplement the boundary — the game only uses 40 calls.
