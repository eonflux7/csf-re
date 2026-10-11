#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int32_t, g_value_008B0670, 0x008B0670);

static int32_t __cdecl FxParticleManager_DestroyInstance()
{
    int32_t result = g_value_008B0670;
    if (result)
    {
        void* subObject = reinterpret_cast<uint8_t*>(result) + 4;
        result = reinterpret_cast<int32_t(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, subObject, 0)[0])(subObject, nullptr, 1);
        g_value_008B0670 = 0;
    }
    return result;
}

HOOK(0x0051F750, FxParticleManager_DestroyInstance, "auto");
