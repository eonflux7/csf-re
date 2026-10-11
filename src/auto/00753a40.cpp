#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_GlobalValue, 0x008BC1DC);

// Releases the cached game-config object held in g_GlobalValue: if set, calls its
// vtable slot 0 with flag 1 (destroy-with-free), then clears the global.
static int __cdecl GameConfig_ReleaseCachedInstance()
{
    int result = 0;
    if (g_GlobalValue != 0)
    {
        void* cachedInstance = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_GlobalValue)));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, cachedInstance, 0)[0])(cachedInstance, nullptr, 1);
        g_GlobalValue = 0;
    }
    return result;
}

HOOK(0x00753A40, GameConfig_ReleaseCachedInstance, "auto");
