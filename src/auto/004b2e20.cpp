#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, S_004af750_EvaluateGuardWaypoint, 0x004AF750, (S_004af750* self, void* , float* position));
static CSF_FUNC(int, __fastcall, AiCoverAttackTasks_TestCoverPosition, 0x004E5C70, (C_0083e9d0* self, void* , float* arg0, int a2, int a4));

static char __fastcall S_004af750_TestAttackPosition(
    S_004af750* self, void* /*edx*/, float* position, int target)
{
    if (CSF_AT(int, reinterpret_cast<void*>(target), 0x08) < 0)
        return 0;

    auto* waypoint = reinterpret_cast<std::uint8_t*>(
        S_004af750_EvaluateGuardWaypoint(self, nullptr, position));
    if (!waypoint)
        return 0;

    for (unsigned index = 0; index < 5; ++index)
    {
        if (CSF_AT(int, waypoint + index * 4, 0) == 2)
        {
            auto* owner = reinterpret_cast<void*>(self->field_002c);
            auto* tasks = CSF_AT(C_0083e9d0*, owner, 0x3C);
            if (static_cast<std::uint8_t>(
                    AiCoverAttackTasks_TestCoverPosition(
                        tasks, nullptr,
                        reinterpret_cast<float*>(waypoint + 20 + index * 12),
                        target, -1)))
                return 1;
        }
    }
    return 0;
}

HOOK(0x004B2E20, S_004af750_TestAttackPosition, "auto");
