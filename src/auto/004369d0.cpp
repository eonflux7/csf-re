#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angleRadians));
CSF_FUNC(float*, __fastcall, CGuiTacticalMap_GetMapBounds, 0x004368C0, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(double, __cdecl, Vec2_ComputeAngleBetween, 0x006AEE40, (float* a1, float* a2));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_HudNegativeHalfPi, 0x0082FACC);
CSF_GLOBAL(float, g_TransitionHalfPi, 0x0082FAD0);
CSF_GLOBAL(float, g_HudPi, 0x00830A0C);
CSF_GLOBAL(float, g_HudAngleEpsilon, 0x00830A18);

static double __fastcall CGuiTacticalMap_ComputeOffMapDirection(
    CGuiTacticalMap* self, void* /*edx*/, float* position)
{
    alignas(8) uint8_t frameBytes[0x28];
    float* lower = reinterpret_cast<float*>(frameBytes);
    float* displacement = reinterpret_cast<float*>(frameBytes + 0x14);
    float* upper = reinterpret_cast<float*>(frameBytes + 0x1C);
    float* reference = reinterpret_cast<float*>(frameBytes + 0x20);

    upper[0] = 0.0f;
    upper[1] = 0.0f;
    upper[2] = 0.0f;
    lower[0] = 0.0f;
    lower[2] = 0.0f;
    CGuiTacticalMap_GetMapBounds(self, nullptr, upper, lower);

    if (lower[0] <= position[0] && position[0] <= upper[0])
    {
        if (!(position[2] > upper[2]))
            return g_HudPi;
        return g_fZero;
    }

    if (lower[2] <= position[2] && position[2] <= upper[2])
    {
        if (!(position[0] > upper[0]))
            return g_TransitionHalfPi;
        return g_HudNegativeHalfPi;
    }

    lower[3] = 0.0f;
    lower[4] = upper[2] - lower[2];

    float cornerX;
    float cornerZ;
    if (position[0] > upper[0] && position[2] > upper[2])
    {
        cornerX = upper[0];
        cornerZ = upper[2];
    }
    else if (position[0] < lower[0] && position[2] > upper[2])
    {
        cornerX = lower[0];
        cornerZ = upper[2];
    }
    else if (position[0] > upper[0] && position[2] < lower[2])
    {
        cornerX = upper[0];
        cornerZ = lower[2];
    }
    else
    {
        cornerX = lower[0];
        cornerZ = lower[2];
    }

    displacement[0] = position[0] - cornerX;
    displacement[1] = position[2] - cornerZ;
    reference[0] = lower[3];
    reference[1] = lower[4];

    double distance = std::fabs(-std::sqrt(
        displacement[1] * displacement[1] +
        displacement[0] * displacement[0]));

    float angle;
    if (distance <= g_HudAngleEpsilon)
    {
        angle = 0.0f;
    }
    else if (!(displacement[0] >= g_fZero))
    {
        angle = static_cast<float>(
            -Vec2_ComputeAngleBetween(displacement, reference));
    }
    else
    {
        angle = static_cast<float>(
            Vec2_ComputeAngleBetween(displacement, reference));
    }

    return MenuMpCharacterSelect_NormalizeAngle(angle) * g_MissingMetricValue;
}

HOOK(0x004369D0, CGuiTacticalMap_ComputeOffMapDirection, "auto");
