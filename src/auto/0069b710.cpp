#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_Value_8baf94, 0x008BAF94);

// Releases the cached object held in g_Value_8baf94: if set, calls its vtable slot 0
// with flag 1 (destroy-with-free), then clears the global. Returns the call's result,
// or 0 when no object was cached.
static int __cdecl CachedInstance_Release()
{
    int result = 0;
    if (g_Value_8baf94 != 0)
    {
        void* cachedInstance = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_Value_8baf94)));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, cachedInstance, 0)[0])(cachedInstance, nullptr, 1);
        g_Value_8baf94 = 0;
    }
    return result;
}

HOOK(0x0069B710, CachedInstance_Release, "auto");
