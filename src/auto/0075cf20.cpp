#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the address of the 44-byte point record `a2[1]` inside group `*a2`,
// or 0 when either index is out of range or the group has no records.
static int __fastcall WaypointGraph_GetGroupPointRecord(C_0086c288* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t groupsBegin = self->field_0008;
    if (groupsBegin == 0)
        return 0;

    int32_t groupCount = (int32_t)(self->field_000c - groupsBegin) / 72;
    if (*a2 >= (uint32_t)groupCount)
        return 0;

    uint8_t* group = reinterpret_cast<uint8_t*>(groupsBegin) + 72 * *a2;

    uint32_t pointsBegin = CSF_AT(uint32_t, group, 0x38);
    uint32_t pointCount = 0;
    if (pointsBegin != 0)
        pointCount = (uint32_t)((int32_t)(CSF_AT(uint32_t, group, 0x3C) - pointsBegin) / 44);

    uint32_t pointIndex = a2[1];
    if (pointIndex >= pointCount)
        return 0;

    return (int)(pointsBegin + 44 * pointIndex);
}

HOOK(0x75CF20, WaypointGraph_GetGroupPointRecord, "auto");
