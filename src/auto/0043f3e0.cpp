#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static char* __fastcall Campaign_AssignRecord(
    char* self, void* /*edx*/, const char* source)
{
    CSF_AT(std::uint32_t, self, 0) = CSF_AT(std::uint32_t, source, 0);
    AssignStringSubstring(self + 4, nullptr, source + 4, 0, -1);
    AssignStringSubstring(self + 32, nullptr, source + 32, 0, -1);

    CSF_AT(std::uint32_t, self, 60) = CSF_AT(std::uint32_t, source, 60);
    AssignStringSubstring(self + 64, nullptr, source + 64, 0, -1);

    CSF_AT(std::uint32_t, self, 92) = CSF_AT(std::uint32_t, source, 92);
    AssignStringSubstring(self + 96, nullptr, source + 96, 0, -1);

    CSF_AT(std::uint32_t, self, 124) = CSF_AT(std::uint32_t, source, 124);
    AssignStringSubstring(self + 128, nullptr, source + 128, 0, -1);
    AssignStringSubstring(self + 156, nullptr, source + 156, 0, -1);

    CSF_AT(std::uint32_t, self, 184) = CSF_AT(std::uint32_t, source, 184);
    CSF_AT(std::uint32_t, self, 188) = CSF_AT(std::uint32_t, source, 188);
    CSF_AT(std::uint8_t, self, 192) = CSF_AT(std::uint8_t, source, 192);

    return self;
}

HOOK(0x0043F3E0, Campaign_AssignRecord, "auto");
