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

CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fBlendFactor, 0x0082E660);
CSF_GLOBAL(float, g_fPi, 0x0082E568);
CSF_GLOBAL(float, g_fSmallScale, 0x0082E1E0);

// Zeroes field_0010, then stores a factor in field_0014 derived from the
// normalized angle difference between field_000c and field_0008.
// Scale factor: when a2 > 0, |diff| / (a2 * pi * small); otherwise |diff| * blend.
static void __fastcall S_0041e820_UpdateAngleFactor(S_0041e820* self, void* /*edx*/, float a2)
{
    self->field_0010 = 0.0f;
    self->field_0010 = 0.0f;

    double angleDiff = AngleDiffNormalized(self->field_000c, self->field_0008);

    if (a2 <= g_fZero) {
        self->field_0014 = static_cast<float>(std::fabs(angleDiff) * g_fBlendFactor);
    } else {
        self->field_0014 = static_cast<float>(std::fabs(angleDiff) / (a2 * g_fPi * g_fSmallScale));
    }
}

HOOK(0x0041E820, S_0041e820_UpdateAngleFactor, "auto");
