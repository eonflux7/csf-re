#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x82D4CC);
CSF_GLOBAL(float, g_HudNegativePi, 0x82D75C);
CSF_GLOBAL(float, g_fPi, 0x83AB28);
CSF_GLOBAL(float, g_fTwoPi, 0x83AB2C);

// Wraps the angular difference (angle - center) into [-pi, pi]. If it lies within
// +/- halfWidth the angle itself is returned; otherwise the result is clamped to the
// nearer edge of the arc (center + halfWidth or center - halfWidth).
static double __cdecl SearchSweep_ClampAngleToArc(float angle, float center, float halfWidth)
{
    double delta = angle - center;

    if (halfWidth < g_fZero) {
        halfWidth = -halfWidth;
    }

    if (delta >= g_HudNegativePi) {
        if (delta > g_fPi) {
            delta = delta - g_fTwoPi;
        }
    } else {
        delta = delta + g_fTwoPi;
    }

    if (delta > halfWidth) {
        return center + halfWidth;
    }
    if (delta >= -halfWidth) {
        return angle;
    }
    return center - halfWidth;
}

HOOK(0x004CD490, SearchSweep_ClampAngleToArc, "auto");
