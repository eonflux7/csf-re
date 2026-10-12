#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(double, __cdecl, CalculateArcCosine, 0x0040C980, (float a1));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_QuaternionSlerpEpsilon, 0x008600BC);

static void __fastcall Math_SphericalInterpolateQuaternion(
    float* self, void* /*edx*/, float* target, float fraction)
{
    float sign = 1.0f;
    float dot =
        CSF_AT(float, target, 0x08) * CSF_AT(float, self, 0x08) +
        CSF_AT(float, target, 0x04) * CSF_AT(float, self, 0x04) +
        CSF_AT(float, target, 0x00) * CSF_AT(float, self, 0x00) +
        CSF_AT(float, self, 0x0C) * CSF_AT(float, target, 0x0C);

    if (dot < g_fZero)
    {
        sign = -1.0f;
        dot = -dot;
    }

    float sourceWeight;
    float targetWeight;
    if (g_TransitionOne - dot < g_QuaternionSlerpEpsilon)
    {
        sourceWeight = g_TransitionOne - fraction;
        targetWeight = fraction;
    }
    else
    {
        float angle = static_cast<float>(CalculateArcCosine(dot));
        float inverseSine = g_TransitionOne / std::sin(angle);
        sourceWeight =
            std::sin((g_TransitionOne - fraction) * angle) * inverseSine;
        targetWeight = std::sin(angle * fraction) * inverseSine;
    }

    CSF_AT(float, self, 0x00) =
        targetWeight * CSF_AT(float, target, 0x00) * sign +
        sourceWeight * CSF_AT(float, self, 0x00);
    CSF_AT(float, self, 0x04) =
        targetWeight * CSF_AT(float, target, 0x04) * sign +
        sourceWeight * CSF_AT(float, self, 0x04);
    CSF_AT(float, self, 0x08) =
        targetWeight * CSF_AT(float, target, 0x08) * sign +
        sourceWeight * CSF_AT(float, self, 0x08);
    CSF_AT(float, self, 0x0C) =
        targetWeight * sign * CSF_AT(float, target, 0x0C) +
        sourceWeight * CSF_AT(float, self, 0x0C);
}

HOOK(0x006AEAC0, Math_SphericalInterpolateQuaternion, "auto");
