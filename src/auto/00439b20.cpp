#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angleRadians));

CSF_GLOBAL(float, g_DegreesToRadiansFactor, 0x830CA8);
CSF_GLOBAL(float, g_RadiansToDegreesFactor, 0x830CA4);
CSF_GLOBAL(float, g_NearZeroEpsilon, 0x830CAC);
CSF_GLOBAL(float, g_fZero, 0x82D4CC);
CSF_GLOBAL(float, g_MissingMetricValue, 0x82D4C4);
CSF_GLOBAL(float, g_FullRotationDegrees, 0x830D04);
CSF_GLOBAL(float, g_PositiveHalfRotationDegrees, 0x830D08);

// Converts an angle in degrees to radians, normalizes it through the game's
// angle normalizer, converts back to degrees and folds the result into the
// [0, 360) range with the sign/zero special cases of the original.
static double __cdecl NormalizeDegreesAngle(float angleDegrees)
{
    double angleRadians = angleDegrees * g_DegreesToRadiansFactor;
    double normalizedRadians = MenuMpCharacterSelect_NormalizeAngle((float)angleRadians);
    double degrees = normalizedRadians * g_RadiansToDegreesFactor;

    if (std::fabs(-degrees) <= g_NearZeroEpsilon)
        return g_fZero;

    if (degrees < g_fZero)
        return degrees * g_MissingMetricValue;

    double folded = (g_PositiveHalfRotationDegrees - degrees) + g_PositiveHalfRotationDegrees;
    if (folded == g_FullRotationDegrees)
        return g_fZero;

    return folded;
}

HOOK(0x439B20, NormalizeDegreesAngle, "auto");
