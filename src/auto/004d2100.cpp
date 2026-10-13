#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x82D4CC);
CSF_GLOBAL(float, g_FullRotationDegrees, 0x830D04);

// Wraps an angle in degrees into the half-open range [0, 360).
static double __stdcall NormalizeAngleTo360(float angleDegrees)
{
    double result = angleDegrees;
    while (result < g_fZero || result >= g_FullRotationDegrees)
    {
        if (result >= g_fZero)
        {
            if (result >= g_FullRotationDegrees)
                result = result - g_FullRotationDegrees;
        }
        else
        {
            result = result + g_FullRotationDegrees;
        }
    }
    return result;
}

HOOK_KEEP_ECX(0x004D2100, NormalizeAngleTo360, 1, "auto");
