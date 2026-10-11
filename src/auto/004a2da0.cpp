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

// Returns 1 when `value` is at or above every threshold selected by `mode`:
//   mode 0: field_0010, field_0020 and field_0030
//   mode 1: field_0010 and field_0020
//   mode 2: field_0010 and field_0030
//   any other mode: 0
static char __fastcall C_008344f0_CheckValueAgainstThresholdsForMode(C_008344f0* self, void* /*edx*/, int mode, unsigned int value)
{
    char result = 0;

    if (mode == 0)
    {
        if (value >= self->field_0010 && value >= self->field_0020 && value >= self->field_0030)
            return 1;
    }
    else if (mode == 1)
    {
        if (value >= self->field_0010 && value >= self->field_0020)
            return 1;
    }
    else if (mode == 2)
    {
        if (value >= self->field_0010 && value >= self->field_0030)
            return 1;
    }
    else
    {
        return result;
    }

    return 0;
}

HOOK_KEEP_ECX(0x004A2DA0, C_008344f0_CheckValueAgainstThresholdsForMode, 2, "auto");
