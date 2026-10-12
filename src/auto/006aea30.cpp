#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_FUNC(double, __cdecl, CalculateArcCosine, 0x0040C980, (float a1));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static double (__cdecl* s_Sine)(double) =
    static_cast<double (__cdecl*)(double)>(&std::sin);

static float* __fastcall GfxHAnim_GetQuaternionAxis(
    float* quaternion, void* /*edx*/, float* axis)
{
    const double sine = s_Sine(CalculateArcCosine(quaternion[3]));

    if (sine == g_fZero)
    {
        axis[0] = 1.0f;
        axis[1] = 0.0f;
        axis[2] = 0.0f;
    }
    else
    {
        const double scale = g_TransitionOne / sine;

        // Read all components before writing, preserving overlapping inputs.
        const float x = static_cast<float>(scale * quaternion[0]);
        const float y = static_cast<float>(scale * quaternion[1]);
        const float z = static_cast<float>(scale * quaternion[2]);

        axis[0] = x;
        axis[1] = y;
        axis[2] = z;
    }

    return axis;
}

HOOK(0x006AEA30, GfxHAnim_GetQuaternionAxis, "auto");
