// Module: net_ddl_object_manager
// Resets the 16-dword block at +0x4C of the spawn-point player-check struct to -1
// and returns a pointer to its first dword.

#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int32_t* __fastcall MpSpawnPoints_ResetPlayerCheck(S_00749d70* self, void* /*edx*/)
{
    // Original writes -1 to dwords at this+0x4C .. this+0x88 (16 entries, indices 19..34).
    for (int i = 0; i < 16; ++i)
    {
        CSF_AT(int32_t, self, 0x4C + i * 4) = -1;
    }

    // Returns this + 19 dwords (= this + 0x4C).
    return reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(self) + 0x4C);
}

HOOK(0x00749B30, MpSpawnPoints_ResetPlayerCheck, "auto");
