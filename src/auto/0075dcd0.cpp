#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, WaypointGraph_CopyIndexPairs, 0x004DC7A0,
         (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(unsigned int, __fastcall, WaypointGraph_InsertIndexPairs, 0x004DD600,
         (void* self, void* edx, int a1, unsigned int a3, uint32_t* arg8));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall WaypointGraph_FindPointsInRadiusRange(
    void* self, void* /*edx*/, int groupId, float* position,
    float minimumRadius, float maximumRadius, uint32_t* results)
{
    void* oldStorage = CSF_AT(void*, results, 0x04);
    if (oldStorage)
        Mem_FreeDebug(oldStorage);

    CSF_AT(uint32_t, results, 0x04) = 0;
    CSF_AT(uint32_t, results, 0x08) = 0;
    CSF_AT(uint32_t, results, 0x0C) = 0;

    const float minimumSquared = minimumRadius * minimumRadius;
    const float maximumSquared = maximumRadius * maximumRadius;

    alignas(8) uint8_t selectionBytes[0x1C];
    uint32_t* selection = reinterpret_cast<uint32_t*>(selectionBytes);
    selection[0] = UINT32_MAX;
    selection[1] = UINT32_MAX;

    const uint32_t groupsBegin = CSF_AT(uint32_t, self, 0x08);
    const uint32_t groupCount = groupsBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(CSF_AT(uint32_t, self, 0x0C) - groupsBegin) / 72)
        : 0;

    uint32_t groupOffset = 0;
    for (uint32_t groupIndex = 0; groupIndex < groupCount;
         ++groupIndex, groupOffset += 72)
    {
        if (groupId != -1)
        {
            uint8_t* group = CSF_AT(uint8_t*, self, 0x08) + groupOffset;
            if (CSF_AT(int, group, 0x20) != groupId)
                continue;
        }

        uint8_t* group = CSF_AT(uint8_t*, self, 0x08) + groupOffset;
        const uint32_t pointsBegin = CSF_AT(uint32_t, group, 0x38);
        uint32_t pointCount = 0;
        if (pointsBegin)
        {
            const int32_t pointBytes = static_cast<int32_t>(
                CSF_AT(uint32_t, group, 0x3C) - pointsBegin);
            // Signed division by the 44-byte point stride.
            const int32_t quotient = static_cast<int32_t>(
                (0x2E8BA2E9LL * pointBytes) >> 32) >> 3;
            pointCount = static_cast<uint32_t>(quotient)
                       + (static_cast<uint32_t>(quotient) >> 31);
        }

        uint32_t pointOffset = 0;
        for (uint32_t pointIndex = 0; pointIndex < pointCount;
             ++pointIndex, pointOffset += 44)
        {
            uint8_t* currentGroup = CSF_AT(uint8_t*, self, 0x08) + groupOffset;
            uint8_t* point = CSF_AT(uint8_t*, currentGroup, 0x38) + pointOffset;
            const float dx = CSF_AT(float, point, 0x04) - position[0];
            const float dy = CSF_AT(float, point, 0x08) - position[1];
            const float dz = CSF_AT(float, point, 0x0C) - position[2];
            const float distanceSquared = dz * dz + dx * dx + dy * dy;

            if (distanceSquared >= minimumSquared && distanceSquared <= maximumSquared)
            {
                selection[0] = groupIndex;
                const uint32_t begin = CSF_AT(uint32_t, results, 0x04);
                selection[1] = pointIndex;
                const uint32_t size = begin
                    ? static_cast<uint32_t>(
                          static_cast<int32_t>(CSF_AT(uint32_t, results, 0x08) - begin) >> 3)
                    : 0;

                if (begin &&
                    size < static_cast<uint32_t>(
                        static_cast<int32_t>(CSF_AT(uint32_t, results, 0x0C) - begin) >> 3))
                {
                    const uint32_t end = CSF_AT(uint32_t, results, 0x08);
                    WaypointGraph_CopyIndexPairs(
                        static_cast<int>(end), 1, reinterpret_cast<int>(selection),
                        reinterpret_cast<int>(results), static_cast<int>(pointOffset));
                    CSF_AT(uint32_t, results, 0x08) = end + 8;
                }
                else
                {
                    WaypointGraph_InsertIndexPairs(
                        results, nullptr,
                        static_cast<int>(CSF_AT(uint32_t, results, 0x08)),
                        1, selection);
                }
            }
        }
    }

    const uint32_t begin = CSF_AT(uint32_t, results, 0x04);
    return begin &&
        (static_cast<int32_t>(CSF_AT(uint32_t, results, 0x08) - begin) >> 3) != 0;
}

HOOK(0x0075DCD0, WaypointGraph_FindPointsInRadiusRange, "auto");
