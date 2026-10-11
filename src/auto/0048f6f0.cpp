#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static std::uint8_t* __fastcall SetByteValueAndMarkDirty(
    void* self, void* /*edx*/, std::uint8_t* value)
{
    void* state = CSF_AT(void*, self, 0xA8);
    CSF_AT(std::uint8_t, self, 0x171) = 1;
    CSF_AT(std::uint8_t, state, 0xE) = 1;
    CSF_AT(std::uint8_t, self, 0x114) = *value;
    return value;
}

HOOK(0x0048F6F0, SetByteValueAndMarkDirty, "auto");
