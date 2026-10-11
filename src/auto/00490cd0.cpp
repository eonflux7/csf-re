#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_FUNC(int, __fastcall, C_00833cf8_OnTimerExpired, 0x0048FBF0, (void* self, void* edx));

static void __fastcall C_00833cf8_UpdateTimer(C_00833cf8* self, void* /*edx*/, float elapsed)
{
    if ((self->field_0170 & 0x800) != 0)
    {
        float remaining;
        std::memcpy(&remaining, &self->field_00bc, sizeof(remaining));
        if (remaining > g_fZero)
        {
            remaining -= elapsed;
            std::memcpy(&self->field_00bc, &remaining, sizeof(remaining));
            if (remaining <= g_fZero)
                C_00833cf8_OnTimerExpired(self, nullptr);
        }
    }
}

HOOK(0x00490CD0, C_00833cf8_UpdateTimer, "auto");
