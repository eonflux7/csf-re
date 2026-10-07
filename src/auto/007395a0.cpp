#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_UnitFactorLowerBound, 0x00867434);
CSF_GLOBAL(float, g_UnitFactorUpperBound, 0x00846178);

static void __fastcall C_00867444_SetFactorsAndUpdateUnitFlag(
    C_00867444* self, void* /*edx*/, float* factors)
{
    if (self->field_0054 != factors[0] ||
        self->field_0058 != factors[1])
    {
        self->field_0054 = factors[0];
        const bool firstAboveLowerBound =
            self->field_0054 > g_UnitFactorLowerBound;

        self->field_0058 = factors[1];

        self->field_005e = static_cast<uint8_t>(
            firstAboveLowerBound &&
            self->field_0058 > g_UnitFactorLowerBound &&
            self->field_0054 < g_UnitFactorUpperBound &&
            self->field_0058 < g_UnitFactorUpperBound);
    }
}

HOOK(0x007395A0, C_00867444_SetFactorsAndUpdateUnitFlag, "auto");
