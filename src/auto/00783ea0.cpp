#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, WeaponDefs_InitializeEmbeddedData, 0x007836D0, (int a1, int a2));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static char* __fastcall WeaponDefs_InitializeDefaults(char* self, void* /*edx*/)
{
    WeaponDefs_InitializeEmbeddedData(reinterpret_cast<int>(self + 4), 0);

    CSF_AT(int32_t, self, 0x1E4) = 0;
    CSF_AT(int32_t, self, 0x1E8) = 0;
    CSF_AT(int32_t, self, 0x1EC) = 0;
    CSF_AT(int32_t, self, 0x1D8) = 0;
    CSF_AT(int32_t, self, 0x1DC) = 0;
    CSF_AT(int32_t, self, 0x1E0) = 0;
    CSF_AT(int32_t, self, 0x20C) = 15;
    CSF_AT(int32_t, self, 0x208) = 0;
    CSF_AT(uint8_t, self, 0x1F8) = 0;
    CSF_AT(int32_t, self, 0x210) = 0;
    CSF_AT(int32_t, self, 0x214) = 0;
    CSF_AT(int32_t, self, 0x218) = 0;
    CSF_AT(int32_t, self, 0x000) = -1;

    AssignStringRange_429B60(self + 32, nullptr,
                           reinterpret_cast<char*>(&g_byte_82E65C), 0);

    CSF_AT(float, self, 0x0F0) = 360.0f;
    CSF_AT(float, self, 0x0F4) = 360.0f;
    CSF_AT(int32_t, self, 0x0D4) = 1;
    CSF_AT(int32_t, self, 0x150) = 1;
    CSF_AT(int32_t, self, 0x130) = -1;
    CSF_AT(int32_t, self, 0x134) = -1;
    CSF_AT(int32_t, self, 0x138) = -1;
    CSF_AT(int32_t, self, 0x13C) = -1;
    CSF_AT(int32_t, self, 0x074) = 0;
    CSF_AT(int32_t, self, 0x078) = 0;
    CSF_AT(int32_t, self, 0x07C) = 0;
    CSF_AT(int32_t, self, 0x080) = 0;
    CSF_AT(int32_t, self, 0x084) = -1;
    CSF_AT(int32_t, self, 0x0D8) = 0;
    CSF_AT(int32_t, self, 0x0DC) = 0;
    CSF_AT(int32_t, self, 0x0E0) = 0;
    CSF_AT(uint8_t, self, 0x0E4) = 0;
    CSF_AT(float, self, 0x0E8) = 20000.0f;
    CSF_AT(float, self, 0x0EC) = 30000.0f;
    CSF_AT(float, self, 0x0F8) = 1.0f;
    CSF_AT(float, self, 0x0FC) = 3.0f;
    CSF_AT(float, self, 0x100) = 1.0f;
    CSF_AT(int32_t, self, 0x104) = 0;
    CSF_AT(int32_t, self, 0x108) = 0;
    CSF_AT(int32_t, self, 0x10C) = 0;
    CSF_AT(int32_t, self, 0x110) = 10;
    CSF_AT(int32_t, self, 0x114) = 20;
    CSF_AT(int32_t, self, 0x118) = 0;
    CSF_AT(int32_t, self, 0x11C) = 200;
    CSF_AT(int32_t, self, 0x120) = 0;
    CSF_AT(int32_t, self, 0x124) = 0;
    CSF_AT(int32_t, self, 0x128) = 0;
    CSF_AT(int32_t, self, 0x12C) = 0;
    CSF_AT(float, self, 0x1A4) = 5.0f;
    CSF_AT(float, self, 0x1A8) = 1500.0f;
    CSF_AT(float, self, 0x1AC) = 30.0f;
    CSF_AT(float, self, 0x1B0) = 1000.0f;
    CSF_AT(int32_t, self, 0x1B4) = 0;
    CSF_AT(float, self, 0x1B8) = 0.5f;
    CSF_AT(float, self, 0x1BC) = 0.65f;
    CSF_AT(float, self, 0x1C0) = 1.7f;
    CSF_AT(float, self, 0x1C4) = 5.0f;
    CSF_AT(float, self, 0x1C8) = 3.0f;
    CSF_AT(float, self, 0x1CC) = 1.0f;
    CSF_AT(float, self, 0x1D0) = 3.0f;
    CSF_AT(float, self, 0x1D4) = 10.0f;
    CSF_AT(int32_t, self, 0x0C0) = 0;
    CSF_AT(float, self, 0x0CC) = 10.0f;
    CSF_AT(float, self, 0x0D0) = 10.0f;
    CSF_AT(float, self, 0x0C4) = 1.0f;
    CSF_AT(float, self, 0x0C8) = 100.0f;
    CSF_AT(int32_t, self, 0x1F0) = 0;

    AssignStringRange_429B60(self + 500, nullptr,
                           reinterpret_cast<char*>(&g_byte_82E65C), 0);

    CSF_AT(float, self, 0x21C) = 1.0f;
    CSF_AT(int32_t, self, 0x220) = 0;
    CSF_AT(float, self, 0x224) = 0.1f;
    return self;
}

HOOK(0x00783EA0, WeaponDefs_InitializeDefaults, "auto");
