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

CSF_FUNC(char, __fastcall, C_0083b668_UpdateBase, 0x004D01C0, (C_0083b668* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, C_0083ac40_RunIdlePhase, 0x004CE040, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0083ac40_RunPhaseTwo, 0x004CE400, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_0083ac40_RunPhaseThree, 0x004CE460, (void* self, void* edx, float a2));

static char __fastcall C_0083ac40_UpdateTargetingState(C_0083ac40* self, void* /*edx*/, int a2)
{
    // Inactive objects do nothing; al still holds field_0038 == 0.
    if (self->field_0038 == 0)
        return 0;

    // The incoming dword is a float time step.
    float timeStep;
    std::memcpy(&timeStep, &a2, sizeof(timeStep));

    C_0083b668_UpdateBase(self, nullptr, timeStep);

    if (self->field_0070 == 2)
        C_0083ac40_RunPhaseTwo(self, nullptr, a2);
    else if (self->field_0070 == 3)
        C_0083ac40_RunPhaseThree(self, nullptr, timeStep);

    uint32_t phase = self->field_0074;
    if (phase == 0)
        return C_0083ac40_RunIdlePhase(self, nullptr, a2);

    if (phase == 1)
    {
        if (self->field_00e4 == 0)
            return 0;

        // vtable slot 0xD8 / 4 = 54: vIsEntryTargetableDefault
        char targetable = ((char(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);
        if (!targetable)
            return 0;

        self->field_0074 = 0;
        return targetable;
    }

    // Any other phase: the original returns (phase - 1) in the low byte.
    return static_cast<char>(phase - 1);
}

HOOK(0x004CE520, C_0083ac40_UpdateTargetingState, "auto");
