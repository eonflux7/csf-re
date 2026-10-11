#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_MissingMetricValue, 0x82D4C4);
CSF_GLOBAL(float, g_fZero, 0x82D4CC);

static char __fastcall C_00830578_ComputeAxisOffsets(C_00830578* self, void* /*edx*/, float* outOffsets)
{
    outOffsets[0] = 0.0f;
    outOffsets[1] = 0.0f;

    if (self->field_002c != g_fZero)
        outOffsets[1] = -(self->field_0038 * self->field_002c);

    if (self->field_0030 != g_fZero)
        outOffsets[0] = self->field_0034 * self->field_0030;

    char flipSign = static_cast<char>(self->field_003d);
    if (flipSign)
        outOffsets[0] = outOffsets[0] * g_MissingMetricValue;

    return flipSign;
}

HOOK(0x42B150, C_00830578_ComputeAxisOffsets, "auto");
