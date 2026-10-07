#include <cstdint>
#include "hook.h"
#include "globals.h"

// __thiscall getter: returns the byte at offset 0x334 of the object.
static uint8_t __fastcall C_Unknown_GetByte334(void* self, void* /*edx*/)
{
    return CSF_AT(uint8_t, self, 0x334);
}

HOOK(0x6A71A0, C_Unknown_GetByte334, "auto");
