#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_Value_8B066C, 0x008B066C);

// Releases the cached object held in g_Value_8B066C: if set, calls its vtable
// slot 0 (byte offset 0) with flag 1, then clears the global. Returns the slot's result
// (0 when the global was empty).
static int __cdecl ReleaseCachedObjectGlobalVtbl0()
{
    int result = 0;
    if (g_Value_8B066C != 0)
    {
        void* cachedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_Value_8B066C)));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, cachedObject, 0)[0])(cachedObject, nullptr, 1);
        g_Value_8B066C = 0;
    }
    return result;
}

HOOK(0x00519400, ReleaseCachedObjectGlobalVtbl0, "auto");
