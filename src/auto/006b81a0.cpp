#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reads the 32-bit flags word at offset +0x04 and extracts bit 6.
// __thiscall (ecx = this, no stack args) -> ret 0.
static int __fastcall IsFlagBit6Set(void* self, void* /*edx*/)
{
    return (CSF_AT(uint32_t, self, 4) >> 6) & 1;
}

HOOK(0x6B81A0, IsFlagBit6Set, "auto");
