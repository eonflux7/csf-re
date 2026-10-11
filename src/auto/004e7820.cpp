#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0083eb70_TickBaseState, 0x004E5280, (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, C_0083eb70_Rearm, 0x004E7780, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fResetValue, 0x0083E99C);

static void __fastcall C_0083eb70_DecayAndMaybeRearm(C_0083eb70* self, void* /*edx*/, float a2)
{
    self->field_00a0 = self->field_00a0 - a2;
    C_0083eb70_TickBaseState(self, nullptr, a2);

    if (self->field_004c)
    {
        C_00833908* v3 = reinterpret_cast<C_00833908*>(self->field_0008);
        uint8_t* v4 = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, v3, 0x34));
        if (v4)
        {
            if (CSF_AT(uint8_t, v4, 0x39) != 0
                && (CSF_AT(uint8_t, v3, 0x1F0) & 8) == 0
                && self->field_00a0 < g_fZero)
            {
                self->field_00a0 = g_fResetValue;
                C_0083eb70_Rearm(self, nullptr);
            }
        }
    }
}

HOOK(0x004E7820, C_0083eb70_DecayAndMaybeRearm, "auto");
