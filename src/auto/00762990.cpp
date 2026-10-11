#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, CopyStructuredMember, 0x004A40E0, (void* self, void* edx, void* a5));
CSF_FUNC(int, __fastcall, CharacterDefs_AssignEmbeddedMember, 0x006CAF70, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, CharacterDefs_AssignDefinitionMember, 0x007625A0, (void* self, void* edx, void* a2));
CSF_FUNC(char*, __fastcall, CharacterDefs_AssignDefinitionExtension, 0x007625F0, (void* self, void* edx, int a2));

static char* __fastcall CharacterDefs_AssignCharacterDefinition(
    char* self, void* /*edx*/, int sourceAddress)
{
    char* source = reinterpret_cast<char*>(sourceAddress);

    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, source, 0x00);
    AssignStringSubstring(self + 4, nullptr, source + 4, 0, -1);
    AssignStringSubstring(self + 32, nullptr, source + 32, 0, -1);
    AssignStringSubstring(self + 60, nullptr, source + 60, 0, -1);
    AssignStringSubstring(self + 88, nullptr, source + 88, 0, -1);
    AssignStringSubstring(self + 116, nullptr, source + 116, 0, -1);
    AssignStringSubstring(self + 144, nullptr, source + 144, 0, -1);

    CSF_AT(std::uint32_t, self, 0xAC) = CSF_AT(std::uint32_t, source, 0xAC);
    CSF_AT(std::uint32_t, self, 0xB0) = CSF_AT(std::uint32_t, source, 0xB0);
    CSF_AT(std::uint32_t, self, 0xB4) = CSF_AT(std::uint32_t, source, 0xB4);
    CSF_AT(std::uint32_t, self, 0xB8) = CSF_AT(std::uint32_t, source, 0xB8);
    CSF_AT(std::uint32_t, self, 0xBC) = CSF_AT(std::uint32_t, source, 0xBC);
    CSF_AT(std::uint32_t, self, 0xC0) = CSF_AT(std::uint32_t, source, 0xC0);
    CSF_AT(std::uint32_t, self, 0xC4) = CSF_AT(std::uint32_t, source, 0xC4);
    CSF_AT(std::uint32_t, self, 0xC8) = CSF_AT(std::uint32_t, source, 0xC8);
    CSF_AT(std::uint32_t, self, 0xCC) = CSF_AT(std::uint32_t, source, 0xCC);
    CSF_AT(std::uint32_t, self, 0xD0) = CSF_AT(std::uint32_t, source, 0xD0);
    CSF_AT(std::uint32_t, self, 0xD4) = CSF_AT(std::uint32_t, source, 0xD4);
    CSF_AT(std::uint32_t, self, 0xD8) = CSF_AT(std::uint32_t, source, 0xD8);
    CSF_AT(std::uint32_t, self, 0xDC) = CSF_AT(std::uint32_t, source, 0xDC);
    CSF_AT(std::uint32_t, self, 0xE0) = CSF_AT(std::uint32_t, source, 0xE0);
    CSF_AT(std::uint32_t, self, 0xE4) = CSF_AT(std::uint32_t, source, 0xE4);
    CSF_AT(std::uint32_t, self, 0xE8) = CSF_AT(std::uint32_t, source, 0xE8);
    CSF_AT(std::uint32_t, self, 0xEC) = CSF_AT(std::uint32_t, source, 0xEC);

    CopyStructuredMember(self + 240, nullptr, source + 240);
    CharacterDefs_AssignEmbeddedMember(self + 256, nullptr,
                                       reinterpret_cast<int>(source + 256));
    CharacterDefs_AssignEmbeddedMember(self + 272, nullptr,
                                       reinterpret_cast<int>(source + 272));

    CSF_AT(std::uint32_t, self, 0x120) = CSF_AT(std::uint32_t, source, 0x120);
    CSF_AT(std::uint32_t, self, 0x124) = CSF_AT(std::uint32_t, source, 0x124);
    CSF_AT(std::uint32_t, self, 0x128) = CSF_AT(std::uint32_t, source, 0x128);
    CSF_AT(std::uint32_t, self, 0x12C) = CSF_AT(std::uint32_t, source, 0x12C);
    CSF_AT(std::uint32_t, self, 0x130) = CSF_AT(std::uint32_t, source, 0x130);
    CSF_AT(std::uint32_t, self, 0x134) = CSF_AT(std::uint32_t, source, 0x134);
    CSF_AT(std::uint32_t, self, 0x138) = CSF_AT(std::uint32_t, source, 0x138);
    CSF_AT(std::uint32_t, self, 0x13C) = CSF_AT(std::uint32_t, source, 0x13C);
    CSF_AT(std::uint32_t, self, 0x140) = CSF_AT(std::uint32_t, source, 0x140);
    CSF_AT(std::uint32_t, self, 0x144) = CSF_AT(std::uint32_t, source, 0x144);
    CSF_AT(std::uint32_t, self, 0x148) = CSF_AT(std::uint32_t, source, 0x148);
    CSF_AT(std::uint32_t, self, 0x14C) = CSF_AT(std::uint32_t, source, 0x14C);
    CSF_AT(std::uint32_t, self, 0x150) = CSF_AT(std::uint32_t, source, 0x150);
    CSF_AT(std::uint32_t, self, 0x154) = CSF_AT(std::uint32_t, source, 0x154);

    CSF_AT(std::uint8_t, self, 0x158) = CSF_AT(std::uint8_t, source, 0x158);
    CSF_AT(std::uint8_t, self, 0x159) = CSF_AT(std::uint8_t, source, 0x159);
    CSF_AT(std::uint8_t, self, 0x15A) = CSF_AT(std::uint8_t, source, 0x15A);
    CSF_AT(std::uint8_t, self, 0x15B) = CSF_AT(std::uint8_t, source, 0x15B);
    CSF_AT(std::uint8_t, self, 0x15C) = CSF_AT(std::uint8_t, source, 0x15C);

    CharacterDefs_AssignDefinitionMember(self + 352, nullptr, source + 352);
    AssignStringSubstring(self + 408, nullptr, source + 408, 0, -1);

    // Preserve the original forward, dword-at-a-time copy.
    for (int offset = 0; offset < 0x3C; offset += 4)
    {
        CSF_AT(std::uint32_t, self + 436, offset) =
            CSF_AT(std::uint32_t, source + 436, offset);
    }

    CharacterDefs_AssignDefinitionExtension(self + 496, nullptr,
                                            reinterpret_cast<int>(source + 496));
    return self;
}

HOOK(0x00762990, CharacterDefs_AssignCharacterDefinition, "auto");
