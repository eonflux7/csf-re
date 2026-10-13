#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_Value_8B0574, 0x008B0574);

// Releases the cached menu-screen object held in g_Value_8B0574: if set, calls its
// vtable slot 0 with flag 1 (destroy-with-free), then clears the global.
static int __cdecl GameBddLoader_CleanupMenuScreens()
{
    int result = 0;
    if (g_Value_8B0574 != 0)
    {
        void* cachedScreen = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_Value_8B0574)));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, cachedScreen, 0)[0])(cachedScreen, nullptr, 1);
        g_Value_8B0574 = 0;
    }
    return result;
}

HOOK(0x00518990, GameBddLoader_CleanupMenuScreens, "auto");
