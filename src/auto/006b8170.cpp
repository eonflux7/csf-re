#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns bit 2 of the 32-bit flags field stored at offset +4 of the object.
static int __fastcall GetBit2OfField4(void* self, void* /*edx*/)
{
    uint32_t flags = CSF_AT(uint32_t, self, 4);
    return (flags >> 2) & 1;
}

HOOK(0x6B8170, GetBit2OfField4, "auto");
