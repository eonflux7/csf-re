#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint64_t, __fastcall, ReadHighResolutionCounter, 0x0069C9B0, (void* self, void* edx));
CSF_GLOBAL(double, g_FrameRateTimeScale, 0x0085E910);

static double __fastcall C_0085e92c_GetScaledElapsedTime(C_0085e92c* self, void* edx)
{
    uint64_t sample;
    if (self->field_0020 != 0)
    {
        sample = uint64_t(self->field_0028)
               | (uint64_t(self->field_002c) << 32);
    }
    else
    {
        // The original leaves EDX unchanged on entry to this call.
        sample = ReadHighResolutionCounter(self, edx);
    }

    const uint64_t previous = uint64_t(self->field_0010)
                            | (uint64_t(self->field_0014) << 32);
    const double elapsed = static_cast<double>(static_cast<int64_t>(sample - previous));
    self->field_0014 = static_cast<uint32_t>(sample >> 32);

    const double scaledElapsed = elapsed * g_FrameRateTimeScale;
    const double frequency = static_cast<double>(static_cast<int64_t>(self->field_0018));
    self->field_0010 = static_cast<uint32_t>(sample);
    return scaledElapsed / frequency;
}

HOOK(0x0069CAC0, C_0085e92c_GetScaledElapsedTime, "auto");
