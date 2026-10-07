#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns bit 6 of the flag word at +0x14.
static int __fastcall C_00863430_GetFlagBit6(C_00863430* self, void* /*edx*/, int /*a2*/)
{
    return (self->field_0014 >> 6) & 1;
}

HOOK(0x007082F0, C_00863430_GetFlagBit6, "auto");
