# CSF-RE plan

Functional (not byte-matching) C++ reimplementation of *Commandos: Strike Force* PC (`CommXPC.exe`),
built hook-first: reimplemented functions are swapped into the running original game in batches until
nothing original remains, then the build flips to a standalone exe.

This file is the living plan. Update the **Phase checklist** as work completes, and log discoveries in
`docs/JOURNAL.md`. Open decisions for the user live at the bottom.

## Facts about the target (see JOURNAL for evidence)

- `CommXPC.exe`: MSVC 7.1 (VS2003), built 2006-02-23, imagebase 0x400000, not packed. d3d9, dinput8, dsound, winsock.
- **No RTTI** (`/GR-`), **no symbols** on any disc (PS2 proto/retail, Xbox). Names come from strings/asserts/structure.
- Developer: Pyro Studios. Identifiers and strings are **Spanish**.
- Original source root: `c:\CommX\Src\Main\` (from Xbox XBE). PDB was `CommXPC.pdb`.
- Statically linked libraries (never reversed, only their boundary API is named):
  RenderWare 3.x (core + D3D9 driver + toolkits), D3DX9 (shader compiler 5.04.00.3900), Quazal NetZ,
  MSVC 7.1 CRT/STL, libpng 1.0.12, zlib. Dynamically: binkw32, vorbisfile.
- `RwEngineInstance` = `0x8b68c0`.
- Region map: `db/regions.yaml`.

## Environment

- Everything runs in WSL. IDA 9.3 Linux at `~/ida-pro-9.3`, driven headless via idalib from `.venv`.
- IDB: `ida/CommXPC.exe.i64` (gitignored, derived). Exe copy: `work/pc/CommXPC.exe`.
- Game builds (read-only originals): `/mnt/e/dev/csf-builds/` (pc-retail, ps2-prototype, ps2-retail, xbox-retail).
  Extracted disc contents: `work/{ps2proto,ps2retail,xbox}`.
- Toolchain: clang-cl 21 + lld-link, `i686-pc-windows-msvc`, MSVC SDK via `xwin` (~/.local/bin/xwin).
- Third-party sources/tools pulled from the internet go in `work/tools/` (gitignored). Do **not** use leaked
  RenderWare SDK source; use librw (MIT) for reference and for cutover.

## Data model

- **Git (knowledge):** `db/regions.yaml`, `db/functions.csv` (ea, size, region, module, name, name_src, status),
  `db/globals.yaml`, `db/modules.yaml`, `db/types/*.h`, `src/`, `notes/`.
- **Derived (gitignored):** `ida/`, `export/` (regenerated from the IDB by `tools/ida/export_*.py`).
- **Runtime state (not in git):** claims/attempts in `work/state.sqlite` once the grunt driver exists.
- The repo is the source of truth; the IDB is derived. Only `tools/ida/sync.py` writes to the IDB, in batches.

## Workflow (revised)

- Grunts are **single LLM calls**, not agents: a Python driver builds a packet (pseudocode with current names,
  callee signatures, referenced types, module note, strings), calls the model, runs the gate, commits serially.
  No worktrees, no claim expiry. One retry with the gate error, then escalate to `queue/` for Opus.
- One task = name + type + reimplement a function (or a batch of tiny ones). Naming-only tasks exist only
  for library-boundary identification.
- Gate: compiles; `static_assert` layouts pass; calling convention / stack purge matches the original (`ret N`).
- Hooks are registered in source next to the function (`HOOK(0xADDR, Fn)`); runtime-togglable via an ini so
  bisecting a batch needs a restart, not a rebuild.
- Behavioral checks before playtests: in-DLL differential tests for pure functions; record/replay of args.
- Allocator shim: in the hook phase, `new/delete/malloc/free` route to the game's CRT heap.
- RW calls go through an `rw.h` shim: game addresses in hook phase, librw at cutover.
- Periodic **link test**: build the standalone exe with the cutover flag; unresolved symbols = true remaining work.
- Ordering: leaf-first over SCCs of the call graph; virtual calls count as satisfied once the class is typed.

## Phase checklist

### Phase 0 — symbol harvest  ✅ done (no symbols exist; see JOURNAL 2026-10-07)

### Phase 1 — triage
- [x] IDB created with full auto-analysis (22,915 functions, Hex-Rays OK)
- [x] Metadata export (`export/functions.jsonl`, `export/strings.json`)
- [x] RTTI scan (none for game classes)
- [x] Draft region map (`db/regions.yaml`)
- [x] `db/functions.csv` generated (`tools/triage.py`)
- [x] Refine regions 0x646000–0x7ae440 and 0x7c4000–0x82c000 (NetZ vs game vs RW toolkits)
- [ ] Identify STL template instantiations inside game regions (mark `lib: stl`)
- [x] Bulk Hex-Rays export (`export/funcs/<ea>.c`)
- [ ] Name the RW boundary API (455 functions, `export/boundary_api.json`) using librw/RW API as reference
- [ ] Name the CRT boundary API (105). NetZ boundary: name only (for stubbing), no reimpl
- [ ] Harvest Spanish strings/state names/config keys → subsystem anchors
- [x] Clone clusters (`tools/clones.py` → export/clones.json): one impl per cluster
- [x] Vtable recovery + QI-chain hierarchy (`tools/vtables.py` → db/vtables.json)
- [ ] Ctor-based hierarchy (base vtable stored before derived) + class size from `operator new` call sites
- [ ] Draft `db/modules.yaml` (call-graph + address clustering, ~one cluster per original .cpp)
- [ ] Find globals: data refs → `db/globals.yaml` draft
- [ ] Locate Xbox XBE code, diff vs PC for structure confirmation (optional)

### Phase 2 — types and module carving (Opus)
- [ ] Core classes: layouts, vtables, sizeof with static_asserts
- [ ] STL layout replicas for VC7.1 (std::string/vector/list/map) if embedded in game classes
- [ ] Module boundaries finalized; first `notes/`

### Phase 3 — hook DLL bring-up
- [x] clang-cl + xwin toolchain builds `dinput8.dll` proxy (`cmake/clang-cl-x86.cmake`, `tools/deploy.sh`)
- [x] Smoke test: game loads the DLL and runs (`tools/smoke.sh`; per-hook hit counts in csf_hook_hits.txt)
- [x] Hook installer (jmp + vtable patch), ini toggles, logging (`hook/`)
- [x] globals.h (address-bound `CSF_GLOBAL`), `legacy::string` (VC7.1 layout, 28 bytes)
- [ ] Allocator shim (operator new/delete → game heap; delete = sub_691110), rw.h shim, globals.yaml → globals.h generator
- [x] First hand-written hooked functions verified in-game (sub_697300 79 hits, sub_697450 1 hit)

### Phase 4 — grunt waves
- [ ] Driver (packet → model → gate → commit), escalation queue
- [ ] Differential test harness
- [ ] Waves per module, leaf-first

### Phase 5 — batch verification (user playtests) and Phase 6 — cutover

## Decisions made

- 2026-10-07: **Single-player first; multiplayer stubbed.** NetZ boundary is not reimplemented; game net code
  (net DDL classes etc.) gets stubbed at cutover. Revisit LAN after SP cutover.
- 2026-10-07: Grunt provider = **DeepSeek API** (key in `~/.config/csf-re/env`, never in git).
- 2026-10-07: xwin x86 CRT/SDK installed at `~/.xwin` (license accepted by user).
- Play copy of the game for hook testing: `/mnt/e/dev/csf-play` (E:\dev\csf-play). Never deploy into csf-builds.

## Open decisions (for the user)

(none)
