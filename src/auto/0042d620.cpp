#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_HalfTurnRadians, 0x00830668);
CSF_GLOBAL(float, g_FullTurnRadians, 0x0083066C);
CSF_GLOBAL(float, g_NegativeHalfTurnRadians, 0x0082D75C);

// Wraps the difference a1 - a2 into the range [-pi, pi] by adding or
// subtracting one full turn (2*pi) when it overshoots either end.
static double __cdecl Hud_WrapAngleDifference(float a1, float a2)
{
    double result = a1 - a2;
    if (result > g_HalfTurnRadians)
        return result - g_FullTurnRadians;
    if (result < g_NegativeHalfTurnRadians)
        return result + g_FullTurnRadians;
    return result;
}

HOOK(0x0042D620, Hud_WrapAngleDifference, "auto");
