#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static std::uint32_t* __fastcall InitializeDefaultState(std::uint32_t* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x08) = 0;
    CSF_AT(std::uint32_t, self, 0x0C) = 0;
    CSF_AT(void*, self, 0x10) = CSF_VTBL(0x0082D448);
    CSF_AT(std::int32_t, self, 0x14) = -1;
    CSF_AT(std::int32_t, self, 0x18) = -1;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    CSF_AT(std::uint32_t, self, 0x20) = 0;
    CSF_AT(std::uint32_t, self, 0x24) = 0;
    CSF_AT(std::uint8_t, self, 0x29) = 0;
    CSF_AT(std::uint8_t, self, 0x28) = 1;
    CSF_AT(std::uint8_t, self, 0x2A) = 1;
    CSF_AT(void*, self, 0x2C) = CSF_VTBL(0x0082D448);
    CSF_AT(void*, self, 0x3C) = CSF_VTBL(0x0082D448);
    CSF_AT(std::int32_t, self, 0x40) = -1;
    CSF_AT(std::int32_t, self, 0x44) = -1;
    CSF_AT(std::uint32_t, self, 0x48) = 0;
    CSF_AT(std::uint32_t, self, 0x4C) = 0;
    CSF_AT(std::uint32_t, self, 0x50) = 0;
    CSF_AT(std::uint8_t, self, 0x58) = 1;
    CSF_AT(std::uint8_t, self, 0x5C) = 1;
    CSF_AT(std::uint32_t, self, 0x38) = 0;
    CSF_AT(std::int32_t, self, 0x54) = -1;
    CSF_AT(std::uint8_t, self, 0x59) = 0;
    CSF_AT(std::uint8_t, self, 0x5A) = 0;
    CSF_AT(std::uint8_t, self, 0x5B) = 0;
    CSF_AT(std::uint8_t, self, 0x5D) = 0;
    CSF_AT(std::int32_t, self, 0x30) = -1;
    CSF_AT(std::int32_t, self, 0x34) = -1;
    CSF_AT(std::uint32_t, self, 0x04) = 1;
    CSF_AT(std::uint32_t, self, 0x00) = 0;
    return self;
}

HOOK(0x0048A110, InitializeDefaultState, "auto");
