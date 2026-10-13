#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_MissingMetricValue, 0x82D4C4);
CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);

static long double __cdecl GfxMatrix_ComputeArcCosine(float input)
{
    if (input < g_MissingMetricValue)
        return std::acos(-1.0);

    float clampedInput = input;
    if (input > g_TransitionOne)
        clampedInput = 1.0f;

    return std::acos(static_cast<double>(clampedInput));
}

HOOK(0x40C980, GfxMatrix_ComputeArcCosine, "auto");
