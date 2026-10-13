#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the first non-zero of field_0048 / field_0043. If both are zero,
// latches field_0048 to 1 and returns 0.
static char __fastcall C_0083ba40_TestAndSetFlag(C_0083ba40* self, void* /*edx*/) {
    char result = self->field_0048;
    if (!result) {
        result = self->field_0043;
        if (!result) {
            self->field_0048 = 1;
        }
    }
    return result;
}

HOOK(0x004D2050, C_0083ba40_TestAndSetFlag, "auto");
