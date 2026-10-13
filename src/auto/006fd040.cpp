#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies a 16-byte block (four dwords) from the caller's buffer into the
// object at +0x50, sets the flag byte at +0x81, and returns the address of
// the stored block as an int.
static int __fastcall CLightObject_StoreSecondaryVec4AndSetFlag81(CLightObject* self, void* /*edx*/, uint32_t* src)
{
    CSF_AT(uint32_t, self, 0x50) = src[0];
    CSF_AT(uint32_t, self, 0x54) = src[1];
    CSF_AT(uint32_t, self, 0x58) = src[2];
    CSF_AT(uint32_t, self, 0x5C) = src[3];
    CSF_AT(uint8_t, self, 0x81) = 1;
    return static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x50));
}

HOOK(0x6FD040, CLightObject_StoreSecondaryVec4AndSetFlag81, "auto");
