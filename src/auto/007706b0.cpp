#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_FloatMax, 0x00831090);

static double __fastcall SoundAreas_GetMinimumPointY(void* self, void* /*edx*/)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x24);
    double minimum = g_FloatMax;

    int32_t pointCount = 0;
    if (begin)
    {
        const uint32_t end = CSF_AT(uint32_t, self, 0x28);
        pointCount = static_cast<int32_t>(end - begin) / 12;
    }

    uint32_t processed = 0;
    if (pointCount >= 4)
    {
        const float* ordinate = reinterpret_cast<const float*>(
            CSF_AT(uint32_t, self, 0x24) + 16);
        uint32_t groups = (static_cast<uint32_t>(pointCount - 4) >> 2) + 1;
        processed = 4 * groups;

        do
        {
            if (minimum > ordinate[-3])
                minimum = ordinate[-3];
            if (minimum > ordinate[0])
                minimum = ordinate[0];
            if (minimum > ordinate[3])
                minimum = ordinate[3];
            if (minimum > ordinate[6])
                minimum = ordinate[6];

            ordinate += 12;
        } while (--groups);
    }

    if (processed < static_cast<uint32_t>(pointCount))
    {
        const float* ordinate = reinterpret_cast<const float*>(
            CSF_AT(uint32_t, self, 0x24) + 12 * processed + 4);
        uint32_t remaining = static_cast<uint32_t>(pointCount) - processed;

        do
        {
            if (minimum > *ordinate)
                minimum = *ordinate;
            ordinate += 3;
        } while (--remaining);
    }

    return minimum;
}

HOOK(0x007706B0, SoundAreas_GetMinimumPointY, "auto");
