#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x82D4CC);
CSF_GLOBAL(float, g_fEpsilon, 0x830674);
CSF_FUNC(double, __cdecl, sub_6AEE40, 0x006AEE40, (float* a1, float* a2));

// Returns the angle of the 2D vector (x, z) against the -Z axis, signed by x.
// Returns 0 when the vector length is at or below epsilon.
static double __stdcall Hud_GetSignedVectorAngle(float* vec) {
    float x = vec[0];
    float z = vec[2];
    float dirBasis[2] = {0.0f, -1.0f};

    float length = std::sqrt(z * z + x * x);
    if (std::fabs(-length) <= g_fEpsilon) {
        return static_cast<double>(g_fZero);
    }

    float xCopy = x;
    if (x < g_fZero) {
        return -sub_6AEE40(&xCopy, dirBasis);
    }
    return sub_6AEE40(&xCopy, dirBasis);
}

HOOK(0x0042DDD0, Hud_GetSignedVectorAngle, "auto");
