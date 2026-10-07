// Reimplementation of sub_548A20 (0x00548A20)
// Clears the three dwords at +0x5C, +0x60, +0x64 and returns 0.

#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

static int __fastcall C_00843db8_ClearTrailingDwords(C_00843db8* self, void* /*edx*/)
{
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    return 0;
}

HOOK(0x00548A20, C_00843db8_ClearTrailingDwords, "auto");
