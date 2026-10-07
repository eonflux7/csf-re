#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall sub_4D10F0: clears the three leading fields of the object
// (byte at +0x00, dwords at +0x04 and +0x08) and returns 0.
static int __fastcall C_004D10F0_ResetFields(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    return 0;
}

HOOK(0x004D10F0, C_004D10F0_ResetFields, "auto");
