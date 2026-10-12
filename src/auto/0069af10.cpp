#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);

// Reads the time source at +0x14 (vtable slot 5, returns double), converts the
// result from milliseconds to seconds, and advances the two accumulators at +0x04
// and +0x08. Stores the step in +0x10. Returns 1 and resets +0x04 once the
// accumulator at +0x04 reaches the threshold at +0x0C; otherwise returns 0.
static char __fastcall MissionScript_HasPendingState(void* self, void* /*edx*/)
{
    void* timeSource = CSF_AT(void*, self, 0x14);
    using TimeQueryFn = double(__fastcall*)(void* self, void* edx);
    double rawMilliseconds = ((TimeQueryFn)CSF_AT(void**, timeSource, 0)[5])(timeSource, nullptr);

    float elapsedSeconds = (float)rawMilliseconds * g_SceneLoadMillisecondsToSeconds;
    CSF_AT(float, self, 0x10) = elapsedSeconds;

    float accumulatedTime = elapsedSeconds + CSF_AT(float, self, 0x04);
    CSF_AT(float, self, 0x04) = accumulatedTime;
    CSF_AT(float, self, 0x08) = elapsedSeconds + CSF_AT(float, self, 0x08);

    if (accumulatedTime < CSF_AT(float, self, 0x0C))
    {
        CSF_AT(float, self, 0x04) = accumulatedTime;
        return 0;
    }

    CSF_AT(float, self, 0x04) = g_fZero;
    return 1;
}

HOOK(0x0069AF10, MissionScript_HasPendingState, "auto");
