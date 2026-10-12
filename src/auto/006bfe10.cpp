#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, RefreshRotationFrame, 0x006BFC70, (void* self, void* edx));

CSF_GLOBAL(float, g_FullRotationDegrees, 0x00830D04);
CSF_GLOBAL(float, g_PositiveHalfRotationDegrees, 0x00830D08);
CSF_GLOBAL(float, g_NegativeHalfRotationDegrees, 0x0083F18C);

// Adds a rotation delta to the stored angle (field_0018), wraps it into [-180, 180),
// then refreshes the frame and returns that call's result.
static char __fastcall C_00860700_AdvanceRotationAndNormalize(C_00860700* self, void* /*edx*/, float deltaDegrees)
{
    self->field_0018 = deltaDegrees + self->field_0018;

    if (self->field_0018 >= g_PositiveHalfRotationDegrees) {
        float wrapped = self->field_0018;
        do {
            wrapped = wrapped - g_FullRotationDegrees;
        } while (wrapped >= g_PositiveHalfRotationDegrees);
        self->field_0018 = wrapped;
    }

    if (self->field_0018 < g_NegativeHalfRotationDegrees) {
        float wrapped = self->field_0018;
        do {
            wrapped = wrapped + g_FullRotationDegrees;
        } while (wrapped < g_NegativeHalfRotationDegrees);
        self->field_0018 = wrapped;
    }

    return RefreshRotationFrame(self, nullptr);
}

HOOK(0x006BFE10, C_00860700_AdvanceRotationAndNormalize, "auto");
