#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t* __fastcall Hud_ConstructLinearRadar(uint32_t* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 10;

    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(uint32_t, self, 0x54) = 0;
    CSF_AT(uint32_t, self, 0x58) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;
    CSF_AT(uint32_t, self, 0x64) = 0;
    CSF_AT(uint32_t, self, 0x68) = 0;
    CSF_AT(uint8_t, self, 0x6C) = 0;
    CSF_AT(uint32_t, self, 0x70) = 0;
    CSF_AT(uint32_t, self, 0x74) = 0;
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(uint32_t, self, 0x7C) = 0;
    CSF_AT(float, self, 0x60) = -1.0f;

    CSF_AT(uint32_t, self, 0x84) = 0;
    CSF_AT(uint32_t, self, 0x88) = 0;
    CSF_AT(float, self, 0x80) = -1.0f;
    CSF_AT(uint8_t, self, 0x8C) = 0;
    CSF_AT(uint32_t, self, 0x90) = 0;
    CSF_AT(uint32_t, self, 0x94) = 0;
    CSF_AT(uint32_t, self, 0x98) = 0;
    CSF_AT(uint32_t, self, 0x9C) = 0;
    CSF_AT(uint32_t, self, 0xA4) = 0;
    CSF_AT(uint32_t, self, 0xA8) = 0;
    CSF_AT(float, self, 0xA0) = -1.0f;
    CSF_AT(uint8_t, self, 0xAC) = 0;

    CSF_AT(uint32_t, self, 0xB0) = 0;
    CSF_AT(uint32_t, self, 0xB4) = 0;
    CSF_AT(uint32_t, self, 0xB8) = 0;
    CSF_AT(uint32_t, self, 0xBC) = 0;
    CSF_AT(uint32_t, self, 0xC4) = 0;
    CSF_AT(uint32_t, self, 0xC8) = 0;
    CSF_AT(float, self, 0xC0) = -1.0f;
    CSF_AT(uint8_t, self, 0xCC) = 0;

    CSF_AT(uint32_t, self, 0xD4) = 0;
    CSF_AT(uint32_t, self, 0xD8) = 0;
    CSF_AT(uint32_t, self, 0xDC) = 0;
    CSF_AT(uint32_t, self, 0xE0) = 0;
    CSF_AT(uint32_t, self, 0xE4) = 0;
    CSF_AT(uint32_t, self, 0xE8) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint8_t, self, 0x4C) = 1;
    CSF_AT(uint8_t, self, 0x4D) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0xD0) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    return self;
}

HOOK(0x004399D0, Hud_ConstructLinearRadar, "auto");
