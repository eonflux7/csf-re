#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns bit 0 of the byte at offset +2 of the object referenced by field_000c.
static int __fastcall C_00861ce0_IsField000cByte2Bit0Set(C_00861ce0* self, void* /*edx*/)
{
    const uint8_t* linked = reinterpret_cast<const uint8_t*>(static_cast<uintptr_t>(self->field_000c));
    return linked[2] & 1;
}

HOOK(0x006E5610, C_00861ce0_IsField000cByte2Bit0Set, "auto");
