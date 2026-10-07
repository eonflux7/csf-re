#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static std::uint32_t* __fastcall InitializeDefaultColorPalette(
    std::uint32_t* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x00) = 0;
    CSF_AT(std::uint32_t, self, 0x04) = 0;
    CSF_AT(std::uint32_t, self, 0x08) = 0;
    CSF_AT(std::uint32_t, self, 0x0C) = 0x3F800000;
    CSF_AT(std::uint32_t, self, 0x1C) = 0x3F800000;
    CSF_AT(std::uint32_t, self, 0x10) = 0;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint32_t, self, 0x2C) = 0x3F800000;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint32_t, self, 0x24) = 0;
    CSF_AT(std::uint32_t, self, 0x28) = 0;
    CSF_AT(std::uint32_t, self, 0x3C) = 0x3F800000;
    CSF_AT(std::uint32_t, self, 0x30) = 0;
    CSF_AT(std::uint32_t, self, 0x34) = 0;
    CSF_AT(std::uint32_t, self, 0x38) = 0;

    CSF_AT(std::uint32_t, self, 0x00) = 0;
    CSF_AT(std::uint32_t, self, 0x04) = 0;
    CSF_AT(std::uint32_t, self, 0x08) = 0;
    CSF_AT(std::uint32_t, self, 0x0C) = 0x433F0000;
    CSF_AT(std::uint32_t, self, 0x10) = 0x435F0000;
    CSF_AT(std::uint32_t, self, 0x14) = 0x43560000;
    CSF_AT(std::uint32_t, self, 0x18) = 0x43400000;
    CSF_AT(std::uint32_t, self, 0x1C) = 0x437F0000;
    CSF_AT(std::uint32_t, self, 0x20) = 0x42FA0000;
    CSF_AT(std::uint32_t, self, 0x24) = 0x42EE0000;
    CSF_AT(std::uint32_t, self, 0x28) = 0x42EC0000;
    CSF_AT(std::uint32_t, self, 0x2C) = 0x437F0000;
    CSF_AT(std::uint32_t, self, 0x30) = 0x42FA0000;
    CSF_AT(std::uint32_t, self, 0x34) = 0x43010000;
    CSF_AT(std::uint32_t, self, 0x38) = 0x42EC0000;
    CSF_AT(std::uint32_t, self, 0x3C) = 0x437F0000;

    return self;
}

HOOK(0x0071DF80, InitializeDefaultColorPalette, "auto");
