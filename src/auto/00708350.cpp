#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns bit 5 of field_0014 (offset 0x14) as a 32-bit int.
static int __fastcall C_00863430_GetBit5OfField14(C_00863430* self, void* /*edx*/, int /*a2*/)
{
    return (self->field_0014 >> 5) & 1;
}

HOOK(0x00708350, C_00863430_GetBit5OfField14, "auto");
