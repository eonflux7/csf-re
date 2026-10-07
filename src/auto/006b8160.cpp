#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Reads the 32-bit field at offset +4 of the object, shifts it right by one
// (unsigned) and returns bit 0 of the result, i.e. bit 1 of the original field.
static int __fastcall GetField04Bit1(void* self, void* /*edx*/)
{
    return (CSF_AT(uint32_t, self, 0x4) >> 1) & 1;
}

HOOK(0x6B8160, GetField04Bit1, "auto");
