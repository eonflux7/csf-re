#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_Value_8b0668, 0x008B0668);

// Releases the cached object held in g_Value_8b0668: if set, calls its vtable
// slot 0 (byte offset 0) with flag 1, then clears the global. Returns the slot's result,
// or 0 when the global was empty.
static int __cdecl ReleaseCachedObjectGlobal()
{
    int result = 0;
    if (g_Value_8b0668 != 0)
    {
        void* cachedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_Value_8b0668)));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, cachedObject, 0)[0])(cachedObject, nullptr, 1);
        g_Value_8b0668 = 0;
    }
    return result;
}

HOOK(0x00519280, ReleaseCachedObjectGlobal, "auto");
