#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angleRadians));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fRadToDeg, 0x00830D18);
CSF_GLOBAL(float, g_fDegToRad, 0x00830D1C);
CSF_GLOBAL(float, g_fMinStepPos, 0x00830DC0);
CSF_GLOBAL(float, g_fMinStepNeg, 0x00830DC4);

// Rotates an angle (degrees) toward a target by a clamped step; writes the delta
// (degrees) to *outDelta and returns the new angle in degrees.
static double __cdecl Minimap_StepAngleTowards(float currentDeg, float targetDeg, float* outDelta)
{
    float speedScale = CSF_AT(float, reinterpret_cast<void*>(GetGlobalState_50D190()), 156);
    float target = targetDeg * g_fDegToRad;
    float current = currentDeg * g_fDegToRad;
    int globalValue = GetGlobalValue_8BC1E0();

    double diff = AngleDiffNormalized(target, current);
    float step = (float)(diff * (double)(speedScale * CSF_AT(float, reinterpret_cast<void*>(globalValue), 2656)));

    if (step >= g_fZero || step <= g_fMinStepNeg) {
        if (step > g_fZero && step < g_fMinStepPos) {
            step = 0.0087266462f;
        }
    } else {
        step = -0.0087266462f;
    }

    float finalAngle;
    if ((current <= target || step + current >= target) &&
        (current >= target || step + current <= target)) {
        double r = MenuMpCharacterSelect_NormalizeAngle(step);
        *outDelta = (float)(r * g_fRadToDeg);
        finalAngle = step + current;
    } else {
        double r = MenuMpCharacterSelect_NormalizeAngle(target - current);
        *outDelta = (float)(r * g_fRadToDeg);
        finalAngle = target;
    }
    double r = MenuMpCharacterSelect_NormalizeAngle(finalAngle);
    return r * g_fRadToDeg;
}

HOOK(0x0043AD70, Minimap_StepAngleTowards, "auto");
