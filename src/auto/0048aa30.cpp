#include <cstdint>
#include "hook.h"

// Constructor/initializer: zeroes the first dword and the byte at offset +4,
// then returns the object pointer (this).
static uint8_t* __fastcall InitZeroedObject(uint8_t* self, void* /*edx*/)
{
    *reinterpret_cast<uint32_t*>(self) = 0;
    *(self + 4) = 0;
    return self;
}

HOOK(0x0048AA30, InitZeroedObject, "auto");
