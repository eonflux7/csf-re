# CSF-RE — Commandos: Strike Force PC reimplementation

Functional C++ reimplementation of `CommXPC.exe` (MSVC 7.1, 2006), hook-first. Full plan: `docs/PLAN.md`.
Findings log: `docs/JOURNAL.md`. You (Opus) are the orchestrator/architect.

## When the user says "status"
Run `.venv/bin/python tools/status.py` (add `--modules` for per-module game progress), then summarize:
current phase, what was done last (git log + latest JOURNAL entry), next 3–5 planned tasks from PLAN.md,
and any open decisions waiting on the user. Keep it short.

## When the user says "continue"
1. Run `tools/status.py`; read `docs/PLAN.md` (checklist + open decisions) and the tail of `docs/JOURNAL.md`.
2. Take the first unchecked item of the current phase (or a better one if findings justify it — the plan is
   allowed to change; update PLAN.md when it does).
3. Work in steps; give the user a brief one-line update before each major step.
4. After each completed item: tick it in PLAN.md, add a short dated JOURNAL entry for anything learned,
   commit (`git add -A && git commit`). Commit messages end with the Co-Authored-By trailer.
5. Stop and ask when: sudo is needed, an open decision blocks progress, or a playtest is needed.

## Layout
- `tools/ida/` idalib scripts (`common.py` opens the IDB). Run with `.venv/bin/python`. IDB: `ida/CommXPC.exe.i64`.
  Only scripts in `tools/ida/` touch the IDB; writes only via `sync.py` (batch).
- `tools/` triage.py (functions.csv), regions.py (region exploration), rtti.py, status.py.
- `db/` source-of-truth knowledge: regions.yaml, functions.csv, (globals.yaml, modules.yaml, types/).
- `export/` derived from IDB (gitignored); regenerate with `tools/ida/export_meta.py`.
- `work/` gitignored scratch: exe copy, extracted discs, third-party tools (`work/tools/`).
- Original game builds (read-only): `/mnt/e/dev/csf-builds/`.

## Rules
- Repo is source of truth; IDB is derived. Never hand-edit `export/`.
- Library regions (rw, d3dx9, netz, crt, png) are never reimplemented — only their boundary API is named.
- Don't use leaked RenderWare SDK source. librw (MIT) is the reference and the cutover target.
- No sudo: if a system package is needed, stop and ask the user.
- Large outputs (IDA dumps, pseudocode) go to files, not the chat.
