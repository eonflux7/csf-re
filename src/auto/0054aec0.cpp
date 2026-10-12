#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static std::uint32_t* __fastcall TypeRegistry_InitializeRecord(
    std::uint32_t* self, void* /*edx*/,
    int first, int second, int third,
    std::uint32_t* source, char flag)
{
    CSF_AT(std::uint32_t, self, 0x00) = first;
    CSF_AT(std::uint32_t, self, 0x04) = second;
    CSF_AT(std::uint32_t, self, 0x08) = third;
    CSF_AT(std::uint32_t, self, 0x0C) = source[0];
    CSF_AT(std::uint32_t, self, 0x10) = source[1];
    CSF_AT(std::uint32_t, self, 0x14) = source[2];
    CSF_AT(char, self, 0x18) = flag;
    CSF_AT(std::uint8_t, self, 0x19) = 0;
    return self;
}

HOOK(0x0054AEC0, TypeRegistry_InitializeRecord, "auto");
