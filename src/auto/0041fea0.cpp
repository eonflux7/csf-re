#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Sets field_0020 and field_0024 to 1 unless field_0020 is already 1 and field_0024 is 0.
// Always returns 1.
static int __fastcall C_0082e7c8_SetActiveFlagsUnlessIdle(C_0082e7c8* self, void* /*edx*/) {
    const int result = 1;
    if (self->field_0020 != 1 || self->field_0024) {
        self->field_0020 = 1;
        self->field_0024 = 1;
    }
    return result;
}

HOOK(0x41FEA0, C_0082e7c8_SetActiveFlagsUnlessIdle, "auto");
