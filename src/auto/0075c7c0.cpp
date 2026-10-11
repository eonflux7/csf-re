#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_FloatMax, 0x00831090);

static int __fastcall BddWaypointGraph_LookupSlotForVector(
    void* self, void* /*edx*/, float* position, void* filter,
    unsigned int* slot)
{
    float bestDistance = g_FloatMax;
    slot[0] = static_cast<unsigned int>(-1);
    slot[1] = static_cast<unsigned int>(-1);

    bool finished = false;
    if (CSF_AT(uint8_t, filter, 0x04) == 0)
        bestDistance = g_TransitionOne;

    const uint32_t groupsBegin = CSF_AT(uint32_t, self, 0x08);
    uint32_t groupCount = 0;
    if (groupsBegin)
    {
        const int32_t bytes = static_cast<int32_t>(
            CSF_AT(uint32_t, self, 0x0C) - groupsBegin);
        const int32_t scaled = static_cast<int32_t>(
            (static_cast<int64_t>(bytes) * 0x38E38E39) >> 32) >> 4;
        groupCount = static_cast<uint32_t>(scaled) +
                     (static_cast<uint32_t>(scaled) >> 31);
    }

    uint32_t groupOffset = 0;
    for (uint32_t groupIndex = 0;
         groupIndex < groupCount && !finished;
         ++groupIndex, groupOffset += 72)
    {
        auto* group = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 0x08) + groupOffset);

        const int32_t requiredId = CSF_AT(int32_t, filter, 0x08);
        bool matches;
        if (requiredId == -1)
        {
            switch (CSF_AT(uint32_t, group, 0x20))
            {
            case 0:
                matches = (CSF_AT(uint8_t, filter, 0x00) & 1) != 0;
                break;
            case 1:
                matches = ((CSF_AT(uint32_t, filter, 0x00) >> 1) & 1) != 0;
                break;
            case 2:
                matches = ((CSF_AT(uint32_t, filter, 0x00) >> 2) & 1) != 0;
                break;
            case 3:
                matches = ((CSF_AT(uint32_t, filter, 0x00) >> 3) & 1) != 0;
                break;
            case 4:
                matches = ((CSF_AT(uint32_t, filter, 0x00) >> 4) & 1) != 0;
                break;
            default:
                continue;
            }
        }
        else
        {
            matches = CSF_AT(int32_t, group, 0x00) == requiredId;
        }

        if (!matches)
            continue;

        const uint32_t pointsBegin = CSF_AT(uint32_t, group, 0x38);
        uint32_t pointCount = 0;
        if (pointsBegin)
        {
            const int32_t bytes = static_cast<int32_t>(
                CSF_AT(uint32_t, group, 0x3C) - pointsBegin);
            const int32_t scaled = static_cast<int32_t>(
                (static_cast<int64_t>(bytes) * 0x2E8BA2E9) >> 32) >> 3;
            pointCount = static_cast<uint32_t>(scaled) +
                         (static_cast<uint32_t>(scaled) >> 31);
        }

        uint32_t pointIndex = 0;
        uint32_t pointOffset = 0;
        if (static_cast<int32_t>(pointCount) >= 4)
        {
            do
            {
                for (uint32_t lane = 0; lane < 4 && !finished; ++lane)
                {
                    auto* point = reinterpret_cast<uint8_t*>(
                        CSF_AT(uint32_t, group, 0x38) +
                        pointOffset + 44 * lane);
                    const float dx = CSF_AT(float, point, 0x04) - position[0];
                    const float dy = CSF_AT(float, point, 0x08) - position[1];
                    const float dz = CSF_AT(float, point, 0x0C) - position[2];
                    const float distance = dz * dz + dx * dx + dy * dy;

                    const bool improves = lane == 3
                        ? !(distance >= bestDistance)
                        : distance < bestDistance;
                    if (improves)
                    {
                        bestDistance = distance;
                        if (lane == 1 || lane == 3)
                        {
                            slot[1] = pointIndex + lane;
                            slot[0] = groupIndex;
                        }
                        else
                        {
                            slot[0] = groupIndex;
                            slot[1] = pointIndex + lane;
                        }
                        finished = CSF_AT(uint8_t, filter, 0x04) == 0;
                    }
                }
                pointOffset += 176;
                pointIndex += 4;
            }
            while (!finished && pointIndex < pointCount - 3);
        }

        while (!finished && pointIndex < pointCount)
        {
            auto* point = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, group, 0x38) + pointOffset);
            const float dx = CSF_AT(float, point, 0x04) - position[0];
            const float dy = CSF_AT(float, point, 0x08) - position[1];
            const float dz = CSF_AT(float, point, 0x0C) - position[2];
            const float distance = dz * dz + dx * dx + dy * dy;

            if (distance < bestDistance)
            {
                bestDistance = distance;
                slot[0] = groupIndex;
                slot[1] = pointIndex;
                finished = CSF_AT(uint8_t, filter, 0x04) == 0;
            }
            pointOffset += 44;
            ++pointIndex;
        }
    }

    return slot[0] != static_cast<unsigned int>(-1);
}

HOOK(0x0075C7C0, BddWaypointGraph_LookupSlotForVector, "auto");
