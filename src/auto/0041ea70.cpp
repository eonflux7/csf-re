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

CSF_FUNC(double, __cdecl, MenuMpCharacterSelect_NormalizeAngle, 0x00406950, (float angle));
CSF_FUNC(double, __cdecl, AngleDiffNormalized, 0x004068E0, (float a1, float a2));
CSF_FUNC(void, __fastcall, S_0041e820_SetSecondaryAngle, 0x0041E820, (void* self, void* edx, float a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fBackAngleLimit, 0x0082E6C0);

static void __fastcall S_0041e820_AdvanceAngleAndSnapToTarget(S_0041e820* self, void* /*edx*/, float angleStep, float angleInput)
{
    (void)CSF_VTBL(0x0041E820);

    const float newAngle = self->field_0010 + angleStep;
    self->field_0010 = newAngle;
    // the normalized input angle (st0 of the call) becomes both stored angles
    const float normalized = static_cast<float>(MenuMpCharacterSelect_NormalizeAngle(angleInput));
    self->field_0008 = normalized;
    self->field_0004 = normalized;
    S_0041e820_SetSecondaryAngle(self, nullptr, 0.0f);

    const float angleDiff = (float)AngleDiffNormalized(self->field_000c, self->field_0008);

    // Direction test depends on the byte flag at +0x000 (re-read after the call).
    const bool directionMatches = (self->field_0000 != 0)
        ? (angleDiff < g_fZero)
        : (angleDiff > g_fZero);
    const bool snapDirect = directionMatches && (std::fabs(angleDiff) < g_fBackAngleLimit);

    if (!snapDirect) {
        // Fall-through path: keep the current angle if it is still at or below the limit.
        if (self->field_0010 <= self->field_0014) {
            return;
        }
    }

    // Snap to the target angle.
    self->field_0008 = self->field_000c;
    self->field_0010 = self->field_0014;
    self->field_0004 = self->field_000c;
}

HOOK(0x0041EA70, S_0041e820_AdvanceAngleAndSnapToTarget, "auto");


