#include <cstdint>
#include "globals.h"
#include "hook.h"

// Zeroes the object's header: byte at +0x0 and dwords at +0x4 / +0x8,
// then returns the object pointer in eax.
static void* __fastcall C_004d1610_ClearHeader(void* self, void* /*edx*/)
{
    *reinterpret_cast<uint8_t*>(self) = 0;
    CSF_AT(uint32_t, self, 0x4) = 0;
    CSF_AT(uint32_t, self, 0x8) = 0;
    return self;
}

HOOK(0x004D1610, C_004d1610_ClearHeader, "auto");
