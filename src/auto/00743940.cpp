#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(uint32_t, g_MpSpawnPointsObject, 0x008BC140);

static int __cdecl MpSpawnPoints_CompleteSpawn(int a1)
{
    uint32_t spawnPointsObject = g_MpSpawnPointsObject;
    int result = (int)spawnPointsObject;
    if (spawnPointsObject)
        CSF_AT(int, reinterpret_cast<void*>(spawnPointsObject), 0x24) = a1;
    return result;
}

HOOK(0x743940, MpSpawnPoints_CompleteSpawn, "auto");
