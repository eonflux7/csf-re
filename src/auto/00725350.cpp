#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static std::uint32_t* __fastcall InitializeState(std::uint32_t* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x24) = 0;
    CSF_AT(std::uint32_t, self, 0x28) = 0;
    CSF_AT(std::uint32_t, self, 0x2C) = 0;
    CSF_AT(std::uint32_t, self, 0x30) = 0x3F800000;
    CSF_AT(std::uint32_t, self, 0x34) = 0;
    CSF_AT(std::uint32_t, self, 0x38) = 0;
    CSF_AT(std::uint32_t, self, 0x3C) = 0;
    CSF_AT(std::uint32_t, self, 0x40) = 0x3F800000;
    CSF_AT(std::uint32_t, self, 0x08) = 0;
    CSF_AT(std::uint32_t, self, 0x04) = 0;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint8_t, self, 0x00) = 0;
    CSF_AT(std::uint8_t, self, 0x10) = 0;
    CSF_AT(std::uint8_t, self, 0x11) = 0;
    CSF_AT(std::uint32_t, self, 0x0C) = 0;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    return self;
}

HOOK(0x00725350, InitializeState, "auto");
