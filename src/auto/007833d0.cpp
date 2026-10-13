#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game heap allocator used by the original (20 bytes).
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// Initializer for the cache object; takes the object as `this` plus one stack argument.
CSF_FUNC(char, __fastcall, CacheObject_Init, 0x00783340, (void* self, void* edx, int a2));

CSF_GLOBAL(int, g_Value8BC1EC, 0x008BC1EC);

static char __cdecl EnsureGlobalCacheObject(int a2)
{
    if (g_Value8BC1EC != 0)
        return 0;

    S_00782f90* cacheObject =
        reinterpret_cast<S_00782f90*>(CoreMemory_AllocUntagged(20));

    if (cacheObject == nullptr)
    {
        g_Value8BC1EC = 0;
        return 0;
    }

    cacheObject->field_0004 = 0;
    cacheObject->field_0008 = 0;
    cacheObject->field_000c = 0;
    CSF_AT(int, cacheObject, 0x10) = 0;

    char result = CacheObject_Init(cacheObject, nullptr, a2);
    g_Value8BC1EC = (int)(std::uintptr_t)cacheObject;
    return result;
}

HOOK(0x007833D0, EnsureGlobalCacheObject, "auto");
