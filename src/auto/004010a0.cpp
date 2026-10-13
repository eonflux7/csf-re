#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_Value_8afc60, 0x008AFC60);

// Releases the cached object held in g_Value_8afc60: if set, calls its vtable
// slot 2 (byte offset 8) with flag 1, then clears the global. Returns the slot's result.
static int __cdecl ReleaseCachedObjectGlobal()
{
    int result = 0;
    if (g_Value_8afc60 != 0)
    {
        void* cachedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_Value_8afc60)));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, cachedObject, 0)[2])(cachedObject, nullptr, 1);
        g_Value_8afc60 = 0;
    }
    return result;
}

HOOK(0x004010A0, ReleaseCachedObjectGlobal, "auto");
