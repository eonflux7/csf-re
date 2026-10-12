#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_004af750_EvaluateGuardWaypoint, 0x004AF750, (S_004af750* self, void* , float* position));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_TestCoverPosition, 0x004E5C70, (C_0083e9d0* self, void* , float* arg0, int a2, int a4));

static char __fastcall S_004af750_CheckSlotPosition(
    S_004af750* self, void* /*edx*/, float* position, int slot)
{
    if (CSF_AT(int, reinterpret_cast<void*>(slot), 0x08) < 0)
        return 1;

    auto* cover = CSF_AT(C_0083e9d0*,
        reinterpret_cast<void*>(self->field_002c), 0x3C);
    if (static_cast<uint8_t>(
            AiCoverAttackTasks_TestCoverPosition(cover, nullptr, position, slot, -1)))
        return 0;

    auto* waypoint = reinterpret_cast<uint8_t*>(
        S_004af750_EvaluateGuardWaypoint(self, nullptr, position));
    if (!waypoint)
        return 0;

    auto* candidate = waypoint + 0x14;
    for (unsigned index = 0; index < 5; ++index, candidate += 12)
    {
        if (CSF_AT(int, waypoint + index * 4, 0) == 1)
        {
            cover = CSF_AT(C_0083e9d0*,
                reinterpret_cast<void*>(self->field_002c), 0x3C);
            if (static_cast<uint8_t>(AiCoverAttackTasks_TestCoverPosition(
                    cover, nullptr, reinterpret_cast<float*>(candidate), slot, -1)))
                return 0;
        }
    }
    return 1;
}

HOOK(0x004B2EE0, S_004af750_CheckSlotPosition, "auto");
