# Documentation index

Living documents (kept current):

| File | What it is |
|---|---|
| [PLAN.md](PLAN.md) | The plan: facts about the target, the production loop, current focus, backlog, decisions |
| [JOURNAL.md](JOURNAL.md) | Dated findings log, newest at the bottom. Older entries are snapshots and can be superseded |
| [STANDALONE.md](STANDALONE.md), [STANDALONE_LINK2.md](STANDALONE_LINK2.md) | How the standalone exe is linked, generated inputs, trap policy |
| [DATAGEN.md](DATAGEN.md) | Generating the original's data image (globals, vtables) for the standalone build |

Reports below were written at the end of individual work items. Each one records what was done, how it was
tested and what was **not** tested on its date. Later work may have superseded them; PLAN.md and the latest
JOURNAL entries are authoritative.

**RenderWare Graphics → librw** (the largest library boundary)

| File | Scope |
|---|---|
| [RW_PORT.md](RW_PORT.md), [RW_PORT_DESIGN.md](RW_PORT_DESIGN.md) | Cutover survey and design of the librw boundary |
| [RW_PORT_INVENTORY.md](RW_PORT_INVENTORY.md), [RW_PORT_SLOTS.json](RW_PORT_SLOTS.json) | Slot inventory (every RW entry point the game uses) |
| [RW_PORT_OFFSETS.md](RW_PORT_OFFSETS.md), [RW_PORT_OFFSETS.csv](RW_PORT_OFFSETS.csv), [RW_PORT_LIBRW_LAYOUTS.txt](RW_PORT_LIBRW_LAYOUTS.txt) | Structure offsets the game reads, with evidence; librw's Win32 layouts |
| RW_SLICE{2,3,4,7,9}_REPORT.md | Early slices (core objects, rasters/images, geometry, device, toolkits) |
| [RW_STARTUP_REPORT.md](RW_STARTUP_REPORT.md), [RW_PLUGINS_REPORT.md](RW_PLUGINS_REPORT.md), [RW_FS_REPORT.md](RW_FS_REPORT.md) | Engine start-up, plugin registration, RtFS / DDS |
| [RW_CLUMP_REPORT.md](RW_CLUMP_REPORT.md), [RW_ANIMATION_REPORT.md](RW_ANIMATION_REPORT.md) | Clump streaming; RtAnim / HAnim / skin / MatFX |
| [RW_WORLD_REPORT.md](RW_WORLD_REPORT.md), [RW_WORLD2_REPORT.md](RW_WORLD2_REPORT.md), [RW_WORLD3_REPORT.md](RW_WORLD3_REPORT.md) | BSP world API, world streaming, FR01 real-asset regression |
| [RW_PARTICLES_REPORT.md](RW_PARTICLES_REPORT.md), [RW_PARTICLES_8B_REPORT.md](RW_PARTICLES_8B_REPORT.md), [RW_PRT_REPORT.md](RW_PRT_REPORT.md) | Particles and PTank |
| [RW_TEAM_REPORT.md](RW_TEAM_REPORT.md), [RW_MISC_REPORT.md](RW_MISC_REPORT.md) | Rwp physics cluster; scattered slots |

**Other boundaries and start-up**

| File | Scope |
|---|---|
| [AUDIO_PORT.md](AUDIO_PORT.md), [AUDIO_INIT_REPORT.md](AUDIO_INIT_REPORT.md) | RenderWare Audio boundary on our own backend |
| [NETZ_LOAD_REPORT.md](NETZ_LOAD_REPORT.md) | Quazal NetZ slots on the mission-load path (offline stubs) |
| [MISSION1_REPORT.md](MISSION1_REPORT.md) | Mission loading: offline investigation |
| [INIT_FUNCTIONS.md](INIT_FUNCTIONS.md), [WINMAIN_REST.md](WINMAIN_REST.md) | CRT initializers; remaining WinMain slots |
| INIT_REACH.txt, WINMAIN_REACH.txt, WINMAIN_REACH_VTABLES.txt | Generated reachability listings (trap slots reachable from start-up / WinMain) |

**Verification and method**

| File | Scope |
|---|---|
| [EMUDIFF_PROFILES.md](EMUDIFF_PROFILES.md), [EMUDIFF_FALSE_ALARMS.tsv](EMUDIFF_FALSE_ALARMS.tsv) | Emulated differential testing: inputs, known false alarms |
| [emumut.md](emumut.md) | Mutation benchmark for emudiff (what it can and cannot detect) |
| [TINY_FUNCTIONS.md](TINY_FUNCTIONS.md) | Decision paper: game functions under six bytes |
| [stl_survey.md](stl_survey.md), [stl_last_slice_report.md](stl_last_slice_report.md) | VC7.1 STL instantiations in the binary |
