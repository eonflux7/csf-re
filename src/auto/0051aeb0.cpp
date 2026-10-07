#include <cstdint>
#include "hook.h"

// Returns the byte stored at index `index` inside the byte buffer whose pointer
// lives at offset 0x10 of the object (no bounds checking in the original).
static char __fastcall GetByteAtIndex(void* self, void* /*edx*/, int index)
{
    uint8_t* buffer = reinterpret_cast<uint8_t*>(
        *reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(self) + 0x10));
    return static_cast<char>(buffer[index]);
}

HOOK(0x51AEB0, GetByteAtIndex, "auto");
