#include <cstdint>
#include "globals.h"
#include "hook.h"

static bool __fastcall BddWaypointGraph_ContainsSpot(
    void* self, void* /*edx*/, int groupId, float* position)
{
    const auto groupsBegin = CSF_AT(std::uint8_t*, self, 0x08);
    const std::uint32_t groupCount = groupsBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  reinterpret_cast<std::uintptr_t>(
                      CSF_AT(std::uint8_t*, self, 0x0C)) -
                  reinterpret_cast<std::uintptr_t>(groupsBegin)) / 72)
        : 0;

    std::uint32_t groupOffset = 0;
    for (std::uint32_t groupIndex = 0; groupIndex < groupCount;
         ++groupIndex, groupOffset += 72)
    {
        if (groupId != -1 &&
            CSF_AT(int, CSF_AT(std::uint8_t*, self, 0x08) + groupOffset,
                   0x20) != groupId)
        {
            continue;
        }

        auto* group = CSF_AT(std::uint8_t*, self, 0x08) + groupOffset;
        const auto pointsBegin = CSF_AT(std::uint8_t*, group, 0x38);
        const std::uint32_t pointCount = pointsBegin
            ? static_cast<std::uint32_t>(
                  static_cast<std::int32_t>(
                      reinterpret_cast<std::uintptr_t>(
                          CSF_AT(std::uint8_t*, group, 0x3C)) -
                      reinterpret_cast<std::uintptr_t>(pointsBegin)) / 44)
            : 0;

        std::uint32_t pointOffset = 0;
        for (std::uint32_t pointIndex = 0; pointIndex < pointCount;
             ++pointIndex, pointOffset += 44)
        {
            auto* point = CSF_AT(std::uint8_t*, group, 0x38) + pointOffset;
            if (CSF_AT(float, point, 0x04) == position[0] &&
                CSF_AT(float, point, 0x08) == position[1] &&
                CSF_AT(float, point, 0x0C) == position[2])
            {
                return true;
            }
        }
    }

    return false;
}

HOOK(0x0075C5A0, BddWaypointGraph_ContainsSpot, "auto");
