#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Constructor of a weapon definition record: initializes the embedded string members
// (empty, capacity 15) and zeroes the vector-like members.
static void* __fastcall WeaponDefs_InitWeaponRecord(void* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    // first string member at +0x04 (size +0x14, capacity +0x18)
    CSF_AT(int, base, 0x14) = 0;
    CSF_AT(int, base, 0x18) = 15;
    uint8_t* second = base + 0x1C;
    CSF_AT(uint8_t, base, 0x04) = 0;

    // second string member at +0x1C
    CSF_AT(int, second, 0x18) = 15;
    CSF_AT(int, second, 0x14) = 0;
    CSF_AT(uint8_t, second, 0x04) = 0;
    AssignStringRange_429B60(second, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);

    CSF_AT(int, base, 0x50) = 15;
    CSF_AT(int, base, 0x4C) = 0;
    CSF_AT(uint8_t, base, 0x3C) = 0;

    CSF_AT(int, base, 0x6C) = 15;
    CSF_AT(int, base, 0x68) = 0;
    CSF_AT(uint8_t, base, 0x58) = 0;

    CSF_AT(int, base, 0x9C) = 15;
    CSF_AT(int, base, 0x98) = 0;
    CSF_AT(uint8_t, base, 0x88) = 0;

    CSF_AT(int, base, 0xB8) = 15;
    CSF_AT(int, base, 0xB4) = 0;
    CSF_AT(uint8_t, base, 0xA4) = 0;

    CSF_AT(int, base, 0x13C) = 0;
    CSF_AT(int, base, 0x140) = 0;
    CSF_AT(int, base, 0x144) = 0;
    CSF_AT(int, base, 0x148) = 0;
    CSF_AT(int, base, 0x154) = 0;
    CSF_AT(int, base, 0x158) = 0;
    CSF_AT(int, base, 0x15C) = 0;
    CSF_AT(int, base, 0x164) = 0;
    CSF_AT(int, base, 0x168) = 0;
    CSF_AT(int, base, 0x16C) = 0;
    CSF_AT(int, base, 0x174) = 0;
    CSF_AT(int, base, 0x178) = 0;
    CSF_AT(int, base, 0x17C) = 0;
    CSF_AT(int, base, 0x184) = 0;
    CSF_AT(int, base, 0x188) = 0;
    CSF_AT(int, base, 0x18C) = 0;
    CSF_AT(int, base, 0x194) = 0;
    CSF_AT(int, base, 0x198) = 0;
    CSF_AT(int, base, 0x19C) = 0;

    return self;
}

HOOK(0x007836D0, WeaponDefs_InitWeaponRecord, "auto");
