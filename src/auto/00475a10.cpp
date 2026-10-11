#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static std::uint32_t* __fastcall WeaponFirearms_InitializeState(
    std::uint32_t* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x00) = 0;
    CSF_AT(std::uint32_t, self, 0x04) = 0;
    CSF_AT(std::uint32_t, self, 0x08) = 0;
    CSF_AT(std::uint8_t, self, 0x0C) = 0;
    CSF_AT(std::uint8_t, self, 0x0D) = 0;
    CSF_AT(std::uint8_t, self, 0x0E) = 0;
    CSF_AT(std::uint32_t, self, 0x10) = 1065353216;
    CSF_AT(std::uint32_t, self, 0x14) = 1065353216;
    CSF_AT(std::uint32_t, self, 0x18) = 1065353216;
    CSF_AT(std::uint8_t, self, 0x1C) = 0;
    CSF_AT(std::uint8_t, self, 0x1D) = 1;
    return self;
}

HOOK(0x00475A10, WeaponFirearms_InitializeState, "auto");
