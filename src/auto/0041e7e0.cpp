#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Normalizes an angle; the original reads the result from st0 (double return).
CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angle));

// Returns the normalized difference between two angles (st0 at return).
CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));

// Float constant used as a scale factor (0.38197187 in the original).
CSF_GLOBAL(float, g_InverseGoldenRatio, 0x0082E660);

// Stores the normalized target angle at +0x0C, zeroes +0x10, and writes
// |angle difference| * 0.38197187 to +0x14. Pops 4 bytes (one stack arg).
static int __fastcall AiTaskState_UpdateNormalizedAngle(void* self, void* /*edx*/, float angle)
{
    float normalizedAngle = (float)MenuMpCharacterSelect_NormalizeAngle(angle);
    CSF_AT(float, self, 0x0C) = normalizedAngle;
    CSF_AT(int, self, 0x10) = 0;

    double angleDiff = AngleDiffNormalized(normalizedAngle, CSF_AT(float, self, 0x08));
    CSF_AT(float, self, 0x14) = (float)(std::fabs(angleDiff) * g_InverseGoldenRatio);

    // The original leaves eax as whatever the diff callee left behind; no meaningful value.
    return 0;
}

HOOK(0x0041E7E0, AiTaskState_UpdateNormalizedAngle, "auto");
