#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Sets the byte flag at offset 0x2C9 of the object.
static void __fastcall SetByteFlag2C9(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x2c9) = 1;
}

HOOK(0x6A71B0, SetByteFlag2C9, "auto");
