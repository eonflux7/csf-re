#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_Value_8B08F8, 0x008B08F8);

// Releases the cached object held in g_Value_8B08F8: if set, calls its vtable
// slot 0 with flag 1 (destroy-with-free), then clears the global.
static int __cdecl MpRules_ReleaseCachedInstance()
{
    int result = 0;
    if (g_Value_8B08F8 != 0)
    {
        void* cachedInstance = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_Value_8B08F8)));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, cachedInstance, 0)[0])(cachedInstance, nullptr, 1);
        g_Value_8B08F8 = 0;
    }
    return result;
}

HOOK(0x0055A9D0, MpRules_ReleaseCachedInstance, "auto");
