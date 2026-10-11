#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static long double __cdecl GfxMatrix_CalculateTriangleArea(
    float* a, float* b, float* c)
{
    const double abX = static_cast<double>(a[0]) - b[0];
    const double abY = static_cast<double>(a[1]) - b[1];
    const double abZ = static_cast<double>(a[2]) - b[2];
    const double ab = std::sqrt(abZ * abZ + abX * abX + abY * abY);

    const double bcX = static_cast<double>(b[0]) - c[0];
    const double bcY = static_cast<double>(b[1]) - c[1];
    const double bcZ = static_cast<double>(b[2]) - c[2];
    const double bc = std::sqrt(bcZ * bcZ + bcY * bcY + bcX * bcX);

    const double caX = static_cast<double>(c[0]) - a[0];
    const double caY = static_cast<double>(c[1]) - a[1];
    const double caZ = static_cast<double>(c[2]) - a[2];
    const double ca = std::sqrt(caZ * caZ + caY * caY + caX * caX);

    const double semiperimeter = (ca + bc + ab) * g_HalfDurationScale;
    return std::sqrt(
        (semiperimeter - ca) *
        (semiperimeter - bc) *
        (semiperimeter - ab) *
        semiperimeter);
}

HOOK(0x006AD4A0, GfxMatrix_CalculateTriangleArea, "auto");
