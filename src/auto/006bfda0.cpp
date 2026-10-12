#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, RefreshRotationFrame, 0x006BFC70, (void* self, void* edx));

CSF_GLOBAL(float, g_FullRotationDegrees, 0x00830D04);
CSF_GLOBAL(float, g_PositiveHalfRotationDegrees, 0x00830D08);
CSF_GLOBAL(float, g_NegativeHalfRotationDegrees, 0x0083F18C);

// Adds a rotation delta to the stored angle, wraps it into [-180, 180),
// then refreshes the frame and returns that call's result.
static char __fastcall C_00860700_AddRotationAndNormalize(C_00860700* self, void* /*edx*/, float deltaDegrees)
{
    self->field_0014 = deltaDegrees + self->field_0014;

    if (self->field_0014 >= g_PositiveHalfRotationDegrees) {
        float wrapped = self->field_0014;
        do {
            wrapped = wrapped - g_FullRotationDegrees;
        } while (wrapped >= g_PositiveHalfRotationDegrees);
        self->field_0014 = wrapped;
    }

    if (self->field_0014 < g_NegativeHalfRotationDegrees) {
        float wrapped = self->field_0014;
        do {
            wrapped = wrapped + g_FullRotationDegrees;
        } while (wrapped < g_NegativeHalfRotationDegrees);
        self->field_0014 = wrapped;
    }

    return RefreshRotationFrame(self, nullptr);
}

HOOK(0x006BFDA0, C_00860700_AddRotationAndNormalize, "auto");
