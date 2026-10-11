#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static std::uint32_t* __fastcall InitializePositionRecord(
    std::uint32_t* self, void* /*edx*/, int firstValue, int secondValue,
    const void* source, const std::uint32_t* position)
{
    CSF_AT(void*, self, 0x8) = CSF_VTBL(0x0082D448);
    CSF_AT(int, self, 0xC) = -1;
    CSF_AT(int, self, 0x10) = -1;
    CSF_AT(std::uint32_t, self, 0x14) = 0;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;

    CSF_AT(int, self, 0x0) = firstValue;
    CSF_AT(int, self, 0x4) = secondValue;
    CSF_AT(std::uint32_t, self, 0xC) = CSF_AT(std::uint32_t, source, 0x4);
    CSF_AT(std::uint32_t, self, 0x10) = CSF_AT(std::uint32_t, source, 0x8);
    CSF_AT(std::uint32_t, self, 0x14) = position[0];
    CSF_AT(std::uint32_t, self, 0x18) = position[1];
    CSF_AT(std::uint32_t, self, 0x1C) = position[2];
    CSF_AT(float, self, 0x20) = 1.0f;
    CSF_AT(std::uint32_t, self, 0x24) = 0;
    return self;
}

HOOK(0x00446040, InitializePositionRecord, "auto");
