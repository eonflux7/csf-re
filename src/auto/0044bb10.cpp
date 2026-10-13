#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_RegisterObject, 0x00475850, (int a1));

// Registers the spawn point object with the multiplayer spawn-point list the first
// time it is called. Later calls return the cached flag (0 from the first check).
static char __fastcall EnsureSpawnPointRegistered(void* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 8);
    if (!result)
    {
        CSF_AT(int, self, 0x1C) = -1;
        CSF_AT(int, self, 0x20) = 0;
        result = static_cast<char>(MpSpawnPoints_RegisterObject(reinterpret_cast<int>(self)));
        CSF_AT(char, self, 8) = 1;
    }
    return result;
}

HOOK(0x0044BB10, EnsureSpawnPointRegistered, "auto");
