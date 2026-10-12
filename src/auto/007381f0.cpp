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

// Stores a new value in field_0008 unless it is unchanged, or the current value is 1
// and field_0004 is zero. Returns the previous value of field_0008 (or the value of
// field_0004 when the 1-state branch was taken and field_0004 was non-zero).
static int __fastcall C_008672e4_SetStateIfChanged(C_008672e4* self, void* /*edx*/, int newValue)
{
    int previous = static_cast<int>(self->field_0008);
    if (newValue != previous &&
        (previous != 1 || (previous = static_cast<int>(self->field_0004)) != 0))
    {
        self->field_0008 = static_cast<uint32_t>(newValue);
    }
    return previous;
}

HOOK(0x007381F0, C_008672e4_SetStateIfChanged, "auto");
