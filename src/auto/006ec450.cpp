#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpWorldSectorGetWorld, 0x0058D1A0, (unsigned int a1));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectSectorData, 0x006B87C0, (int objectIndex));

static std::uint32_t* __cdecl GfxCollision_RecordSectorIntersection(
    int /*unused*/, int sectorAddress, std::uint32_t* collision,
    int /*unused*/, int queryAddress)
{
    auto* sector = reinterpret_cast<std::uint8_t*>(sectorAddress);
    auto* query = reinterpret_cast<std::uint8_t*>(queryAddress);

    const auto* triangles = CSF_AT(std::uint8_t*, sector, 0x4);
    const std::uint16_t materialIndex =
        CSF_AT(std::uint8_t, triangles + collision[6] * 8, 0x6);

    const int worldAddress =
        RpWorldSectorGetWorld(static_cast<unsigned int>(sectorAddress));
    const int objectIndex =
        CSF_AT(int*, reinterpret_cast<void*>(worldAddress), 0x10)[materialIndex];
    auto* sectorData = reinterpret_cast<std::uint8_t*>(
        GfxObjectProps_GetObjectSectorData(objectIndex));

    if (CSF_AT(std::uint8_t, query, 0x3) ||
        !sectorData ||
        (CSF_AT(std::uint8_t, query, 0x1) &&
         CSF_AT(std::uint8_t, sectorData, 0x10)) ||
        !(CSF_AT(std::uint32_t, sectorData, 0xC) &
          CSF_AT(std::uint32_t, query, 0x4)))
    {
        return collision;
    }

    auto* result = CSF_AT(std::uint8_t*, query, 0xC);
    CSF_AT(std::uint8_t, query, 0x2) = 1;

    if (result)
    {
        const int hitWorld =
            RpWorldSectorGetWorld(static_cast<unsigned int>(sectorAddress));
        CSF_AT(int, CSF_AT(std::uint8_t*, query, 0xC), 0x34) = hitWorld;
        CSF_AT(std::uint32_t, CSF_AT(std::uint8_t*, query, 0xC), 0x38) = 0;
        std::memcpy(CSF_AT(std::uint8_t*, query, 0xC) + 0xC, collision, 0x28);
        CSF_AT(std::uint32_t, CSF_AT(std::uint8_t*, query, 0xC), 0x4) =
            CSF_AT(std::uint32_t, sectorData, 0x8);
    }

    return nullptr;
}

HOOK(0x006EC450, GfxCollision_RecordSectorIntersection, "auto");
