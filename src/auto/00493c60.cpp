#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns true when the state/flag word at +0x2E0 has bit 0x4 clear
// (full 32-bit compare of the masked value against zero).
static int __fastcall C_00833ee8_IsFlagBit2Clear(C_00833ee8* self, void* /*edx*/)
{
    return (self->field_02e0 & 0xFFFFFFFBu) == 0;
}

HOOK(0x00493C60, C_00833ee8_IsFlagBit2Clear, "auto");
