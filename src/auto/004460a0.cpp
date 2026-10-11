#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static void* __fastcall InitializePositionRecord(
    void* self, void* /*edx*/, int identifier, const void* source,
    const void* position, int parameter)
{
    CSF_AT(void*, self, 0x8) = CSF_VTBL(0x0082D448);
    CSF_AT(int, self, 0xC) = -1;
    CSF_AT(int, self, 0x10) = -1;
    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 0;
    CSF_AT(int, self, 0x4) = -1;
    CSF_AT(int, self, 0x0) = identifier;
    CSF_AT(std::uint32_t, self, 0xC) = CSF_AT(std::uint32_t, source, 0x4);
    CSF_AT(std::uint32_t, self, 0x10) = CSF_AT(std::uint32_t, source, 0x8);
    CSF_AT(std::uint32_t, self, 0x14) = CSF_AT(std::uint32_t, position, 0x0);
    CSF_AT(std::uint32_t, self, 0x18) = CSF_AT(std::uint32_t, position, 0x4);
    CSF_AT(std::uint32_t, self, 0x1C) = CSF_AT(std::uint32_t, position, 0x8);
    CSF_AT(float, self, 0x20) = 1.0f;
    CSF_AT(int, self, 0x24) = parameter;
    return self;
}

HOOK(0x004460A0, InitializePositionRecord, "auto");
