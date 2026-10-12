# CSF-RE — Commandos: Strike Force PC reimplementation

Functional C++ reimplementation of `CommXPC.exe` (MSVC 7.1, 2006), hook-first. Full plan: `docs/PLAN.md`.
Findings log: `docs/JOURNAL.md`. You (Opus) are the orchestrator/architect.

## When the user says "status"
Run `.venv/bin/python tools/status.py` (add `--modules` for per-module game progress), then summarize:
current focus, what was done last (git log + latest JOURNAL entry), next 3–5 items (current focus, then
backlog), pending/verified counts and the link-test line, and any open decisions waiting on the user. Keep it short.

## When the user says "continue"
1. Run `tools/status.py`; read `docs/PLAN.md` (the loop, current focus, backlog, open decisions) and the tail
   of `docs/JOURNAL.md`.
2. Take the first current-focus item (or a better one if findings justify it — the plan is allowed to change;
   update PLAN.md when it does). Module work is one command: `tools/grunt.py wave <module>` (`wave --next`
   ranks modules); waves never rename (names: `tools/naming.py` between wave runs). Production never waits on the game: `tools/verifier.py` verifies pending functions apart.
3. Work in steps; give the user a brief one-line update before each major step.
4. After each completed item: tick it in the PLAN.md backlog (or update current focus), add a short dated JOURNAL entry for anything learned,
   commit (`git add -A && git commit`). Commit messages end with the Co-Authored-By trailer.
5. Stop and ask when: sudo is needed, an open decision blocks progress, or a playtest is needed.

## Layout
- `tools/ida/` idalib scripts (`common.py` opens the IDB). Run with `.venv/bin/python`. IDB: `ida/CommXPC.exe.i64`.
  Only scripts in `tools/ida/` touch the IDB; writes only via `sync.py` (batch). IDA is the source of truth for what
  it computes from the binary: `export_abi.py` (Hex-Rays prototypes, call-site args, caller cleanup) feeds
  `api.py`; `propagate.sh` is the feedback loop (push facts/types -> re-decompile -> this_class.py derives more);
  `export_pseudo.py` writes the typed pseudocode packets show. Precedence: exact binary facts > Hex-Rays > heuristics.
- `tools/` the loop: grunt.py (lanes, wave; model policy in `db/lanes.yaml`), verifier.py + verify.py (in-game
  install/diff, evidence ledger), gate.py + semcheck.py + controls.py (static checks; fpret.py: float returns decided from the binary), emudiff.py (emulated diff
  vs the original, offline), api.py + addrfix.py + linktest.py (cutover contract), pool.py + playcopy.py (parallel
  smoke instances), status.py, monitor.py. Between wave runs: naming.py (the only place names change: captain.py +
  fields.py + rename_classes.py under a code-hash invariant), ida/propagate.sh, review.py (rolling Opus sample);
  this_class.py + plain_classes.py + vtbl_types.py (classes, `this`/parameter/member types, typed vtables).
  `tools/setup/`: one-shot scripts of the setup phase (regions, rwname, stl, clones, ...).
- `src/globals.h`: everything original is referenced by address (CSF_FUNC/GLOBAL/STR/VTBL/ADDR_*); the standalone
  build resolves by address, so never write an original address as a bare number.
- `db/` source-of-truth knowledge: regions.yaml, functions.csv, globals.yaml, modules.yaml, types/, lanes.yaml.
- `queue/` escalations for the orchestrator (one file per function: gate, smoke or review failure).
- `export/` derived from IDB (gitignored); regenerate with `tools/ida/export_meta.py`.
- `work/` gitignored scratch: exe copy, extracted discs, third-party tools (`work/tools/`).
- Original game builds (read-only): `/mnt/e/dev/csf-builds/`.

## Rules
- Repo is source of truth; IDB is derived. Never hand-edit `export/`.
- Library regions (rw, d3dx9, netz, crt, png) are never reimplemented — only their boundary API is named.
- Don't use leaked RenderWare SDK source. librw (MIT) is the reference and the cutover target.
- No sudo: if a system package is needed, stop and ask the user.
- Large outputs (IDA dumps, pseudocode) go to files, not the chat.
