#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(uint32_t*, __fastcall, WaypointGraph_Finalize, 0x0075F160, (void* self, void* edx));

// Resets the graph state: frees the buffer at +0x20, clears per-point field (+0x18 of each 44-byte
// entry in every 72-byte group), then calls the sub-object at +0x14.
static int __fastcall C_0086c288_ResetGraphState(C_0086c288* self, void* /*edx*/)
{
    self->field_0030 = 0;
    uint32_t buffer = self->field_0020;
    unsigned int pointIndex = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    if (buffer != 0)
    {
        Mem_Free(reinterpret_cast<void*>(buffer));
        self->field_0020 = 0;
    }
    self->field_0018 = 0;
    self->field_001c = 0;

    unsigned int groupIndex = 0;
    int groupOffset = 0;
    for (;;)
    {
        uint32_t groupsBegin = self->field_0008;
        if (groupsBegin == 0)
            break;
        int groupCount = (int)(self->field_000c - groupsBegin) / 72;
        if (groupIndex >= (unsigned int)groupCount)
            break;

        int pointOffset = 0;
        for (;;)
        {
            uint8_t* group = reinterpret_cast<uint8_t*>(self->field_0008) + groupOffset;
            uint32_t pointsBegin = CSF_AT(uint32_t, group, 0x38);
            if (pointsBegin == 0)
                break;
            int pointCount = (int)(CSF_AT(uint32_t, group, 0x3C) - pointsBegin) / 44;
            if (pointIndex >= (unsigned int)pointCount)
                break;
            uint8_t* point = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, group, 0x38)) + pointOffset;
            pointIndex++;
            CSF_AT(uint32_t, point, 0x18) = 0;
            pointOffset += 44;
        }
        groupIndex++;
        groupOffset += 72;
        pointIndex = 0;
    }

    return (int)(uintptr_t)WaypointGraph_Finalize(reinterpret_cast<void*>(self->field_0014), nullptr);
}

HOOK(0x0075D1F0, C_0086c288_ResetGraphState, "auto");
