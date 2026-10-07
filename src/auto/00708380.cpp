#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns bit 2 of the flag word at offset 0x14.
static int __fastcall C_00863430_GetFlagBit2(C_00863430* self, void* /*edx*/)
{
    return static_cast<int>((self->field_0014 >> 2) & 1u);
}

HOOK(0x00708380, C_00863430_GetFlagBit2, "auto");
