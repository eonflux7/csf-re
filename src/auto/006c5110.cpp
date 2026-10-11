#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static double __cdecl GfxSkin_GetPointBoxDistance(
    float x, float y, float z, const void* bounds)
{
    const float extentX =
        CSF_AT(float, bounds, 0x00) - CSF_AT(float, bounds, 0x0C);
    const float extentY =
        CSF_AT(float, bounds, 0x04) - CSF_AT(float, bounds, 0x10);
    const float extentZ =
        CSF_AT(float, bounds, 0x08) - CSF_AT(float, bounds, 0x14);

    const float fromMinX = x - CSF_AT(float, bounds, 0x0C);
    const float fromMinY = y - CSF_AT(float, bounds, 0x10);
    const float fromMinZ = z - CSF_AT(float, bounds, 0x14);

    const float fromMaxX = extentX - fromMinX;
    const float fromMaxY = extentY - fromMinY;
    const float fromMaxZ = extentZ - fromMinZ;

    const bool insideX = fromMaxX >= g_fZero && fromMinX >= g_fZero;
    const bool insideY = fromMaxY >= g_fZero && fromMinY >= g_fZero;
    const bool insideZ = fromMaxZ >= g_fZero && fromMinZ >= g_fZero;

    float distanceX;
    if (insideX)
    {
        if (insideY && insideZ)
            return g_MissingMetricValue;
        distanceX = 0.0f;
    }
    else
    {
        const float lowerDistance = std::fabs(fromMinX);
        const float upperDistance = std::fabs(fromMaxX);
        distanceX = upperDistance >= lowerDistance
            ? lowerDistance : upperDistance;
    }

    float distanceY;
    if (insideY)
    {
        distanceY = 0.0f;
    }
    else
    {
        const float lowerDistance = std::fabs(fromMinY);
        const float upperDistance = std::fabs(fromMaxY);
        distanceY = upperDistance >= lowerDistance
            ? lowerDistance : upperDistance;
    }

    float distanceZ;
    if (insideZ)
    {
        distanceZ = g_fZero;
    }
    else
    {
        const float lowerDistance = std::fabs(fromMinZ);
        const float upperDistance = std::fabs(fromMaxZ);
        distanceZ = upperDistance >= lowerDistance
            ? lowerDistance : upperDistance;
    }

    if (insideX)
    {
        if (insideY)
            return distanceZ;
        if (insideZ)
            return distanceY;
    }
    else if (insideY && insideZ)
    {
        return distanceX;
    }

    if (insideX)
        return std::sqrt(distanceZ * distanceZ + distanceY * distanceY);
    if (insideY)
        return std::sqrt(distanceZ * distanceZ + distanceX * distanceX);
    if (insideZ)
        return std::sqrt(distanceY * distanceY + distanceX * distanceX);

    return std::sqrt(distanceZ * distanceZ + distanceY * distanceY
                     + distanceX * distanceX);
}

HOOK(0x006C5110, GfxSkin_GetPointBoxDistance, "auto");
