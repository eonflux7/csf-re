#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_Value8BB4B8, 0x008BB4B8);

// Releases the cached skin object held in g_Value8BB4B8: if set, calls its
// vtable slot 1 with flag 1 (destroy-with-free), then clears the global.
static int __cdecl GfxSkin_ReleaseCachedSkinObject()
{
    int result = 0;
    if (g_Value8BB4B8 != 0)
    {
        void* cachedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_Value8BB4B8)));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, cachedObject, 0)[1])(cachedObject, nullptr, 1);
        g_Value8BB4B8 = 0;
    }
    return result;
}

HOOK(0x006C5740, GfxSkin_ReleaseCachedSkinObject, "auto");
