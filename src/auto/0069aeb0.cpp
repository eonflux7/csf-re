#include <cstdint>
#include "hook.h"

// Zeroes four 32-bit fields of the object at offsets 4, 8, 0xC and 0x10
// (in the original's store order) and returns 0.
static int __fastcall ResetFourDwords(void* self, void* /*edx*/)
{
    uint8_t* obj = static_cast<uint8_t*>(self);

    *reinterpret_cast<uint32_t*>(obj + 0x0C) = 0;
    *reinterpret_cast<uint32_t*>(obj + 0x04) = 0;
    *reinterpret_cast<uint32_t*>(obj + 0x10) = 0;
    *reinterpret_cast<uint32_t*>(obj + 0x08) = 0;

    return 0;
}

HOOK(0x69AEB0, ResetFourDwords, "auto");
