#include <cstdint>
#include "hook.h"

// Writes the four bytes of `packed` big-endian into the 5-byte record at `dest`,
// then zeroes the fifth byte. Returns `dest`.
static int __cdecl SelectionData_BuildFromIndex(int packed, int dest)
{
    uint8_t* out = reinterpret_cast<uint8_t*>(dest);
    uint32_t value = static_cast<uint32_t>(packed);

    out[0] = static_cast<uint8_t>(value >> 24);
    out[1] = static_cast<uint8_t>(value >> 16);
    out[2] = static_cast<uint8_t>(value >> 8);
    out[3] = static_cast<uint8_t>(value);
    out[4] = 0;

    return dest;
}

HOOK(0x00698DF0, SelectionData_BuildFromIndex, "auto");
