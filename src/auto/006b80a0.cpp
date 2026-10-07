// Reimplementation of 0x006B80A0 (Commandos: Strike Force, Pyro Studios)
// Reads a 16-bit value stored two bytes into the object. The value 0xFFFF is a
// sentinel meaning "no valid value" and is translated to -1.

#include <cstdint>

#include "globals.h"
#include "hook.h"

// __thiscall method: ecx = this, no stack arguments (ret).
// Returns the zero-extended 16-bit field, or -1 when the field is 0xFFFF.
static int __fastcall GetUint16OrSentinelMinusOne(void* self, void* /*edx*/)
{
    const uint16_t rawValue = CSF_AT(uint16_t, self, 2);
    if (rawValue == 0xFFFF)
        return -1;
    return static_cast<int>(rawValue);
}

HOOK(0x6B80A0, GetUint16OrSentinelMinusOne, "auto");
