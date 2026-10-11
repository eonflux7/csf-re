#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// RpGeometryForAllMaterials (RenderWare, lib) - calls a callback for every material of a geometry.
CSF_FUNC(int, __cdecl, RpGeometryForAllMaterials, 0x590670,
         (int geometry, int (__cdecl* callback)(int, int), int userData));

// Material callback at 0x6C4DD0, passed to RpGeometryForAllMaterials. Bound as an address value.
static int (__cdecl* const kMaterialAlphaCallback)(int, int) =
    reinterpret_cast<int (__cdecl*)(int, int)>(CSF_ADDR_FN(0x6C4DD0));

// Clamps the requested alpha to [0, 255], flags the atomic's geometry, and applies the
// alpha to every material of that geometry.
static int __cdecl SkinGeometry_SetMaterialAlpha(int atomicPtr, int alpha)
{
    if (alpha >= 0)
    {
        if (alpha > 255)
            alpha = 255;
    }
    else
    {
        alpha = 0;
    }

    int geometry = (int)CSF_AT(uint32_t, atomicPtr, 0x18);
    if (geometry != 0)
    {
        CSF_AT(uint32_t, geometry, 8) |= 0x40u;
        return RpGeometryForAllMaterials(geometry, kMaterialAlphaCallback, (int)&alpha);
    }
    return geometry;
}

HOOK(0x6C4E60, SkinGeometry_SetMaterialAlpha, "auto");
