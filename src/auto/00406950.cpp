#include "globals.h"
#include "hook.h"

// Pi, 2*Pi and -Pi as float constants stored in the game's data section.
CSF_GLOBAL(float, g_PiConstant, 0x0082D748);
CSF_GLOBAL(float, g_TwoPiConstant, 0x0082D74C);
CSF_GLOBAL(float, g_HudNegativePi, 0x0082D75C);

// Wraps an angle in radians into the range [-Pi, Pi] by repeatedly adding or
// subtracting a full turn (2*Pi) until it lies inside the interval.
static double MenuMpCharacterSelect_NormalizeAngle(float angleRadians)
{
    double result = angleRadians;

    while (result > g_PiConstant)
    {
        result = result - g_TwoPiConstant;
    }

    while (result < g_HudNegativePi)
    {
        result = result + g_TwoPiConstant;
    }

    return result;
}

HOOK(0x00406950, MenuMpCharacterSelect_NormalizeAngle, "auto");
