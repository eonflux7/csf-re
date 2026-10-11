#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(double, __cdecl, GfxMatrix_ComputeInverseSine, 0x00411FB0, (float a1));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_EulerSingularityThreshold, 0x00860090);

static float* __fastcall GfxMatrix_GetEulerAngles(
    float** self, void* /*edx*/, float* angles)
{
    const float* matrix = CSF_AT(float*, self, 0x00);
    const float m00 = CSF_AT(float, matrix, 0x00);
    const float m01 = CSF_AT(float, matrix, 0x04);
    const float m02 = CSF_AT(float, matrix, 0x08);
    const float m11 = CSF_AT(float, matrix, 0x14);
    const float negativeM21 = -CSF_AT(float, matrix, 0x24);
    const float m20 = CSF_AT(float, matrix, 0x20);
    const float m22 = CSF_AT(float, matrix, 0x28);

    const double pitch = GfxMatrix_ComputeInverseSine(negativeM21);
    const float storedPitch = static_cast<float>(pitch);
    const double pitchCosine = std::cos(pitch);

    float yaw;
    float roll;
    if (!(pitchCosine > g_EulerSingularityThreshold))
    {
        roll = 0.0f;
        yaw = static_cast<float>(std::atan2(-m02, m00));
    }
    else
    {
        const double inverseCosine = g_TransitionOne / pitchCosine;
        yaw = static_cast<float>(
            std::atan2(m20 * inverseCosine, inverseCosine * m22));
        roll = static_cast<float>(
            std::atan2(m01 * inverseCosine, inverseCosine * m11));
    }

    angles[0] = storedPitch;
    angles[1] = yaw;
    angles[2] = roll;
    return angles;
}

HOOK(0x006AE3A0, GfxMatrix_GetEulerAngles, "auto");
