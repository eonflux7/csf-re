#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stores the given byte into the object's field at offset +6 and returns that same value.
// __thiscall(self, char) with `ret 4` -> ecx = this, one 4-byte stack argument.
static char __fastcall C_50A4F0_SetByteField6(void* self, void* /*edx*/, char value)
{
    *reinterpret_cast<char*>(static_cast<uint8_t*>(self) + 6) = value;
    return value;
}

HOOK(0x50A4F0, C_50A4F0_SetByteField6, "auto");
