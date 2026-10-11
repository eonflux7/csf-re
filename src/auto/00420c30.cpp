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

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __fastcall, CharTask_OnTimerExpired, 0x004DB440, (void* self, void* edx, int a2));

// Timer-driven state expiry: decrements the remaining time (+0x14) by dt and,
// once it drops below zero, asks the owner's controller (owner+0x14) to run its
// virtual slot 35 with (0, 0, -1, 0), then forwards the event to CharTask_OnTimerExpired.
static void __fastcall C_00834fa8_UpdateStateTimer(C_00834fa8* self, void* /*edx*/, float elapsedSeconds)
{
    float remainingTime = CSF_AT(float, self, 0x14) - elapsedSeconds;
    CSF_AT(float, self, 0x14) = remainingTime;

    if (remainingTime < g_fZero)
    {
        uint32_t owner = CSF_AT(uint32_t, self, 0x08);
        void* controller = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(owner), 0x14));

        ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, controller, 0)[140 / 4])(
            controller, nullptr, 0, 0, -1, 0);

        int stateArg = static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 32);
        CharTask_OnTimerExpired(self, nullptr, stateArg);
    }
}

HOOK(0x00420C30, C_00834fa8_UpdateStateTimer, "auto");
