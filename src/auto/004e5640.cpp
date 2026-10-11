#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(double, __fastcall, S_00526d40_GetVectorAngleRadians, 0x006AB930, (S_00526d40* self, void*));
CSF_FUNC(double, __fastcall, S_00526d40_NormalizeCopyAndReportY, 0x006AB9E0, (S_00526d40* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fAngleScale, 0x0083E9A8);

// The virtual getters return their value on the FPU stack; it is used unrounded.
typedef double(__fastcall* FpuGetterFn)(S_004e5640*, void*);

// x87 status word produced by comparing a with b (C0 = 0x100, C2 = 0x400, C3 = 0x4000)
static unsigned FpuCompareStatus(double a, double b)
{
    if (a != a || b != b)
        return 0x4500;
    if (a < b)
        return 0x0100;
    if (a == b)
        return 0x4000;
    return 0;
}

static bool __fastcall S_004e5640_IsTargetWithinAngleAndRange(S_004e5640* self, void* /*edx*/, float a2, S_00526d40* a3)
{
    FpuGetterFn getAngleLimit = (FpuGetterFn)CSF_AT(void**, self, 0)[31];
    float maxAngle = (float)(getAngleLimit(self, nullptr) * g_fAngleScale);

    float angle = (float)S_00526d40_GetVectorAngleRadians(a3, nullptr);
    double diff = AngleDiffNormalized(angle, a2);

    // test ah,5 / jp : only a strict "less than" passes
    unsigned status = FpuCompareStatus(std::fabs(diff), maxAngle);
    if ((status & 0x0500) != 0x0100 && (status & 0x0500) != 0x0400)
        return false;

    // The unrounded result is compared with zero; a float copy is kept for the range test.
    double lengthFull = S_00526d40_NormalizeCopyAndReportY(a3, nullptr);
    float length = (float)lengthFull;
    status = FpuCompareStatus(lengthFull, g_fZero);
    if ((status & 0x4100) != 0)
        return true;

    FpuGetterFn getRange = (FpuGetterFn)CSF_AT(void**, self, 0)[32];
    double maxRange = getRange(self, nullptr) * g_fAngleScale;

    status = FpuCompareStatus(maxRange, length);
    return (status & 0x4100) == 0;
}

HOOK(0x004E5640, S_004e5640_IsTargetWithinAngleAndRange, "auto");
