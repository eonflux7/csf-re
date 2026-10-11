#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores the enable flag and its inverse, plus a second flag, on an audio effect entry.
// Returns true when the enable flag is zero (i.e. the entry is disabled).
static bool __fastcall C_008639b0_SetEnabledFlags(C_008639b0* self, void* /*edx*/, char enabled, char secondaryFlag)
{
    self->field_0041 = enabled;
    self->field_0042 = (enabled == 0);
    self->field_0043 = secondaryFlag;
    return enabled == 0;
}

HOOK(0x0070E200, C_008639b0_SetEnabledFlags, "auto");
