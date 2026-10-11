#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee: RpGeometryForAllMaterials(RpGeometry*, callback, userData)
CSF_FUNC(int, __cdecl, RpGeometryForAllMaterials, 0x590670,
         (int geometry, int (__cdecl* callback)(uint32_t, int), int userData));

static int __cdecl GfxSkin_IterateSkinMaterials(int skin, int userData)
{
    // Geometry pointer lives at +0x18 of the skin object.
    const int geometry = CSF_AT(int, reinterpret_cast<void*>(skin), 0x18);

    // Callback at 0x6C4F30 is passed by address (original pushes it as a value).
    auto callback = reinterpret_cast<int (__cdecl*)(uint32_t, int)>(CSF_ADDR_FN(0x6C4F30));

    return RpGeometryForAllMaterials(geometry, callback, userData);
}

HOOK(0x6C4F50, GfxSkin_IterateSkinMaterials, "auto");
