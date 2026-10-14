# CSF-RE plan

Functional (not byte-matching) C++ reimplementation of *Commandos: Strike Force* PC (`CommXPC.exe`),
built hook-first: reimplemented functions are swapped into the running original game until nothing original
remains, then the same source builds a standalone exe (cutover).

This file is the living plan: **the loop** (how work is done), **current focus**, the **backlog** (what is left,
by area) and decisions. Numbers are generated, not written here: `tools/status.py --modules` (bytes done per
module, evidence, link test), `tools/grunt.py wave --next` (what to wave next), `tools/fields.py coverage` (types),
`tools/verify.py report` (evidence), `tools/review.py report` (quality). Findings go to `docs/JOURNAL.md`.

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
- Game builds (read-only originals): `/mnt/e/dev/csf-builds/`; never deploy into them. Play copy for hook testing:
  `/mnt/e/dev/csf-play`; smoke pool copies `csf-play-p1..p7` (verifier/bisect only); `csf-play-user` is the unhooked
  original for the user's own play and savegames.
- Python deps in `.venv` beyond idalib: pefile, capstone, pyyaml, unicorn (tools/emudiff.py).
- Toolchain: clang-cl 21 + lld-link, `i686-pc-windows-msvc`, MSVC SDK via `xwin` (`~/.xwin`, license accepted by
  the user). x64dbg + x64dbg-automate under `E:\dev\tools` for crash triage and targeted runtime RE.
- Third-party sources/tools go in `work/tools/` (gitignored). Never leaked RenderWare SDK source; librw (MIT) only.
- Models: grunts on their own Claude account (`~/.config/csf-re/claude-grunts`, Haiku only, via `claude -p`) and
  Codex; the captain (naming) on the main account. Quota guards and ladders: `db/lanes.yaml`.

## The loop (per module)

One command runs it: **`tools/grunt.py wave <module>`** (`wave --next` ranks modules by centrality: undone
bytes x how much other code calls into them; `wave --waves -1` runs them in rank order as one rolling run: up to
`--overlap` 3 modules open, the next one starts while the earlier ones' L/X tails finish, and a closed module's
verify + link test + report run in the background). Each step also runs alone.

1. **Prep** (automatic, ~1 min): `ida/sync.py` pushes what db/ holds into the IDB and the module is re-decompiled.
2. **Produce** (`grunt.py lanes --module M`): leaf-first selection, clone-aware; size tiers S/M/L/X with model
   ladders, quota guards and stand-ins, all in **`db/lanes.yaml`**. Packet = pseudocode + disasm + class fields +
   typed virtual calls + the canonical `bind as` line per callee. `normalize()` auto-fixes; the **gate** checks
   includes, compile, references vs the original (semcheck), `ret N`, ABI of every binding, no raw addresses; Codex
   reviews gate-passed Claude answers. Batches are linked and committed **pending**.
3. **Install** (`verifier.py`, the wave's verify step; also runs alone next to lanes): its own detached tree, the
   installed baseline smoked alone first, then the module's pending functions enabled on top; crash report ->
   culprit (else bisect), diff mismatches -> quarantine (`queue/`). Every deploy keeps pending hooks off.
4. **Consolidate**: `api.py build` (per commit), `linktest.py`; stop rule = functions left pending (the quarantine-rate rule was dropped, user 2026-10-13), or the full regression smoke (every 3 modules) failing twice in a row without a culprit.

### Words (one meaning each)

A function's status is only **todo** or **done** (`db/functions.csv`); the rest are labels on top.
- Production says: **OK** (passed the gate, committed done + pending), **deferred** (no verdict, quota ran out;
  retried by itself), **escalated** (every rung of the model ladder failed the gate; stays todo, goes to `queue/`).
  Moving to the stronger model inside the ladder is the **next rung**, not an escalation.
- The game says: **pending** (done, hook never on in a passing smoke; off in every deploy), **installed** (hook was
  on in a passing smoke; older logs say "verified"), **quarantined** (blamed for a crash or a diff mismatch: file
  to `queue/`, status back to todo).
- Evidence of an installed function: **never ran** < **executed** < **diffed** (below).
- **`queue/`** is the orchestrator's inbox. Ways in: escalated, quarantined, **review-rejected** (`review.py`; the
  file stays in src/auto), **parked** (by hand, keeps the lanes off it). Lanes skip a function while its file is there.
- **Waived** (`db/waivers.yaml`): one gate check switched off for one function, with the reason.

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
- **`tools/emudiff.py`**: original bytes vs our object in an emulator, no game needed. The gate's last step since
  2026-10-13 (`db/lanes.yaml emudiff_gate`): a DIFF rejects the answer and the ladder retries with what differed.

The orchestrator (Opus, interactive) works `queue/` and the backlog below.

## Current focus (priority order)

Stage since 2026-10-13: the waves are exhausted at 91.9% of game code. What is left is done by **agent jobs**
(`tools/deep_worker.py`: sol-med for most jobs, sol-hi for hard ones, haiku-hi (or glm-flash for mechanical rewrites) for small ones;
user 2026-10-13); the orchestrator writes the
tasks, reviews reports and patches, decides. Everything is visible on `tools/webmon.py` (user, 2026-10-13).

State on 2026-10-14, night: the standalone exe (`/mnt/e/dev/csf-standalone`, `tools/dryrun.py --full`) runs its
main loop: the four logo Bink movies and the full intro cinematic play on screen (screenshots taken), no trap or crash
in 160 s. Next: a playtest (does a key/click skip the intro and reach the main menu? SendKeys Escape did not), then
the menu itself. Game functions missing from the standalone: 25. Static reach from WinMain: docs/WINMAIN_REACH.txt
(direct calls) and docs/WINMAIN_REACH_VTABLES.txt (`--vtables`, an upper bound). Jobs' output enters the tree only after review.

1. **Run the standalone to its first presented frame**, stop by stop: RtFS and DDS (job rwfs), the particle
   callbacks left (rw8b), the 32 undecided game functions after emudiff can decide them (emufix4). Each stop:
   dryrun --full, `tools/dbg.sh crash 60 <exe> full` on a crash.
2. **RenderWare on librw** beyond start-up: docs/RW_PORT.md slices, 188 slots unresolved.
3. **Installed functions that differ in emudiff**: emufix3 removed most false-alarm causes; what still differs goes
   to grouped sol-med jobs (`work/measure/groupwaves.py`, at most 5-6 sol jobs at once, user 2026-10-14).
4. **Escalation queue** and the last game code: 30 STL instances, the larger queue functions.
5. **NetZ** (165 unresolved), audio backend beyond null/PCM, D3DX9 and PNG slots (trap stubs today).
6. **Names and types**: `tools/naming.py`, `propagate.sh`.

Earlier decisions that still hold: no playtest milestone, evidence comes from the verifier's smokes, the in-game
diff and emudiff (user, 2026-10-11); emudiff is a gate step (2026-10-13).

## Backlog

### Loop & tooling
- [ ] IDB: apply the corrected prototypes (db/regargs.json spurious regs dropped) in a sync.py stage + propagate
- [ ] Float returns Hex-Rays got wrong (`tools/fpret.py`: 0x40FC50 typed `long double`, returns nothing): push
  `void` for the functions in db/fpret.json that differ, in the same sync.py stage, so packets stop showing it.
- [ ] Tier 3: heap objects reached through a global pointer are not watched for callee writes (only pointer
  arguments are): 0x6981B0 mismatched 1/1 on a correct file and sits in difftable's NO_DIFF. Watch the objects
  the function's pointer globals point to (first 256 bytes) and conclude nothing when a callee changed them.
- [ ] Gate: a virtual/indirect call given an argument the original pushes early for the NEXT call
  (0x6B2A10, 0x6E5580: `push 5; call [eax+0x1c]; mov ecx, eax; call 0x6C4870`) passes every static check and
  unbalances the stack in game. Count pushes consumed per call from the callee cleanup (`ret N`) where known.
- [ ] After the x87 leak fix (2026-10-12): smoke the queue's float/NaN mismatches and crash bisects again
  (each has a note). The verifier now quarantines nobody on a smoke with an `X87 ...` line (hook/diff.cpp: four
  or more x87 entries live at a diffed function's entry): baseline stops, a group stays pending. First smoke
  after the fix: check that no such line appears; if one does, bisect for the leaking function by hand.
- [ ] Rename guard in `tools/naming.py`: a wrapper whose target got implemented takes its real prototype; an
  int<->float flip of a parameter compiles silently in older callers (value conversion changes the bits). Diff
  old/new wrapper params before the rewrite and revert/flag on a flip with a non-literal argument (first one:
  0x5266C0 +0x28, nullptr->0.0f)
- [ ] Tail-forwarding thunks (sets state, then `jmp [obj->vtbl+N]` with the caller's stack args untouched, count
  unknown): C++ can't forward them, grunts hand-write naked asm and name a C++ function in it (link error). 4/160
  escalated in audio_sound_manager (0x521D60, 0x523760, 0x527510, 0x521E80). Give hook.h a helper (like
  HOOK_KEEP_ECX) + packet NOTE, then re-grunt them
- [ ] Usercall entry stub: 0x465310 and 0x52F8A0 take real register inputs (both in `queue/`); hook.h needs an
  entry stub like HOOK_KEEP_ECX
- [ ] 57 older src/auto files still use `volatile` (dead loads, uninitialized args passed on, forced global loads):
  rewrite at their next touch (gate now rejects them on re-grunt)
- [ ] Run `tools/naming.py` at the next milestone (first full run with the captain steps; then propagate.sh);
  re-test Sonnet for the captain's bulk function renames in that run
- [ ] Opus pass over the 44 held-back RW names (`tools/setup/rwname.py report`): neighbour pairs given the same name
      (Set/Get, Width/Height, Int32/Int16) and low-confidence toolkit internals (RpTeam, Rt2d, RtWorldImport)
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
- [ ] emudiff: switch cases. 0x4D9170 (40-case command dispatcher) had a wrong `this` in one case and showed
      `same`: no profile got past the entry test, let alone into each case. For a function with a jump table
      (export/disasm `switches`), add runs that force each table index at the `jmp [reg*4+table]` (set the register
      on both sides at that instruction / at ours' matching compare) and count cases reached in the verdict.
- [ ] Coverage from real play: the T* AI tasks and GUI input handlers only run there. Savegames taken in combat
      (user playtest) and the user's saves 2..24 (2026-10-09), each checked alone first, go into
      `CSF_VERIFY_SAVES` for `verifier.py --full`
- [ ] Gate gap: what an indirect or virtual call pops. Through a forwarder (0x4F5170 `mov ecx, [ecx+4];
      jmp [eax+0x20]`) Hex-Rays attaches an early-pushed arg to the wrong call (0x45B570, 0x42DF80); 0x42DA20 and
      0x42FF80 passed one stack argument too many / too few to a vtable slot (stack 4 bytes off -> eip on the stack,
      load hang). The gate checks bound functions' ABI, not slots. The original's pushes between calls are exact;
      which call pops them follows from the ret N of the slot's implementations in the receiver's class: needs
      member / receiver types (Types backlog). emudiff has the same blind spot (0x533D90)
- [ ] 8 done functions whose callers seem to read an ecx the callee *produced* (0x697330, 0x6973C0, 0x6AF8A0,
      0x71DF00, 0x722A40, 0x4F50D0, 0x542850, 0x6D2BE0; regargs.callers_keep_ecx with writes_ecx): check each,
      extend HOOK_KEEP_ECX to an "ecx out" variant if real
- [x] emudiff gate step (2026-10-13): `emudiff_gate: on` in db/lanes.yaml; 0.13% false alarms on the diffed tier
- [ ] **Sweep the done functions with emudiff** (`emudiff.py --tier executed`, `--tier gate`: about 6-10% DIFF):
      read a sample of 30 for the real-bug share, fix the remaining false-alarm classes, then send the flagged
      ones back to the lanes (status todo) so the ladder redoes them under the emudiff gate
- [ ] emudiff: an `emulated` evidence level in the ledger (verify.py tiers / status.py) for functions that pass
- [ ] **emudiff triage**: fix 0x544C60 (passes src+0x1C to the string assign; the original passes src, then adds)
      and 0x42DA20 (binds 0x6C8620 as int, the original tests `al`); read the other ~45 DIFFs of the 600-function
      sample (`emudiff.py --tier gate --n 300`, `--tier executed --n 300`), 0x728160, 0x53D8B0 (harness suspect)
- [ ] **emudiff to gate grade**, in this order (each is a measured false-alarm or blind spot):
      eax of functions whose callers ignore it (difftable knows them); ecx only for callees that read it
      (regargs); what a virtual call pops (the gate gap above); a frame dword passed on before it is written
      (0x4696C0, 0x47EE50: mask the argument as tier 3 does); mocked callees that write nothing, so a value
      read before a call equals the one reloaded after it (0x6AB9E0, missed: mocks should dirty what their
      pointer arguments point to); inconclusive runs (10% of small, half of large
      functions: int-typed values that are pointers; type inputs from class layouts). Then: an `emulated` evidence
      tier in the ledger, a gate step for new functions, and only after that retire parts of semcheck
- [ ] Remove diff tier 3 (record/replay) once emudiff's false alarms are fixed; tiers 1/2 stay. Footprint:
      hook/replay.cpp, the tier 3 half of difftable.py, kT3 / g_no_t3 in hook/diff.cpp, namespace t3 in hook.h,
      replay_table.inc in CMakeLists.txt

### Cutover
- [ ] Allocator shim: written (`src/boundary/alloc/`, 2026-10-13), not run; open: heap initialisation and alignment, malformed callers (`src/boundary/REPORT.md`); rw.h shim
- [ ] (generator in place 2026-10-13: `tools/datagen.py`, `docs/DATAGEN.md`; open: see current focus) Generated data for the standalone build: csf::gv<A> (globals: initial bytes from the exe) and csf::vt<A>
      (vtables: tables of csf::fn_slot pointers), from `linktest.py` (`work/linktest.json`; counts in `status.py`)
- [x] (2026-10-13: 70 of 72 rewritten by jobs; 0x50C370 and 0x532DC0 wait on an emudiff DIFF) 83 files bind vtables with CSF_GLOBAL and take `&g_X` (right at runtime, resolves as data in the standalone
      build): rewrite to CSF_VTBL / CSF_ADDR_VT (the gate now rejects new ones)
- [x] (2026-10-13: 14 of 15; 0x53D8B0 in job glmraw) Rewrite the naked asm thunks that embed original addresses (`linktest.py --list asm`)
- [ ] Library boundaries the standalone link needs: rw (librw), crt, netz (stub), rwa (reimplement the 40-function boundary on our own audio backend, user 2026-10-13); what is
      unresolved per library is in the `status.py` link-test line
- [ ] NetZ boundary (320): name only if needed for stubbing — game net layer is stubbed at cutover

## Foundations (done)

No symbols exist on any disc. In place: the IDB and its exports, region map, RW/CRT/RWA boundary names, STL
instantiations, clone clusters, vtables + ctor hierarchy, modules, globals; the clang-cl hook DLL (dinput8 proxy,
JMP/vtable/tramp hooks, ini toggles, crash handler, dev starts, background/muted smokes), the in-game diff,
negative controls (`controls.py`), verification ledger, waivers; the grunt driver with tiers, ladders and quota
guards. One-shot scripts of that phase: `tools/setup/`. History: JOURNAL.

## Decisions made

- 2026-10-07: **Single-player first; multiplayer stubbed.** NetZ boundary is not reimplemented; game net code
  (net DDL classes etc.) gets stubbed at cutover. Revisit LAN after SP cutover.
- 2026-10-08: Grunts: Haiku 5.5 / Opus 5.5 on the grunts' own Claude account (never the main account) + Codex
  (Sonnet as the Codex stand-in); OpenCode Go and DeepSeek dropped (allowance / quality). Ladders chosen from the
  run logs (`grunt.py cost`), kept in `db/lanes.yaml`.
- 2026-10-08 (user): **progress is bytes per module**, waves per module.
- 2026-10-08 (user): **production never waits on the game**: lanes commit gated functions as pending; the verifier
  checks them in game asynchronously; every deploy keeps pending hooks off.
- 2026-10-08: **the standalone link is keyed by original address** (src/globals.h), never by names; one canonical
  name + signature per address (db/api.json) for readability.
- 2026-10-08 (user): **use IDA fully and treat it as solid**: what IDA computes from the binary (prototypes,
  call-site arguments, stack deltas, xrefs) is the source of truth, refined only by exact binary facts (`ret N`,
  callers' cleanup, an entry-block register read); our knowledge goes back into the IDB and Hex-Rays re-derives.
- 2026-10-08: **fidelity is behavioral**: natural expressions, no emulation of x87 rounding or register/stack
  artifacts; the in-game diff tolerates last-bit float differences where the original stores floats.
- 2026-10-12 (user): **the grunts' Claude account is for Haiku rungs only.** No Opus rung, no Sonnet stand-in;
  M/L/X are Codex `gpt-6.1-sol@high` and end in `queue/`; Haiku out of quota -> `gpt-6.1-sol@low`.
- 2026-10-10 (user): **simpler loop**: names never change inside a wave (tools/naming.py at milestones, code-hash
  invariant); types come first as structure from the binary (propagate.sh), not as names. Review is one rolling
  sample, not a wave stage. Model policy is db/lanes.yaml. One word per evidence level.
- 2026-10-10 (user): **emudiff instead of diff tier 3 v3.** Tier 3 (record/replay in game) cost hook/replay.cpp +
  half of difftable.py and a row of harness crashes for ~90 functions; tools/emudiff.py makes the same comparison
  offline. Tiers 1/2 stay (the only ground truth from the real game).

## Open decisions (for the user)

None open. Decided 2026-10-13 (user): the quarantine-rate stop rule is dropped; RenderWare Audio is replaced by
reimplementing its 40-function boundary on our own backend (e.g. miniaudio/OpenAL + libvorbis), not RWA itself.

