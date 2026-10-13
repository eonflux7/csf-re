#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: int __thiscall sub_4DC8F0(this, uint32_t* a2), ret 4.
// Finds the index of the cached waypoint entry matching a2, or -1 if none.
CSF_FUNC(int, __fastcall, StaticAi_FindCachedWaypointIndex, 0x004DC8F0,
         (void* self, void* edx, uint32_t* a2));

// Returns the assessment value stored at offset +12 of the matching 16-byte
// entry in the table at this->field_0068, or 0 when no entry matches.
static int __fastcall StaticAi_GetCachedWaypointAssessment(S_004dc8f0* self, void* /*edx*/, uint32_t* a2)
{
    int index = StaticAi_FindCachedWaypointIndex(self, nullptr, a2);
    if (index == -1)
        return 0;

    uint8_t* table = reinterpret_cast<uint8_t*>(self->field_0068);
    return static_cast<int>(CSF_AT(uint32_t, table, 16 * index + 12));
}

HOOK(0x004DC980, StaticAi_GetCachedWaypointAssessment, "auto");
