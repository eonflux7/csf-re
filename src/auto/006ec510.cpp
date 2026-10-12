#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectSectorData, 0x006B87C0, (int objectIndex));

static int __cdecl GfxCollision_FilterSectorIntersection(
    int /*unused1*/, int intersection, int /*unused3*/, int context)
{
    const auto object = CSF_AT(std::uint32_t, context, 0);
    const auto geometry = CSF_AT(std::uint32_t, object, 0x18);
    const auto entries = CSF_AT(std::uint32_t, geometry, 0x2C);
    const auto entryIndex = CSF_AT(std::uint32_t, intersection, 0x18);
    const auto sectorIndex = CSF_AT(std::uint8_t, entries + entryIndex * 8, 6);
    const auto sectors = CSF_AT(std::uint32_t, geometry, 0x20);
    const int objectIndex = CSF_AT(int, sectors + sectorIndex * 4, 0);

    if (!objectIndex)
        return intersection;

    const int sectorData = GfxObjectProps_GetObjectSectorData(objectIndex);
    if (!sectorData || CSF_AT(std::uint8_t, context, 5))
        return intersection;
    if (CSF_AT(std::uint8_t, context, 4) &&
        CSF_AT(std::uint8_t, sectorData, 0x10))
        return intersection;
    if ((CSF_AT(std::uint32_t, sectorData, 0xC) &
         CSF_AT(std::uint32_t, context, 8)) == 0)
        return intersection;

    CSF_AT(std::uint8_t, context, 0xC) = 1;
    return 0;
}

HOOK(0x006EC510, GfxCollision_FilterSectorIntersection, "auto");
