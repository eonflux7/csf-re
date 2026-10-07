#include <cstdint>
#include "hook.h"

// __thiscall getter: returns the 32-bit value stored at offset 0x98 of the object.
static int __fastcall C_006cabc0_GetField0098(void* self, void* /*edx*/)
{
    return *reinterpret_cast<const int*>(static_cast<const char*>(self) + 0x98);
}

HOOK(0x6CABC0, C_006cabc0_GetField0098, "auto");
