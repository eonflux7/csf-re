#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// CRT asin: argument and result travel on the x87 stack (st0).
CSF_FUNC(double, __cdecl, CIasin, 0x7AF8F0, (double x));

CSF_GLOBAL(float, g_MissingMetricValue, 0x82D4C4);
CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);

static long double __cdecl GfxMatrix_ComputeInverseSine(float a1)
{
    // Lower clamp: values below the bound return asin(-1).
    if (a1 < g_MissingMetricValue)
        return CIasin(-1.0);

    // Upper clamp: values above the bound are pinned to 1.
    float clampedInput = a1;
    if (a1 > g_TransitionOne)
        clampedInput = 1.0f;

    return CIasin(clampedInput);
}

HOOK(0x411FB0, GfxMatrix_ComputeInverseSine, "auto");
