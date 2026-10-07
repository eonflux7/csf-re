#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Reads bit 3 of field_0014 (offset 0x14) and returns it as an int.
static int __fastcall C_00863430_GetFlagBit3(C_00863430* self, void* /*edx*/)
{
    return (self->field_0014 >> 3) & 1;
}

HOOK(0x007082D0, C_00863430_GetFlagBit3, "auto");
