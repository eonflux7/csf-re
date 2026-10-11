#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angle));

// Angle tween: self+0x04 = start angle, +0x08 = current angle,
// +0x0C = target angle, +0x10 = elapsed time, +0x14 = duration.
static double __fastcall AiTaskStates_AdvanceAngleTween(float* self, void* /*edx*/, float a2)
{
    if (CSF_AT(float, self, 0x08) != CSF_AT(float, self, 0x0C))
    {
        a2 = a2 + CSF_AT(float, self, 0x10);
        CSF_AT(float, self, 0x10) = a2;

        if (a2 >= CSF_AT(float, self, 0x14))
        {
            // Tween finished: snap to target.
            float target = CSF_AT(float, self, 0x0C);
            CSF_AT(float, self, 0x10) = 0.0f;
            CSF_AT(float, self, 0x08) = target;
            CSF_AT(float, self, 0x04) = target;
            return (double)CSF_AT(float, self, 0x08);
        }

        // Interpolate along the shortest angular difference.
        float start = CSF_AT(float, self, 0x04);
        float target = CSF_AT(float, self, 0x0C);
        double angleDelta = AngleDiffNormalized(target, start);
        float rawAngle = (float)(angleDelta * a2 / CSF_AT(float, self, 0x14) + start);
        double normalizedAngle = MenuMpCharacterSelect_NormalizeAngle(rawAngle);
        CSF_AT(float, self, 0x08) = (float)normalizedAngle;
    }
    return (double)CSF_AT(float, self, 0x08);
}

HOOK(0x0041E490, AiTaskStates_AdvanceAngleTween, "auto");
