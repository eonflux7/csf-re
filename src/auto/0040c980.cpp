#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// CRT acos: argument and result travel on the x87 stack (st0).
CSF_FUNC(double, __cdecl, CIacos, 0x7AF7A0, (double x));

CSF_GLOBAL(float, g_MissingMetricValue, 0x82D4C4);
CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);

static long double __cdecl GfxMatrix_ComputeArcCosine(float a1)
{
    // Lower clamp: values below -1 return acos(-1).
    if (a1 < g_MissingMetricValue)
        return CIacos(-1.0);

    // Upper clamp: values above 1 are pinned to 1.
    float clampedInput = a1;
    if (a1 > g_TransitionOne)
        clampedInput = 1.0f;

    return CIacos(clampedInput);
}

HOOK(0x40C980, GfxMatrix_ComputeArcCosine, "auto");
