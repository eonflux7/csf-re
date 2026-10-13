#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns 0 unless field_02c4 == field_01f8 and field_0330 is zero.
// In that case compares field_02dc with field_02d6: equal -> 3, otherwise 2 - (a < b) -> 1 or 2.
static int __fastcall S_00747f20_GetComparisonStatus(S_00747f20* self, void* /*edx*/)
{
    if (self->field_02c4 == self->field_01f8 && !self->field_0330)
    {
        uint16_t v2 = self->field_02dc;
        uint16_t v3 = self->field_02d6;
        if (v2 == v3)
            return 3;
        return 2 - (v2 < v3);
    }
    return 0;
}

HOOK(0x747F20, S_00747f20_GetComparisonStatus, "auto");
