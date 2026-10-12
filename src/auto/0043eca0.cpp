#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static char* __fastcall Campaign_CopyConstructRecord(
    char* self, void* /*edx*/, const char* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);

    CSF_AT(std::uint32_t, self, 0x1C) = 15;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(char, self, 0x08) = 0;
    AssignStringSubstring(self + 0x04, nullptr, source + 0x04, 0, -1);

    CSF_AT(std::uint32_t, self, 0x38) = 15;
    CSF_AT(std::uint32_t, self, 0x34) = 0;
    CSF_AT(char, self, 0x24) = 0;
    AssignStringSubstring(self + 0x20, nullptr, source + 0x20, 0, -1);

    CSF_AT(std::uint32_t, self, 0x3C) = CSF_AT(std::uint32_t, source, 0x3C);

    CSF_AT(std::uint32_t, self, 0x58) = 15;
    CSF_AT(std::uint32_t, self, 0x54) = 0;
    CSF_AT(char, self, 0x44) = 0;
    AssignStringSubstring(self + 0x40, nullptr, source + 0x40, 0, -1);

    CSF_AT(std::uint32_t, self, 0x5C) = CSF_AT(std::uint32_t, source, 0x5C);

    CSF_AT(std::uint32_t, self, 0x78) = 15;
    CSF_AT(std::uint32_t, self, 0x74) = 0;
    CSF_AT(char, self, 0x64) = 0;
    AssignStringSubstring(self + 0x60, nullptr, source + 0x60, 0, -1);

    CSF_AT(std::uint32_t, self, 0x7C) = CSF_AT(std::uint32_t, source, 0x7C);

    CSF_AT(std::uint32_t, self, 0x98) = 15;
    CSF_AT(std::uint32_t, self, 0x94) = 0;
    CSF_AT(char, self, 0x84) = 0;
    AssignStringSubstring(self + 0x80, nullptr, source + 0x80, 0, -1);

    CSF_AT(std::uint32_t, self, 0xB4) = 15;
    CSF_AT(std::uint32_t, self, 0xB0) = 0;
    CSF_AT(char, self, 0xA0) = 0;
    AssignStringSubstring(self + 0x9C, nullptr, source + 0x9C, 0, -1);

    CSF_AT(std::uint32_t, self, 0xB8) = CSF_AT(std::uint32_t, source, 0xB8);
    CSF_AT(std::uint32_t, self, 0xBC) = CSF_AT(std::uint32_t, source, 0xBC);
    CSF_AT(char, self, 0xC0) = CSF_AT(char, source, 0xC0);
    return self;
}

HOOK(0x0043ECA0, Campaign_CopyConstructRecord, "auto");
