#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_FloatMax, 0x00831090);

static char __fastcall C_0083e0d8_FindNearestWaypoint(
    C_0083e0d8* self, void* /*edx*/, int groupId,
    float* position, uint32_t* result)
{
    float nearestDistance = g_FloatMax;
    char found = 0;

    const uint32_t groupsBegin = self->field_0008;
    const uint32_t groupCount = groupsBegin
        ? static_cast<int32_t>(self->field_000c - groupsBegin) / 72
        : 0;

    for (uint32_t groupIndex = 0; groupIndex < groupCount; ++groupIndex)
    {
        const uint32_t groupOffset = groupIndex * 72;
        if (groupId != -1)
        {
            auto* group = reinterpret_cast<C_00833908*>(
                self->field_0008 + groupOffset);
            if (group->field_0020 != static_cast<uint32_t>(groupId))
                continue;
        }

        auto* group = reinterpret_cast<C_00833908*>(
            self->field_0008 + groupOffset);
        const uint32_t pointsBegin = group->field_0038;
        int32_t pointCount = 0;
        if (pointsBegin)
        {
            const int32_t pointBytes =
                static_cast<int32_t>(group->field_003c - pointsBegin);
            // Original signed division by the 44-byte point stride.
            const int64_t product = int64_t{0x2E8BA2E9} * pointBytes;
            const int32_t quotient =
                static_cast<int32_t>(product >> 32) >> 3;
            pointCount = quotient +
                static_cast<int32_t>(static_cast<uint32_t>(quotient) >> 31);
        }

        for (uint32_t pointIndex = 0;
             pointIndex < static_cast<uint32_t>(pointCount);
             ++pointIndex)
        {
            // Reload storage after every possible result write.
            group = reinterpret_cast<C_00833908*>(
                self->field_0008 + groupOffset);
            auto* point = reinterpret_cast<uint8_t*>(
                group->field_0038 + pointIndex * 44);

            const float dx = CSF_AT(float, point, 0x4) - position[0];
            const float dy = CSF_AT(float, point, 0x8) - position[1];
            const float dz = CSF_AT(float, point, 0xC) - position[2];
            const float distance = dz * dz + dx * dx + dy * dy;

            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                result[0] = groupIndex;
                result[1] = pointIndex;
                found = 1;
            }
        }
    }

    return found;
}

HOOK(0x0075CB30, C_0083e0d8_FindNearestWaypoint, "auto");
