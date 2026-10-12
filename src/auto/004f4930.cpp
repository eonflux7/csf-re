#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int32_t, g_Value8B0558, 0x008B0558);

static int __cdecl HudStringShow_ReleaseCachedInstance()
{
    int result = 0;
    if (g_Value8B0558 != 0)
    {
        void* cachedInstance = reinterpret_cast<void*>(
            static_cast<uintptr_t>(static_cast<uint32_t>(g_Value8B0558)));
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, cachedInstance, 0)[0])(cachedInstance, nullptr, 1);
        g_Value8B0558 = 0;
    }
    return result;
}

HOOK(0x004F4930, HudStringShow_ReleaseCachedInstance, "auto");
