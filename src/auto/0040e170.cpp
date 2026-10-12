#include <cmath>
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(double, __fastcall, C_008342e8_ComputeResolvedObjectScalar, 0x0048F220, (C_008342e8* self, void*));

CSF_GLOBAL(float, g_DegreesToRadians, 0x0082DD5C);
CSF_GLOBAL(float, g_AngleEpsilon, 0x0082DD60);

static void __fastcall C_0082dd78_UpdateHeadingIfChanged(C_0082dd78* self, void* /*edx*/)
{
    double scalarValue = C_008342e8_ComputeResolvedObjectScalar(
        reinterpret_cast<C_008342e8*>(self->field_0004), nullptr);
    float targetHeading = (float)(scalarValue * g_DegreesToRadians);

    // The callee's cdecl return value (st0) is what the original tests with fabs.
    double headingDelta = AngleDiffNormalized(
        targetHeading, *reinterpret_cast<float*>(&self->field_0044));

    if (std::fabs(self->field_0048) + g_AngleEpsilon < std::fabs(headingDelta))
        *reinterpret_cast<float*>(&self->field_0044) = targetHeading;
}

HOOK(0x0040E170, C_0082dd78_UpdateHeadingIfChanged, "auto");
