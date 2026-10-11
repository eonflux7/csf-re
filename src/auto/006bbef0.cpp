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

CSF_FUNC(int, __cdecl, RpWorldForAllMaterials, 0x0058CF30, (int a1, int (__cdecl* a2)(uint32_t, int), int a3));

// Forwards the world to RpWorldForAllMaterials with the material callback at 0x6BBBF0 and a null user data.
static int __cdecl GfxObjectProps_ForEachWorldMaterial(int world)
{
    int (__cdecl* materialCallback)(uint32_t, int) =
        reinterpret_cast<int (__cdecl*)(uint32_t, int)>(CSF_ADDR_FN(0x6BBBF0));
    return RpWorldForAllMaterials(world, materialCallback, 0);
}

HOOK(0x6BBEF0, GfxObjectProps_ForEachWorldMaterial, "auto");
