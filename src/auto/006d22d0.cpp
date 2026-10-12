#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TimerAccumulator, 0x008BB4F4);
CSF_GLOBAL(float, g_TimerThreshold, 0x00860DD8);

CSF_FUNC(uint32_t, __fastcall, C_00860e68_FireTimedAction, 0x006D22A0, (C_00860e68* self, void* edx));

static void __fastcall C_00860e68_AccumulateTimerAndTrigger(C_00860e68* self, void* /*edx*/, float elapsedSeconds)
{
    if (self->field_0098)
    {
        g_TimerAccumulator = g_TimerAccumulator + elapsedSeconds;
        if (g_TimerAccumulator >= g_TimerThreshold)
        {
            g_TimerAccumulator = 0.0f;
            C_00860e68_FireTimedAction(self, nullptr);
        }
    }
}

HOOK(0x006D22D0, C_00860e68_AccumulateTimerAndTrigger, "auto");
