#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, MissionScript_ProcessEntry_4419F0, 0x004419F0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void**, __fastcall, TypeRegistry_CopyEmbeddedDefinition, 0x0054D900, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, TypeRegistry_CopyTrailingDefinition, 0x0054D960, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, WeaponDefs_CopyTwelveByteVector, 0x0054D9D0, (void* self, void* edx, int a2));

static char* __fastcall TypeRegistry_CopyCharacterDefinition(
    char* self, void* /*edx*/, int sourceAddress)
{
    char* source = reinterpret_cast<char*>(sourceAddress);

    CSF_AT(uint32_t, self, 0x00) = CSF_AT(uint32_t, source, 0x00);

    CSF_AT(uint32_t, self, 0x1C) = 15;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(char, self, 0x08) = 0;
    AssignStringSubstring(self + 4, nullptr, source + 4, 0, -1);

    CSF_AT(uint32_t, self, 0x38) = 15;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(char, self, 0x24) = 0;
    AssignStringSubstring(self + 32, nullptr, source + 32, 0, -1);

    CSF_AT(uint32_t, self, 0x54) = 15;
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(char, self, 0x40) = 0;
    AssignStringSubstring(self + 60, nullptr, source + 60, 0, -1);

    CSF_AT(uint32_t, self, 0x70) = 15;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(char, self, 0x5C) = 0;
    AssignStringSubstring(self + 88, nullptr, source + 88, 0, -1);

    CSF_AT(uint32_t, self, 0x8C) = 15;
    CSF_AT(uint32_t, self, 0x88) = 0;
    CSF_AT(char, self, 0x78) = 0;
    AssignStringSubstring(self + 116, nullptr, source + 116, 0, -1);

    CSF_AT(uint32_t, self, 0xA8) = 15;
    CSF_AT(uint32_t, self, 0xA4) = 0;
    CSF_AT(char, self, 0x94) = 0;
    AssignStringSubstring(self + 144, nullptr, source + 144, 0, -1);

    CSF_AT(uint32_t, self, 0xAC) = CSF_AT(uint32_t, source, 0xAC);
    CSF_AT(uint32_t, self, 0xB0) = CSF_AT(uint32_t, source, 0xB0);
    CSF_AT(uint32_t, self, 0xB4) = CSF_AT(uint32_t, source, 0xB4);
    CSF_AT(uint32_t, self, 0xB8) = CSF_AT(uint32_t, source, 0xB8);
    CSF_AT(uint32_t, self, 0xBC) = CSF_AT(uint32_t, source, 0xBC);
    CSF_AT(uint32_t, self, 0xC0) = CSF_AT(uint32_t, source, 0xC0);
    CSF_AT(uint32_t, self, 0xC4) = CSF_AT(uint32_t, source, 0xC4);
    CSF_AT(uint32_t, self, 0xC8) = CSF_AT(uint32_t, source, 0xC8);
    CSF_AT(uint32_t, self, 0xCC) = CSF_AT(uint32_t, source, 0xCC);
    CSF_AT(uint32_t, self, 0xD0) = CSF_AT(uint32_t, source, 0xD0);
    CSF_AT(uint32_t, self, 0xD4) = CSF_AT(uint32_t, source, 0xD4);
    CSF_AT(uint32_t, self, 0xD8) = CSF_AT(uint32_t, source, 0xD8);
    CSF_AT(uint32_t, self, 0xDC) = CSF_AT(uint32_t, source, 0xDC);
    CSF_AT(uint32_t, self, 0xE0) = CSF_AT(uint32_t, source, 0xE0);
    CSF_AT(uint32_t, self, 0xE4) = CSF_AT(uint32_t, source, 0xE4);
    CSF_AT(uint32_t, self, 0xE8) = CSF_AT(uint32_t, source, 0xE8);
    CSF_AT(uint32_t, self, 0xEC) = CSF_AT(uint32_t, source, 0xEC);

    WeaponDefs_CopyTwelveByteVector(
        self + 240, nullptr, reinterpret_cast<int>(source + 240));
    MissionScript_ProcessEntry_4419F0(
        self + 256, nullptr, reinterpret_cast<uint32_t*>(source + 256));
    MissionScript_ProcessEntry_4419F0(
        self + 272, nullptr, reinterpret_cast<uint32_t*>(source + 272));

    CSF_AT(uint32_t, self, 0x120) = CSF_AT(uint32_t, source, 0x120);
    CSF_AT(uint32_t, self, 0x124) = CSF_AT(uint32_t, source, 0x124);
    CSF_AT(uint32_t, self, 0x128) = CSF_AT(uint32_t, source, 0x128);
    CSF_AT(uint32_t, self, 0x12C) = CSF_AT(uint32_t, source, 0x12C);
    CSF_AT(uint32_t, self, 0x130) = CSF_AT(uint32_t, source, 0x130);
    CSF_AT(uint32_t, self, 0x134) = CSF_AT(uint32_t, source, 0x134);
    CSF_AT(uint32_t, self, 0x138) = CSF_AT(uint32_t, source, 0x138);
    CSF_AT(uint32_t, self, 0x13C) = CSF_AT(uint32_t, source, 0x13C);
    CSF_AT(uint32_t, self, 0x140) = CSF_AT(uint32_t, source, 0x140);
    CSF_AT(uint32_t, self, 0x144) = CSF_AT(uint32_t, source, 0x144);
    CSF_AT(uint32_t, self, 0x148) = CSF_AT(uint32_t, source, 0x148);
    CSF_AT(uint32_t, self, 0x14C) = CSF_AT(uint32_t, source, 0x14C);
    CSF_AT(uint32_t, self, 0x150) = CSF_AT(uint32_t, source, 0x150);
    CSF_AT(uint32_t, self, 0x154) = CSF_AT(uint32_t, source, 0x154);

    CSF_AT(uint8_t, self, 0x158) = CSF_AT(uint8_t, source, 0x158);
    CSF_AT(uint8_t, self, 0x159) = CSF_AT(uint8_t, source, 0x159);
    CSF_AT(uint8_t, self, 0x15A) = CSF_AT(uint8_t, source, 0x15A);
    CSF_AT(uint8_t, self, 0x15B) = CSF_AT(uint8_t, source, 0x15B);
    CSF_AT(uint8_t, self, 0x15C) = CSF_AT(uint8_t, source, 0x15C);

    TypeRegistry_CopyEmbeddedDefinition(self + 352, nullptr, source + 352);

    CSF_AT(uint32_t, self, 0x1B0) = 15;
    CSF_AT(uint32_t, self, 0x1AC) = 0;
    CSF_AT(char, self, 0x19C) = 0;
    AssignStringSubstring(self + 408, nullptr, source + 408, 0, -1);

    // Preserve the original forward, dword-at-a-time copy even for overlap.
    for (uint32_t offset = 0; offset < 0x3C; offset += 4)
        CSF_AT(uint32_t, self + 436, offset) =
            CSF_AT(uint32_t, source + 436, offset);

    TypeRegistry_CopyTrailingDefinition(
        self + 496, nullptr, reinterpret_cast<int>(source + 496));

    return self;
}

HOOK(0x0054DA80, TypeRegistry_CopyCharacterDefinition, "auto");
