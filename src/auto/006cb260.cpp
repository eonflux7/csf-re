#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static bool __fastcall GfxScene_IsBoundingBoxInsidePlanes(
    float* self, void* /*edx*/, float* minimum, float* maximum)
{
    if (!CSF_AT(uint8_t, self, 0x6A))
        return false;

    const float centerX = (minimum[0] + maximum[0]) * g_HalfDurationScale;
    const float centerY = (minimum[1] + maximum[1]) * g_HalfDurationScale;
    const float centerZ = (minimum[2] + maximum[2]) * g_HalfDurationScale;

    const float extentX = maximum[0] - centerX;
    const float extentY = maximum[1] - centerY;
    const float extentZ = maximum[2] - centerZ;

    const float* plane = self + 7;
    const float* extentCoefficients = self + 47;

    for (int planeIndex = 0; planeIndex < 5; ++planeIndex)
    {
        const float distance =
            centerZ * plane[1] +
            centerX * plane[-1] +
            centerY * plane[0] -
            plane[2] -
            (extentY * extentCoefficients[-1] +
             extentX * extentCoefficients[-2] +
             extentZ * extentCoefficients[0]);

        if (distance < g_fZero)
            return false;

        plane += 4;
        extentCoefficients += 3;
    }

    return true;
}

HOOK(0x006CB260, GfxScene_IsBoundingBoxInsidePlanes, "auto");
