#include <cstdint>
#include "hook.h"

// Stores a 16-bit value into the object at offset 0 and returns that value.
static int16_t __fastcall SetWordValue(uint16_t* self, void* /*edx*/, int16_t value)
{
    *self = static_cast<uint16_t>(value);
    return value;
}

HOOK(0x006B8370, SetWordValue, "auto");
