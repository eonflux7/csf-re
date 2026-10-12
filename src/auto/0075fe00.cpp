#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_Value_8BC1E4, 0x008BC1E4);

// Releases the cached object held in g_Value_8BC1E4: if set, calls its vtable slot 0
// with flag 1 (destroy-with-free), then clears the global.
static int __cdecl AppConfig_ReleaseCachedInstance()
{
    int result = 0;
    if (g_Value_8BC1E4 != 0)
    {
        void* cachedInstance = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_Value_8BC1E4)));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, cachedInstance, 0)[0])(cachedInstance, nullptr, 1);
        g_Value_8BC1E4 = 0;
    }
    return result;
}

HOOK(0x0075FE00, AppConfig_ReleaseCachedInstance, "auto");
