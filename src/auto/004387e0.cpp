#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

static double __fastcall CalculateColorChannelIntensity(
    void* self, void* /*edx*/, int position)
{
    if (position >= CSF_AT(int32_t, self, 0xDC) &&
        position <= CSF_AT(int32_t, self, 0xE4))
        return g_ColorChannelScale;

    const int32_t start = CSF_AT(int32_t, self, 0x00);
    const int32_t length = CSF_AT(int32_t, self, 0x08);
    const int32_t end = static_cast<int32_t>(
        static_cast<uint32_t>(start) + static_cast<uint32_t>(length));

    if (position >= end || position <= start)
        return g_fZero;

    const int32_t plateauStart = CSF_AT(int32_t, self, 0xDC);
    const int32_t rampWidth = static_cast<int32_t>(
        static_cast<uint32_t>(plateauStart) - static_cast<uint32_t>(start));

    if (position < plateauStart)
    {
        const int32_t distance = static_cast<int32_t>(
            static_cast<uint32_t>(position) - static_cast<uint32_t>(start));
        return static_cast<double>(distance) * g_ColorChannelScale /
               static_cast<double>(rampWidth);
    }

    int32_t distance = 0;
    if (position > CSF_AT(int32_t, self, 0xE4))
    {
        distance = static_cast<int32_t>(
            static_cast<uint32_t>(start) - static_cast<uint32_t>(position) +
            static_cast<uint32_t>(length));
    }

    return static_cast<double>(distance) * g_ColorChannelScale /
           static_cast<double>(rampWidth);
}

HOOK(0x004387E0, CalculateColorChannelIntensity, "auto");
