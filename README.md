# CSF-RE: Commandos: Strike Force (PC) reimplementation

A functional C++ reimplementation of **Commandos: Strike Force** for Windows (`CommXPC.exe`, Pyro Studios /
Eidos, 2006), built **hook-first**. Reimplemented functions are swapped into the running original game one at a
time and checked against it. When nothing of the original's own code is left, the same sources link into a
standalone executable (the **cutover**). Library code the game linked statically (RenderWare, Quazal NetZ, D3DX,
the MSVC CRT) is never reimplemented. The game's calls into it are routed to open replacements, mainly
[librw](https://github.com/aap/librw) for RenderWare Graphics.

> **Status: work in progress, not playable.** The standalone exe runs the intro and the menus and creates
> profiles. The first mission loads its world and then stops. Read the state and the evidence ledger below
> before relying on any part of it.

The goal is behavioural fidelity, not a byte-matching decompilation. Code uses natural C++ expressions and does
not emulate x87 rounding or register and stack artifacts.

**No game files are included.** You need your own legally obtained copy of the PC game. See [Legal](#legal).

---

## Project state (2026-10-22, commit after `43233e7f`)

### The short version

| Area | State |
|---|---|
| Game code (the game's own functions) | **14,900 of 14,903 written** (100% of bytes; 3 todo). Volume of written code, **not** correctness |
| Verified against the original | **2,205 functions (14.8%) compared in game with matching results**; 5,536 more only executed; 7,150 never ran |
| Standalone link | Links. 14,927/14,927 sources compile standalone; 138 library slots still trap (NetZ 122, RW 15, PNG 1) |
| Standalone runtime (Windows, last run 2026-10-15) | Logos and intro movies, main menu, Settings pages, new profile, Campaign page, mission start |
| First mission (FR01) | Scripts, animations, clumps and BSP world load. Last Windows run **faulted in a sector collision query** |
| Collision fix (2026-10-17) | Implemented and tested natively: real FR01 collision world, 144 trees / 99,653 triangles, under ASan/UBSan. **Not yet re-run on Windows** |
| Still missing for a playable mission | World/sector rendering and visibility, player movement and collision, weapon use, clean exit from a mission |
| Multiplayer | Out of scope for now. The NetZ layer is stubbed offline (decision 2026-10-07) |

### What the game has been seen to do (standalone exe, Windows)

Recorded in [docs/JOURNAL.md](docs/JOURNAL.md), runs of 2026-10-14 and 2026-10-15:

- Start-up through the CRT initializers, NetZ, DirectInput and RenderWare engine start (D3D9 device through
  librw), presenting frames at 800×600 full screen.
- Eidos / Pyro / RenderWare / Nvidia / Bink logos and the intro cinematic. Escape skips movies.
- Main menu, Settings and its pages, Credits, message boxes, the Exit dialog and a clean exit from the menu.
- Profile creation with text entry, the Campaign page and the Game options page.
- Mission start for FR01 (`profile → Select → Play Mission → Normal`). The loading screen shows, the mission
  scripts parse, animations, clumps and the BSP world load, and the run then faults in the sector collision
  query. The collision plugin has since been fixed natively (above) but not re-run in the game.

All of these runs are scripted and unattended (`tools/dryrun.py --walk`): DirectInput events are injected and
screenshots are taken. As of the last run, 13 of the 14 named walk scripts passed; the mission script is the one
that stops.

### Current focus (from [docs/PLAN.md](docs/PLAN.md))

1. **Make one gameplay path complete**: FR01 load → visible world → movement/collision → weapon use → clean exit.
   Next: re-run the mission with the collision fix, then sector rendering and visibility.
2. **Make the checks dependable**: `tools/check_all.sh` now fails on any failed stage (failure-injection tested).
   A full real run of it after the 2026-10-17 changes is still outstanding.
3. **Turn each defect into evidence**: original-binary comparisons, real-asset fixtures, sanitizer runs, and an
   audit of related code for the same root cause.
4. Then the remaining integration (RW physics/animation/particle consumers, NetZ offline boundaries, audio beyond
   null/PCM), then the verification backlog (below).

---

## The evidence ledger

Every written game function sits at one evidence level. `db/verify.json` is the ledger. `tools/verify.py` writes
it after each in-game smoke run, keyed by the function's address and the hash of its compiled code. Evidence
recorded for one code hash does not carry over when the code changes: the function falls back to "stale" or
"never ran".

Levels, weakest to strongest:

| Level | Meaning |
|---|---|
| **never ran** | Passed the static checks only (compile, reference check vs the original, ABI, `ret N`; since 2026-10-13 also the emulated diff). May be installed, but never executed in a recorded run |
| **executed** | Ran inside the original game with the hook installed, and the smoke passed. Results not compared |
| **diffed** | Ran in the game and its results (return value, written memory, outgoing calls) were compared with the original's on the same inputs, with no mismatch |
| **mismatched** | A recorded in-game comparison differed from the original |
| **installed** | Orthogonal flag: the hook was on in at least one passing smoke |

### Ledger totals (game region, recorded as of the last smoke runs, 2026-10-14)

| Level | Functions | Share | Code size |
|---|---:|---:|---:|
| diffed | 2,205 | 14.8% | 135 KB |
| executed | 5,536 | 37.2% | 1,209 KB |
| never ran | 7,150 | 48.0% | ~965 KB |
| mismatched | 9 | 0.1% | 1 KB |
| **total written** | **14,900** | | **2,309 KB** |

- 14,605 functions are flagged **installed**. 282 written functions have no ledger entry yet, and the never-ran
  row counts them.
- 73 entries have inconclusive in-game diffs (record/replay could not decide). 3 had their diff evidence reset
  after harness fixes.
- **Mismatched** (9): `0x40D140`, `0x41EA70`, `0x698C10`, `0x6AB9E0`, `0x6AEE40`, `0x6C61F0`, `0x6C6200`,
  `0x712AB0`, `0x7781E0`.
- Most code that only runs in combat (AI tasks, in-mission GUI input) has never run in a recorded smoke. The
  smokes cover start-up, menus and savegame loads.

By module family (module names come from `db/modules.yaml`; "unnamed" modules have no name yet):

| Family | Written | diffed | executed | never ran | mismatched |
|---|---:|---:|---:|---:|---:|
| unnamed modules | 4,123 | 612 | 1,487 | 2,022 | 2 |
| other named | 3,626 | 577 | 1,510 | 1,537 | 2 |
| gfx | 2,099 | 451 | 887 | 758 | 3 |
| ai | 1,338 | 177 | 504 | 656 | 1 |
| menus | 962 | 80 | 254 | 628 | 0 |
| gui | 885 | 150 | 411 | 324 | 0 |
| net | 877 | 23 | 29 | 825 | 0 |
| data defs (bdd) | 356 | 55 | 157 | 144 | 0 |
| audio | 349 | 54 | 187 | 107 | 1 |
| characters | 285 | 26 | 110 | 149 | 0 |

**How to read it.** The ledger says what has been observed, not whether the code is right. "diffed" is the only
level with a correctness signal from the real game, and even that covers only the inputs the smokes happened to
produce. The emulated differential test (`tools/emudiff.py`) adds an offline check. Its sweeps over done code
flagged about 6–10% as DIFF; a sample is being triaged to separate real bugs from false alarms
([docs/emumut.md](docs/emumut.md) measures its blind spots). Several real bugs were found only by running the
standalone, among them stray stack pushes consumed by the wrong call, swapped constants and split stack records.
Expect more.

> `tools/status.py` and `tools/verify.py report` work out the levels by hashing the **built hook objects**
> (`build/CMakeFiles/csf_hook.dir`). In a fresh checkout, or one without a hook build, they report every function
> as `pending` with 0 evidence. That is a missing-build artifact, not lost evidence. The table above reads
> `db/verify.json` directly. Build the hook DLL to get the hash-checked view, which also shows which entries are
> stale.

### Escalation queue (`queue/`, 20 items)

The functions the automated pipeline could not settle, one file each with the failing evidence:

| Count | Reason |
|---:|---|
| 12 | Emulated run differs from the original after every model attempt (`emudiff`) |
| 2 | Reference check failed (the original uses constants the rewrite does not) |
| 3 | Broke the installed baseline in game (crash, found by bisect): `0x41F6C0`, `0x440F50`, `0x4DE9B0` |
| 1 | In-game diff mismatch: `0x44DE90` |
| 1 | Duplicate hook with dev tooling: `0x508F50` |
| 1 | Binding error: `0x6D6800` |

### Standalone link test (snapshot 2026-10-15, `tools/linktest.py`)

- 14,927/14,927 source files compile in the standalone configuration. 15,981 functions are defined in
  total (game code plus library boundary slots).
- Unresolved library functions, which become traps that print their address and abort (never a fake success):
  NetZ 122, RenderWare 15, libpng 1.
- Data generated from the original image at build time: 2,895 globals, 1,154 vtables. 0 raw original addresses
  in code, 0 asm thunks embedding original addresses, 50 other undefined symbols.

### Functions by region

| Region | Functions | Size | Treatment |
|---|---:|---:|---|
| game | 14,903 | 2,309 KB | Reimplemented in `src/auto/` (14,900 done) |
| RenderWare Graphics | 2,159 | 543 KB | Boundary onto librw (`src/boundary/rw/`) |
| D3DX9 | 1,247 | 470 KB | Forwarded to `d3dx9_43.dll` where needed |
| Quazal NetZ | 3,601 | 443 KB | Offline stubs (`src/boundary/netz/`); multiplayer out of scope |
| RenderWare Audio | 581 | 92 KB | Its 40-slot boundary reimplemented on our own backend. Partial: silent timeline and optional DirectSound PCM; compressed banks are not decoded yet ([docs/AUDIO_PORT.md](docs/AUDIO_PORT.md)) |
| MSVC CRT | 486 | 84 KB | Boundary onto the host UCRT (`src/boundary/crt/`) |
| libpng | 156 | 54 KB | Images read through librw's PNG decoder |

---

## How it works

1. **Analysis.** IDA 9.3 (headless, idalib) is the source of truth for what it computes from the binary:
   prototypes, call-site arguments, stack deltas, xrefs. The binary has no symbols and no RTTI, so names come from
   strings, asserts and structure (identifiers are Spanish). Knowledge lives in `db/` and is pushed back into the
   IDB, and Hex-Rays re-derives types from it (`tools/ida/propagate.sh`).
2. **Production.** Each function gets a packet: typed pseudocode, disassembly, class fields, typed virtual calls
   and the canonical binding of every callee. A model writes the C++ and a **gate** checks it: compile; the
   references match the original's (globals, constants, vtable slots); ABI and `ret N`; no raw addresses; and,
   since 2026-10-13, an emulated differential run of the original bytes against our object (`tools/emudiff.py`,
   Unicorn). Gated functions are committed as *pending*.
3. **In-game verification.** `hook/` builds a `dinput8.dll` proxy that installs the reimplemented functions into
   the original game (JMP, vtable and trampoline hooks via MinHook) and records hits and in-game diffs.
   `tools/verifier.py` smokes pending functions in background copies of the game, bisects crashes and quarantines
   culprits to `queue/`.
4. **Cutover.** Every reference to the original goes through an address-keyed macro (`CSF_FUNC`, `CSF_GLOBAL`,
   `CSF_VTBL`, … in `src/globals.h`). The standalone build resolves them to our definitions, generated data
   (`tools/datagen.py`) or library boundaries. Anything unresolved becomes an explicit trap.

### How this was made

This project was built largely by **AI agents under human direction**. Claude (Opus) worked as the orchestrator
and architect. Smaller models (Claude Haiku, OpenAI Codex/GPT and others; the policy is in `db/lanes.yaml`) wrote
most of `src/auto/` through the gated pipeline above. The tooling for that, including `tools/grunt.py`,
`tools/deep_worker.py`, `tools/naming.py`, `tools/review.py` and `CLAUDE.md`, is kept in the repo as a record of
the method. The pipeline is no longer the current way of working: since 2026-10-17, work is done directly and
targeted at the playable mission. Agent logins and API keys are read from `$CSF_CONFIG_DIR` (default
`~/.config/csf-re`) and never stored in the repo.

The evidence ledger exists because code written this way must not be trusted on sight.

---

## Building

Developed on Linux (WSL2), cross-compiling to 32-bit Windows. The game runs on the Windows side.

**Requirements**

- clang-cl / lld-link 21 (plus `llvm-lib-21`, `llvm-rc-21`, `llvm-mt-21`) and the MSVC CRT/SDK from
  [xwin](https://github.com/Jake-Shadle/xwin) in `~/.xwin` (accept its license).
- CMake ≥ 3.20, Ninja.
- Python 3 venv in `.venv` with `pefile`, `capstone`, `pyyaml`, `unicorn`. IDA Pro 9.3 with idalib is needed only
  for the analysis tools (`tools/ida/`), not for building.
- Your copy of the PC game. Put the executable at `work/pc/CommXPC.exe`. The tools were developed against
  SHA-256 `b225664e06d83303e2707bf182606c40aaacf5d38a5eb0a134c31f8f5b8e3d97` (MSVC 7.1 build of 2006-02-23).
- For the standalone target: a librw checkout at `work/tools/librw` (developed against
  `aap/librw@18532c2e13efbc1aa43f0b00b4459831f9414de0`, unmodified).

**Hook DLL** (installs into a copy of the game, never into your original install):

```sh
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/clang-cl-x86.cmake
ninja -C build                      # -> build/dinput8.dll
CSF_PLAY=/path/to/game-copy tools/deploy.sh   # rebuilds, copies the DLL in; pending hooks stay off
```

**Standalone executable** (needs librw; data is generated from your `CommXPC.exe` at build time):

```sh
cmake --build build --target csf_standalone -j 8   # -> build/csf_standalone.exe
.venv/bin/python tools/dryrun.py --walk --dir /path/to/run-dir   # scripted, unattended runs (Windows via WSL interop)
```

See [docs/STANDALONE.md](docs/STANDALONE.md) for the generated inputs and the trap policy, and
`standalone/harness.cpp` for the run-time switches (`CSF_DISPLAY`, `CSF_SKIP_INTRO`, `CSF_MISSION`, `CSF_SHOTS`,
…).

**Checks:** `tools/check_all.sh` (all stages; `--offline` skips the Windows game). It needs the game data for the
real-asset tests (`CSF_FR01_DIR` = an extracted `Maps/FR01` from `Ransom.pak`). `tools/status.py --modules` prints
the live numbers.

**Environment variables used by the tools:** `CSF_PLAY` (hooked game copy), `CSF_POOL` (parallel smoke copies),
`CSF_FR01_DIR` (extracted mission assets), `CSF_CHECK_RW_BUILD` (native RW test build dir), `CSF_CONFIG_DIR`
(agent logins/keys). Several tools still default to the original developer's paths under `/mnt/e/dev/`. Set
the variables.

---

## Repository layout

| Path | Contents |
|---|---|
| `src/auto/` | One file per reimplemented game function (`<address>.cpp`) |
| `src/core/`, `src/types/` | Hand-written core code; generated class layouts (`auto_classes.h`) |
| `src/boundary/` | Library boundaries for the standalone build: `rw` (librw), `rwa` (audio), `netz`, `crt`, `d3dx`, `alloc` |
| `src/globals.h` | Address-keyed bindings to everything original (`CSF_FUNC`/`GLOBAL`/`VTBL`/`ADDR_*`) |
| `hook/` | The `dinput8.dll` proxy: hook installation, in-game diff, crash handler, autopilot |
| `standalone/` | Standalone entry point, run harness, adjustor thunks |
| `db/` | Source-of-truth knowledge: function list and status, regions, modules, globals, types, vtables, ledger |
| `tools/` | Analysis, production, verification and cutover tooling (`tools/ida/` is the only code touching the IDB) |
| `tests/` | Native (C++, ASan/UBSan) and Python tests, negative controls |
| `queue/` | Escalations: functions the pipeline could not settle |
| `docs/` | Plan, journal and per-item reports; see [docs/README.md](docs/README.md) |
| `third_party/minhook/` | MinHook (BSD-2-Clause), vendored |

Not in the repo (gitignored): `work/` (exe copy, extracted data, third-party checkouts), `ida/` (the IDB),
`export/` (derived from the IDB), `build/`.

---

## Legal

- This is an independent, non-commercial preservation and research project. It is **not affiliated with or
  endorsed by** Pyro Studios, Eidos Interactive, Square Enix or any other rights holder. *Commandos* and
  *Commandos: Strike Force* are trademarks of their respective owners.
- The repository contains **no game executable, data, assets or original code bytes**. The original data image
  the standalone build needs is generated locally, at build time, from the user's own copy of the game.
- No leaked RenderWare SDK source was used. librw (MIT) is the reference and the replacement.
- The project's own code is under the [MIT License](LICENSE). Third-party: MinHook (BSD-2-Clause,
  `third_party/minhook/LICENSE.txt`). librw (MIT) is used as an external checkout and not vendored.
