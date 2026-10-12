#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies a 64-byte block from *source into the buffer at +0x84, then sets the flag at +0x20.
// Returns this.
static int __fastcall C_00867260_LoadSixtyFourByteBlock(C_00867260* self, void* /*edx*/, const void** source)
{
    std::memcpy(CSF_AT(void*, self, 0x84), *source, 0x40);
    CSF_AT(uint8_t, self, 0x20) = 1;
    return reinterpret_cast<int>(self);
}

HOOK(0x737780, C_00867260_LoadSixtyFourByteBlock, "auto");
