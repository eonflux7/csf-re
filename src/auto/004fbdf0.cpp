#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(uint32_t, g_Value_8B0564, 0x008B0564);

// Releases the cached console object held in g_Value_8B0564: if set, calls its
// vtable slot 0 (byte offset 0) with flag 1, then clears the global.
// Returns the slot's result (0 when the global was empty).
static int __cdecl DevConsole_ReleaseCachedObjectGlobal()
{
    int result = 0;
    if (g_Value_8B0564 != 0)
    {
        void* cachedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8B0564));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, cachedObject, 0)[0])(cachedObject, nullptr, 1);
        g_Value_8B0564 = 0;
    }
    return result;
}

HOOK(0x004FBDF0, DevConsole_ReleaseCachedObjectGlobal, "auto");
