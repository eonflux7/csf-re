#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns non-zero when bit 0x8 of the flags word at +0x170 is set,
// or when the code at +0x50 equals 34 (0x22).
static int __fastcall C_00833908_HasFlag8OrCodeIs34(C_00833908* self, void* /*edx*/)
{
    if ((self->field_0170 & 8) != 0) {
        return 1;
    }
    if (self->field_0050 == 34) {
        return 1;
    }
    return 0;
}

HOOK(0x48E300, C_00833908_HasFlag8OrCodeIs34, "auto");
