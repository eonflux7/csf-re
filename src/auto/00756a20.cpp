#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(uint32_t, g_Value_8BC1E0, 0x008BC1E0);

// Releases the cached configuration object held in g_Value_8BC1E0: if set, calls its
// vtable slot 0 with flag 1 (destroy-with-free), then clears the global. Returns the
// slot's result, or 0 when no object was cached.
static int __cdecl GameConfig_ReleaseCachedInstance()
{
    int result = 0;
    if (g_Value_8BC1E0 != 0)
    {
        void* cachedInstance = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8BC1E0));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, cachedInstance, 0)[0])(cachedInstance, nullptr, 1);
        g_Value_8BC1E0 = 0;
    }
    return result;
}

HOOK(0x756A20, GameConfig_ReleaseCachedInstance, "auto");
