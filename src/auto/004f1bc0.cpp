#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, SceneLightObjects_StepBase, 0x004F0CB0, (void* self, void* edx, float a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fClampedCountdown, 0x0083EF0C);

static void __fastcall C_0083f040_AdvanceCountdown(C_0083f040* self, void* /*edx*/, float deltaTime)
{
    self->field_0044 = self->field_0044 - deltaTime;
    SceneLightObjects_StepBase(self, nullptr, deltaTime);

    if (self->field_0040)
    {
        C_00833908* owner = reinterpret_cast<C_00833908*>(self->field_0008);
        void* linked = CSF_AT(void*, owner, 0x34);
        if (linked)
        {
            if (CSF_AT(uint8_t, linked, 0x39)
                && (CSF_AT(uint8_t, owner, 0x1F0) & 8) == 0
                && self->field_0044 < g_fZero)
            {
                self->field_0044 = g_fClampedCountdown;
            }
        }
    }
}

HOOK(0x004F1BC0, C_0083f040_AdvanceCountdown, "auto");
