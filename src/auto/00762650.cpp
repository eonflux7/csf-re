#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));

static char* __fastcall CharacterDefs_InitializeCharacterData(char* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x1C) = 15;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(char, self, 0x08) = 0;
    AssignStringRange_429B60(self + 4, nullptr, const_cast<char*>(CSF_STR(0x0082E65C, "")), 0);

    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 15;
    CSF_AT(char, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x54) = 15;
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(char, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x70) = 15;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(char, self, 0x5C) = 0;
    CSF_AT(uint32_t, self, 0x8C) = 15;
    CSF_AT(uint32_t, self, 0x88) = 0;
    CSF_AT(char, self, 0x78) = 0;
    CSF_AT(uint32_t, self, 0xA8) = 15;
    CSF_AT(uint32_t, self, 0xA4) = 0;
    CSF_AT(char, self, 0x94) = 0;

    CSF_AT(uint32_t, self, 0xF4) = 0;
    CSF_AT(uint32_t, self, 0xF8) = 0;
    CSF_AT(uint32_t, self, 0xFC) = 0;
    CSF_AT(uint32_t, self, 0x100) = 0;
    CSF_AT(uint32_t, self, 0x104) = 0;
    CSF_AT(uint32_t, self, 0x108) = 0;
    CSF_AT(uint32_t, self, 0x10C) = 10;
    CSF_AT(uint32_t, self, 0x11C) = 10;
    CSF_AT(uint32_t, self, 0x110) = 0;
    CSF_AT(uint32_t, self, 0x114) = 0;
    CSF_AT(uint32_t, self, 0x118) = 0;
    CSF_AT(uint32_t, self, 0x13C) = 0;
    CSF_AT(uint32_t, self, 0x140) = 0;
    CSF_AT(uint32_t, self, 0x144) = 0;
    CSF_AT(uint32_t, self, 0x130) = 0;
    CSF_AT(uint32_t, self, 0x134) = 0;
    CSF_AT(uint32_t, self, 0x138) = 0;
    CSF_AT(uint32_t, self, 0x160) = 0;
    CSF_AT(uint32_t, self, 0x164) = 0;
    CSF_AT(uint32_t, self, 0x168) = 0;

    CSF_AT(uint32_t, self, 0x184) = 15;
    CSF_AT(uint32_t, self, 0x180) = 0;
    CSF_AT(char, self, 0x170) = 0;
    CSF_AT(uint32_t, self, 0x18C) = 0;
    CSF_AT(uint32_t, self, 0x190) = 0;
    CSF_AT(uint32_t, self, 0x194) = 0;
    CSF_AT(uint32_t, self, 0x1B0) = 15;
    CSF_AT(uint32_t, self, 0x1AC) = 0;
    CSF_AT(char, self, 0x19C) = 0;
    CSF_AT(uint32_t, self, 0x20C) = 15;
    CSF_AT(uint32_t, self, 0x208) = 0;
    CSF_AT(char, self, 0x1F8) = 0;
    CSF_AT(uint32_t, self, 0x210) = 0;
    CSF_AT(uint32_t, self, 0x214) = 0;
    CSF_AT(uint32_t, self, 0x218) = 0;

    CSF_AT(int32_t, self, 0x00) = -1;
    AssignStringRange_429B60(self + 4, nullptr, const_cast<char*>(CSF_STR(0x0082E65C, "")), 0);

    CSF_AT(int32_t, self, 0xB4) = 1;
    CSF_AT(int32_t, self, 0xB8) = 0;
    CSF_AT(int32_t, self, 0xBC) = 0;
    CSF_AT(int32_t, self, 0xC0) = -1;
    CSF_AT(int32_t, self, 0xC4) = -1;
    CSF_AT(int32_t, self, 0xC8) = -1;
    CSF_AT(int32_t, self, 0xCC) = 0;
    CSF_AT(int32_t, self, 0xD0) = 0;
    CSF_AT(int32_t, self, 0xD4) = -1;
    CSF_AT(int32_t, self, 0xD8) = 0;
    CSF_AT(int32_t, self, 0xDC) = 0;
    CSF_AT(int32_t, self, 0xE0) = 0;
    CSF_AT(int32_t, self, 0xAC) = 0;
    CSF_AT(int32_t, self, 0xB0) = 0;
    CSF_AT(char, self, 0x158) = 0;
    CSF_AT(uint32_t, self, 0x160) = 0;
    CSF_AT(uint32_t, self, 0x164) = 0;
    CSF_AT(uint32_t, self, 0x168) = 0;

    AssignStringRange_429B60(self + 364, nullptr, const_cast<char*>(CSF_STR(0x0082E65C, "")), 0);

    CSF_AT(uint32_t, self, 0x188) = 0;
    CSF_AT(uint32_t, self, 0x18C) = 0;
    CSF_AT(uint32_t, self, 0x190) = 0;
    CSF_AT(uint32_t, self, 0x194) = 0;
    CSF_AT(uint32_t, self, 0xE4) = 0;
    CSF_AT(uint32_t, self, 0xE8) = 0;
    CSF_AT(uint32_t, self, 0xEC) = 0;

    CSF_AT(float, self, 0x120) = 120.0f;
    CSF_AT(float, self, 0x124) = 2000.0f;
    CSF_AT(float, self, 0x128) = 5000.0f;
    CSF_AT(int32_t, self, 0x12C) = 100;
    CSF_AT(float, self, 0x1B4) = 200.0f;
    CSF_AT(float, self, 0x1B8) = 100.0f;
    CSF_AT(float, self, 0x1BC) = 300.0f;
    CSF_AT(float, self, 0x1C0) = 500.0f;
    CSF_AT(uint32_t, self, 0x1C4) = 0;
    CSF_AT(uint32_t, self, 0x1C8) = 0;
    CSF_AT(uint32_t, self, 0x1CC) = 0;
    CSF_AT(uint32_t, self, 0x1D0) = 0;
    CSF_AT(float, self, 0x1D4) = 300.0f;
    CSF_AT(float, self, 0x1D8) = 0.4f;
    CSF_AT(float, self, 0x1DC) = 1.0f;
    CSF_AT(float, self, 0x1E0) = 1.0f;
    CSF_AT(float, self, 0x1E4) = 1.0f;
    CSF_AT(float, self, 0x1E8) = 1.0f;
    CSF_AT(float, self, 0x1EC) = 1.0f;
    CSF_AT(uint32_t, self, 0x1F0) = 0;

    AssignStringRange_429B60(self + 500, nullptr, const_cast<char*>(CSF_STR(0x0082E65C, "")), 0);

    CSF_AT(float, self, 0x220) = 1.0f;
    CSF_AT(char, self, 0x21C) = 0;
    CSF_AT(char, self, 0x21D) = 0;
    CSF_AT(uint32_t, self, 0x224) = 0;
    CSF_AT(char, self, 0x159) = 0;
    CSF_AT(char, self, 0x15A) = 0;
    CSF_AT(char, self, 0x15B) = 0;
    CSF_AT(uint32_t, self, 0x14C) = 0;
    CSF_AT(float, self, 0x228) = 0.5f;
    CSF_AT(int32_t, self, 0x148) = -1;
    CSF_AT(int32_t, self, 0x150) = 600;
    CSF_AT(float, self, 0x154) = 600.0f;
    CSF_AT(char, self, 0x15C) = 1;

    return self;
}

HOOK(0x00762650, CharacterDefs_InitializeCharacterData, "auto");
