#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static std::uint32_t* __fastcall WeaponFirearms_CopyState(
    std::uint32_t* self, void* /*edx*/, const void* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);
    CSF_AT(std::uint32_t, self, 0x08) = CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x0C) = CSF_AT(std::uint32_t, source, 0x0C);
    CSF_AT(std::uint32_t, self, 0x10) = CSF_AT(std::uint32_t, source, 0x10);
    CSF_AT(std::uint32_t, self, 0x14) = CSF_AT(std::uint32_t, source, 0x14);
    CSF_AT(std::uint32_t, self, 0x18) = CSF_AT(std::uint32_t, source, 0x18);
    CSF_AT(std::uint32_t, self, 0x1C) = CSF_AT(std::uint32_t, source, 0x1C);
    CSF_AT(std::uint8_t, self, 0x20) = CSF_AT(std::uint8_t, source, 0x20);
    CSF_AT(std::uint8_t, self, 0x21) = CSF_AT(std::uint8_t, source, 0x21);
    CSF_AT(std::uint8_t, self, 0x22) = CSF_AT(std::uint8_t, source, 0x22);
    CSF_AT(std::uint8_t, self, 0x23) = CSF_AT(std::uint8_t, source, 0x23);
    CSF_AT(std::uint8_t, self, 0x24) = CSF_AT(std::uint8_t, source, 0x24);
    CSF_AT(std::uint8_t, self, 0x25) = CSF_AT(std::uint8_t, source, 0x25);
    return self;
}

HOOK(0x004759B0, WeaponFirearms_CopyState, "auto");
