#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// The original calls the CRT's __CIasin (0x7AF8F0) with its operand on the x87 stack (`fld dword ptr`, no
// stack argument) and returns the callee's st0. C++ cannot load st0 without assembly (the gate allows only tail-jump
// thunks), so the call is the plain std::asin on the same value, as 00740aa0 does for __CIfmod.
CSF_GLOBAL(float, g_MissingMetricValue, 0x82D4C4);
CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);

static long double __cdecl GfxMatrix_ComputeInverseSine(float a1)
{
    // Lower clamp: values below the bound return asin(-1).
    if (a1 < g_MissingMetricValue)
        return std::asin(-1.0);

    // Upper clamp: values above the bound are pinned to 1.
    float clampedInput = a1;
    if (a1 > g_TransitionOne)
        clampedInput = 1.0f;

    return std::asin(static_cast<double>(clampedInput));
}

HOOK(0x411FB0, GfxMatrix_ComputeInverseSine, "auto");
