#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static void* __fastcall GfxCollision_ResetQuery(void* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x04) = 0;
    CSF_AT(std::uint32_t, self, 0x08) = 0;
    CSF_AT(std::uint32_t, self, 0x0C) = 0;
    CSF_AT(std::uint32_t, self, 0x10) = 0;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint32_t, self, 0x5C) = 0;
    CSF_AT(std::uint8_t, self, 0x28) = 0;
    CSF_AT(std::int32_t, self, 0x2C) = -1;
    CSF_AT(std::int8_t, self, 0x30) = -1;
    CSF_AT(std::uint32_t, self, 0x60) = 0;
    CSF_AT(std::uint32_t, self, 0x64) = 10;
    CSF_AT(std::uint32_t, self, 0x24) = 0;
    CSF_AT(std::int32_t, self, 0x1C) = -1;
    CSF_AT(std::int8_t, self, 0x20) = -1;
    return self;
}

HOOK(0x006EBDE0, GfxCollision_ResetQuery, "auto");
