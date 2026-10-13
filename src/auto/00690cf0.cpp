#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns true when every bit set in `mask` is also set in the flags byte at this+0x28.
static bool __fastcall MemDebug_HasAllFlagBits(void* self, void* /*edx*/, uint8_t mask)
{
    uint8_t flags = CSF_AT(uint8_t, self, 0x28);
    return (flags & mask) == mask;
}

HOOK(0x690CF0, MemDebug_HasAllFlagBits, "auto");
