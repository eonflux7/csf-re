#include <cstdint>
#include "globals.h"
#include "hook.h"

// Setter: stores the given byte argument at offset 0x14 of the object and returns it.
// Original: char __thiscall sub_50B440(_BYTE *this, char a2)
static char __fastcall C_0050b440_SetByteAt0x14(void* self, void* /*edx*/, char value)
{
    CSF_AT(uint8_t, self, 0x14) = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x50B440, C_0050b440_SetByteAt0x14, "auto");
