#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static char* __fastcall GameBddLoader_AssignMessageBoxConfiguration(
    char* self, void* /*edx*/, const char* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = CSF_AT(std::uint32_t, source, 0x04);
    CSF_AT(std::uint32_t, self, 0x08) = CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint8_t, self, 0x0C) = CSF_AT(std::uint8_t, source, 0x0C);
    CSF_AT(std::uint32_t, self, 0x10) = CSF_AT(std::uint32_t, source, 0x10);
    CSF_AT(std::uint32_t, self, 0x14) = CSF_AT(std::uint32_t, source, 0x14);
    CSF_AT(std::uint8_t, self, 0x18) = CSF_AT(std::uint8_t, source, 0x18);
    CSF_AT(std::uint8_t, self, 0x19) = CSF_AT(std::uint8_t, source, 0x19);
    CSF_AT(std::uint8_t, self, 0x1A) = CSF_AT(std::uint8_t, source, 0x1A);

    AssignStringSubstring(self + 28, nullptr, source + 28, 0, -1);
    AssignStringSubstring(self + 56, nullptr, source + 56, 0, -1);
    AssignStringSubstring(self + 84, nullptr, source + 84, 0, -1);
    AssignStringSubstring(self + 112, nullptr, source + 112, 0, -1);
    AssignStringSubstring(self + 140, nullptr, source + 140, 0, -1);

    CSF_AT(std::uint8_t, self, 0xA8) = CSF_AT(std::uint8_t, source, 0xA8);
    CSF_AT(std::uint8_t, self, 0xA9) = CSF_AT(std::uint8_t, source, 0xA9);
    CSF_AT(std::uint32_t, self, 0xAC) = CSF_AT(std::uint32_t, source, 0xAC);
    CSF_AT(std::uint32_t, self, 0xB0) = CSF_AT(std::uint32_t, source, 0xB0);

    return self;
}

HOOK(0x0050F160, GameBddLoader_AssignMessageBoxConfiguration, "auto");
