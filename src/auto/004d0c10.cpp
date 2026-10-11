#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_4D0AA0, 0x004D0AA0, (void* self, void* edx));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Subtracts the elapsed time from the countdown (field_0040). When it reaches zero or below,
// the countdown is reset to 1.0. If the owner's expiry check (sub_4D0AA0) reports success,
// the countdown is set to 0.5 and the mode (field_0044) is set to 2.
static void __fastcall C_0083b770_TickCountdown(C_0083b770* self, void* /*edx*/, float elapsedTime)
{
    float remainingTime = self->field_0040 - elapsedTime;
    self->field_0040 = remainingTime;
    if (remainingTime <= g_fZero)
    {
        self->field_0040 = 1.0f;
        if (sub_4D0AA0(self, nullptr))
        {
            self->field_0040 = 0.5f;
            self->field_0044 = 2;
        }
    }
}

HOOK(0x004D0C10, C_0083b770_TickCountdown, "auto");
