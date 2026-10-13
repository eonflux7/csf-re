#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_RegisterObject, 0x00475850, (int a1));

CSF_GLOBAL(int, g_Value8AFC84, 0x008AFC84);

static C_008311a0* __cdecl MpSpawnPoints_GetSingleton()
{
    C_008311a0* result = reinterpret_cast<C_008311a0*>(g_Value8AFC84);
    if (g_Value8AFC84 == 0)
    {
        result = reinterpret_cast<C_008311a0*>(CoreMemory_AllocUntagged(36));
        C_008311a0* created = result;
        if (created != nullptr)
        {
            *reinterpret_cast<void***>(created) = CSF_VTBL(0x008311A0);
            CSF_AT(uint32_t, created, 0x08) = 0;
            CSF_AT(uint32_t, created, 0x0C) = 0;
            CSF_AT(uint32_t, created, 0x10) = 0;
            CSF_AT(uint32_t, created, 0x18) = 0;
            CSF_AT(uint32_t, created, 0x1C) = 0;
            CSF_AT(uint32_t, created, 0x20) = 0;
            result = reinterpret_cast<C_008311a0*>(
                MpSpawnPoints_RegisterObject(reinterpret_cast<int>(created)));
            g_Value8AFC84 = reinterpret_cast<int>(created);
        }
        else
        {
            g_Value8AFC84 = 0;
        }
    }
    return result;
}

HOOK(0x0044BAC0, MpSpawnPoints_GetSingleton, "auto");
