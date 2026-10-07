#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns true when bit 2 (value 4) of the 32-bit flags field at this+0x7C is clear.
static bool __fastcall IsStateFlagBit2Clear(void* self, void* /*edx*/)
{
    return (CSF_AT(uint32_t, self, 0x7c) & 4u) != 4u;
}

HOOK(0x41D030, IsStateFlagBit2Clear, "auto");
