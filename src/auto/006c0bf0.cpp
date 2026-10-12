#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callback signature used by RpGeometryForAllMaterials: called per material with the caller's data.
typedef int (__cdecl* MaterialVisitorFn)(int material, int data);

CSF_FUNC(int, __cdecl, RpGeometryForAllMaterials, 0x00590670, (int geometry, int (__cdecl* callback)(int, int), int userData));

// Original calls RpGeometryForAllMaterials(geometry, PushValueToDynamicArray @ 0x6BD100, collector).
static int __cdecl GfxFrames_CollectGeometryMaterials(int geometryHandle, int collectorHandle)
{
    // Reset the collector's counter field at +0x08 before visiting materials.
    CSF_AT(int, reinterpret_cast<void*>(collectorHandle), 0x8) = 0;

    return RpGeometryForAllMaterials(
        geometryHandle,
        reinterpret_cast<MaterialVisitorFn>(CSF_ADDR_FN(0x006BD100)),
        collectorHandle);
}

HOOK(0x006C0BF0, GfxFrames_CollectGeometryMaterials, "auto");
