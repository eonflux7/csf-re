#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

static char* __fastcall CharacterDefs_CopyDefinition(char* self, void* /*edx*/, char* source)
{
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);
    AssignStringSubstring(self + 4, nullptr, source + 4, 0, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, self, 0x20) = CSF_AT(std::uint32_t, source, 0x20);
    CSF_AT(std::uint32_t, self, 0x24) = CSF_AT(std::uint32_t, source, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = CSF_AT(std::uint32_t, source, 0x28);

    CSF_AT(std::uint8_t, self, 0x2C) = CSF_AT(std::uint8_t, source, 0x2C);
    CSF_AT(std::uint8_t, self, 0x2D) = CSF_AT(std::uint8_t, source, 0x2D);

    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, source, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, source, 0x34);
    CSF_AT(std::uint32_t, self, 0x38) = CSF_AT(std::uint32_t, source, 0x38);

    return self;
}

HOOK(0x007625F0, CharacterDefs_CopyDefinition, "auto");
