#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns 1 when the float at +0x64 is less than or equal to zero, 0 otherwise.
// NaN yields 0, matching the original's unordered (parity) test.
static int __fastcall C_008344f0_IsFieldAtOrBelowZero(C_008344f0* self, void* /*edx*/)
{
    return self->field_0064 <= g_fZero ? 1 : 0;
}

HOOK(0x004A2F40, C_008344f0_IsFieldAtOrBelowZero, "auto");
