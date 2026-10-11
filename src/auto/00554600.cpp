#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __cdecl, sub_73E5A0, 0x0073E5A0, ());
CSF_FUNC(int, __fastcall, sub_553680, 0x00553680, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, sub_554250, 0x00554250, (void* self, void* edx));

// The original references this class's vtable (per the packet). It is only compared or
// stored by the caller side, not written here, so keep a read-only reference to it.
[[maybe_unused]] static const void* const kSpawnPointsVtableRef = CSF_VTBL(0x00553680);

// Picks the spawn-point mode from two global team values, then either returns the
// sentinel (mode 1), runs the mode-2 setup and falls through, or tail-calls the
// default spawn-point handler (mode 0 and mode 2).
static C_00833908* __fastcall SpawnPoints_ResolveForMode(S_00553680* self, void* /*edx*/)
{
    int spawnMode = 2;

    if (sub_73E5A0() != 0) {
        GetGlobalDword_73C020();
        if (sub_73E5A0() == 2)
            spawnMode = 1;
    } else {
        spawnMode = 0;
    }

    if (spawnMode != 0) {
        if (spawnMode != 2)
            return reinterpret_cast<C_00833908*>(static_cast<std::intptr_t>(spawnMode - 2));
        sub_553680(self, nullptr);
    }

    return static_cast<C_00833908*>(sub_554250(self, nullptr));
}

HOOK(0x554600, SpawnPoints_ResolveForMode, "auto");
