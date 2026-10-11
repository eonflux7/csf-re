#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __cdecl GfxSkin_ComputeDistanceFade(
    float* fade,
    float* position,
    float* referencePosition,
    float distanceScale,
    float maximumDistance,
    float minimumDistance,
    float fadeWidth)
{
    const bool hasMaximum = maximumDistance > g_fZero;
    *fade = 1.0f;
    const bool hasMinimum = minimumDistance > g_fZero;

    if (!hasMaximum && !hasMinimum)
        return 1;

    const float dx = position[0] - referencePosition[0];
    const float dy = position[1] - referencePosition[1];
    const float dz = position[2] - referencePosition[2];
    const float distance =
        std::sqrt(dz * dz + dx * dx + dy * dy) * distanceScale;

    char withinRange = 1;
    if (hasMaximum)
        withinRange = distance <= maximumDistance;
    if (hasMinimum)
        withinRange &= distance >= minimumDistance;

    if (!withinRange)
        return withinRange;

    if (hasMaximum)
    {
        const float remainingDistance = maximumDistance - distance;
        if (remainingDistance < fadeWidth)
            *fade = remainingDistance / fadeWidth;
    }

    if (!hasMinimum)
        return withinRange;

    std::uint32_t fadeBits;
    std::memcpy(&fadeBits, fade, sizeof(fadeBits));
    if (fadeBits != 1065353216)
        return withinRange;

    const float remainingDistance = distance - minimumDistance;
    if (remainingDistance < fadeWidth)
        *fade = remainingDistance / fadeWidth;

    return withinRange;
}

HOOK(0x006C5310, GfxSkin_ComputeDistanceFade, "auto");
