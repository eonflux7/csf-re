#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0083b668_UpdateBase, 0x004D01C0, (C_0083b668* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, sub_4D5C60, 0x004D5C60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4D5E60, 0x004D5E60, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_4D5F40, 0x004D5F40, (void* self, void* edx, float a2));

static char __fastcall C_0083c280_UpdateTargetingState(C_0083c280* self, void* /*edx*/, int a2)
{
    // Inactive objects do nothing and return 0 (al still holds field_0038 == 0).
    if (self->field_0038 == 0)
        return 0;

    // The incoming dword is a float time step.
    float timeStep;
    std::memcpy(&timeStep, &a2, sizeof(timeStep));

    C_0083b668_UpdateBase(self, nullptr, timeStep);

    if (self->field_0070 == 2)
        sub_4D5E60(self, nullptr, a2);
    else if (self->field_0070 == 3)
        sub_4D5F40(self, nullptr, timeStep);

    uint32_t phase = self->field_0074;
    if (phase == 0)
        return sub_4D5C60(self, nullptr, a2);

    if (phase == 1)
    {
        if (self->field_00e4 == 0)
            return 0;

        // vtable slot 0xD8 / 4 = 54: vIsEntryTargetableDefault
        char targetable = ((char(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xD8 / 4])(self, nullptr);
        if (!targetable)
            return 0;

        self->field_0074 = 0;
        return 1;
    }

    // Any other phase: the original returns (phase - 1) in the low byte.
    return static_cast<char>(phase - 1);
}

HOOK(0x004D6080, C_0083c280_UpdateTargetingState, "auto");
