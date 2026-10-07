#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall SetVectorAndMarkDirty(
    void* self, void* /*edx*/, const std::uint32_t* value)
{
    void* linkedObject = CSF_AT(void*, self, 0xA8);
    CSF_AT(std::uint8_t, self, 0x171) = 1;
    CSF_AT(std::uint8_t, linkedObject, 0x0C) = 1;

    CSF_AT(std::uint32_t, self, 0xE4) = value[0];
    CSF_AT(std::uint32_t, self, 0xE8) = value[1];
    const std::uint32_t lastComponent = value[2];
    CSF_AT(std::uint32_t, self, 0xEC) = lastComponent;
    return static_cast<int>(lastComponent);
}

HOOK(0x0048F630, SetVectorAndMarkDirty, "auto");
