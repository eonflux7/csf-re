#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpGeometryForAllMaterials, 0x00590670, (int geometry, int (__cdecl* callback)(int, int), int userData));

static int __cdecl GfxFrames_ForEachFrameMaterial(int a1, int* a2)
{
    if (a1)
    {
        void* frameObj = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));
        int geometry = CSF_AT(int, frameObj, 0x18);
        if (geometry)
        {
            int userData = a2[1];
            int callbackAddr = a2[0];
            RpGeometryForAllMaterials(
                geometry,
                reinterpret_cast<int (__cdecl*)(int, int)>(static_cast<uintptr_t>(callbackAddr)),
                userData);
        }
    }
    return a1;
}

HOOK(0x6C02F0, GfxFrames_ForEachFrameMaterial, "auto");
