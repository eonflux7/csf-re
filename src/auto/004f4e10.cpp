#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_FullRotationDegrees, 0x830D04);
CSF_GLOBAL(float, g_PositiveHalfRotationDegrees, 0x830D08);
CSF_GLOBAL(float, g_NegativeHalfRotationDegrees, 0x83F18C);

// Wraps the difference (a1 - a2) into the [-180, 180] degree range by adding or
// subtracting full turns. Stack args: 2 floats, callee pops 8 bytes (__stdcall).
static double __stdcall CharacterModel_WrapAngleDifferenceDegrees(float a1, float a2)
{
    double result = a1 - a2;

    if (a1 <= a2)
    {
        while (result < g_NegativeHalfRotationDegrees)
            result = result + g_FullRotationDegrees;
    }
    else
    {
        while (result > g_PositiveHalfRotationDegrees)
            result = result - g_FullRotationDegrees;
    }

    return result;
}

HOOK(0x4F4E10, CharacterModel_WrapAngleDifferenceDegrees, "auto");
