// NetCogibleDDL.cpp (generated DDL)
#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Stores the new player state byte at +0x443. If the state changes while the
// flag at +0x6F0 is set, that flag is cleared first.
static int __fastcall MpSpawnPoints_UpdatePlayerState(uint8_t* self, void* /*edx*/, int newState)
{
    const int result = newState;

    if (static_cast<int>(CSF_AT(int8_t, self, 0x443)) != newState &&
        CSF_AT(uint8_t, self, 0x6F0) != 0)
    {
        CSF_AT(uint8_t, self, 0x6F0) = 0;
    }

    if (static_cast<int8_t>(newState) != CSF_AT(int8_t, self, 0x443))
    {
        CSF_AT(int8_t, self, 0x443) = static_cast<int8_t>(newState);
    }

    return result;
}

HOOK(0x00740F40, MpSpawnPoints_UpdatePlayerState, "auto");
