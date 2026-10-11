#include <cstdint>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall AiCore_InitializeState(void* self, void* /*edx*/)
{
    CSF_AT(std::int32_t, self, 0x04) = 0;
    CSF_AT(std::int32_t, self, 0x08) = 0;
    CSF_AT(std::int32_t, self, 0x0C) = 0;
    CSF_AT(std::int32_t, self, 0x10) = 0;
    CSF_AT(std::int32_t, self, 0x14) = 10;
    CSF_AT(std::int32_t, self, 0x24) = 10;
    CSF_AT(std::int32_t, self, 0x18) = 0;
    CSF_AT(std::int32_t, self, 0x1C) = 0;
    CSF_AT(std::int32_t, self, 0x20) = 0;
    CSF_AT(std::int32_t, self, 0x38) = -1;
    CSF_AT(std::int32_t, self, 0x3C) = -1;
    CSF_AT(void**, self, 0x34) = CSF_VTBL(0x0082D448);
    CSF_AT(std::int32_t, self, 0x40) = 0;
    CSF_AT(std::int32_t, self, 0x44) = 0;
    CSF_AT(std::int32_t, self, 0x48) = 0;
    CSF_AT(std::int32_t, self, 0x28) = -1;
    CSF_AT(std::int32_t, self, 0x2C) = -1;
    CSF_AT(std::int32_t, self, 0x30) = -1;
    CSF_AT(std::int32_t, self, 0x38) = -1;
    CSF_AT(std::int32_t, self, 0x3C) = -1;
    CSF_AT(void**, self, 0x4C) = CSF_VTBL(0x0082D448);
    CSF_AT(std::int32_t, self, 0x50) = -1;
    CSF_AT(std::int32_t, self, 0x54) = -1;
    CSF_AT(std::int32_t, self, 0x58) = 0;
    CSF_AT(std::int32_t, self, 0x64) = 0;
    CSF_AT(std::int32_t, self, 0x68) = 0;
    CSF_AT(std::int32_t, self, 0x6C) = 0;

    auto* vector = static_cast<std::uint8_t*>(self) + 0x70;
    for (int remaining = 3; remaining != 0; --remaining, vector += 0x0C)
    {
        CSF_AT(std::int32_t, vector, 0x00) = 0;
        CSF_AT(std::int32_t, vector, 0x04) = 0;
        CSF_AT(std::int32_t, vector, 0x08) = 0;
    }

    CSF_AT(std::int32_t, self, 0x5C) = 0;
    CSF_AT(float, self, 0x60) = 1.0f;
    CSF_AT(std::int32_t, self, 0xA4) = 0;
    CSF_AT(std::int32_t, self, 0x28) = -1;
    CSF_AT(std::int32_t, self, 0x2C) = -1;
    CSF_AT(std::int32_t, self, 0x30) = -1;
    CSF_AT(std::int32_t, self, 0x38) = -1;
    CSF_AT(std::int32_t, self, 0x3C) = -1;
    CSF_AT(std::int32_t, self, 0x50) = -1;
    CSF_AT(std::int32_t, self, 0x54) = -1;
    CSF_AT(float, self, 0x60) = 1.0f;
    CSF_AT(std::int32_t, self, 0x5C) = 0;
    CSF_AT(float, self, 0x9C) = FLT_MAX;
    CSF_AT(std::int32_t, self, 0x94) = 0;
    CSF_AT(std::uint8_t, self, 0xA8) = 0;
    CSF_AT(std::uint8_t, self, 0xA9) = 0;
    CSF_AT(std::int32_t, self, 0xA0) = 0;
    CSF_AT(std::int32_t, self, 0xA4) = 0;

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x004BD6C0, AiCore_InitializeState, "auto");
