#include <cstdint>
#include "hook.h"

// 0x00694C20
// Writes a NUL terminator at data[length] and returns the length.
// Field layout used: +0x00 = length (uint32), +0x0C = pointer to the data buffer.
static int __fastcall LengthPrefixedBuffer_TerminateAtLength(void* self, void* /*edx*/)
{
    uint8_t* const data = *reinterpret_cast<uint8_t* const*>(static_cast<uint8_t*>(self) + 0xC);
    data[*reinterpret_cast<const uint32_t*>(self)] = 0;
    return static_cast<int>(*reinterpret_cast<const uint32_t*>(self));
}

HOOK(0x694C20, LengthPrefixedBuffer_TerminateAtLength, "auto");
