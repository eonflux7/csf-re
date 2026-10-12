#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t* __fastcall GfxLights_InitializeState(uint32_t* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x18) = 10;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 10;

    CSF_AT(uint8_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;
    CSF_AT(uint32_t, self, 0x54) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;
    CSF_AT(uint32_t, self, 0x60) = 0;
    CSF_AT(uint32_t, self, 0x64) = 0;
    CSF_AT(uint32_t, self, 0x6C) = 0;
    CSF_AT(uint32_t, self, 0x70) = 0;
    CSF_AT(uint32_t, self, 0x74) = 0;

    CSF_AT(uint8_t, self, 0x84) = 0;
    CSF_AT(uint32_t, self, 0x8C) = 0;
    CSF_AT(uint32_t, self, 0x90) = 0;
    CSF_AT(uint32_t, self, 0x94) = 0;
    CSF_AT(uint32_t, self, 0x9C) = 0;
    CSF_AT(uint32_t, self, 0xA0) = 0;
    CSF_AT(uint32_t, self, 0xA4) = 0;
    CSF_AT(uint32_t, self, 0xAC) = 0;
    CSF_AT(uint32_t, self, 0xB0) = 0;
    CSF_AT(uint32_t, self, 0xB4) = 0;

    CSF_AT(uint32_t, self, 0xD8) = 0;
    CSF_AT(uint32_t, self, 0xDC) = 0;
    CSF_AT(uint32_t, self, 0xE0) = 0;
    CSF_AT(uint32_t, self, 0xE4) = 10;
    CSF_AT(uint32_t, self, 0xE8) = 0;
    CSF_AT(uint32_t, self, 0xEC) = 0;
    CSF_AT(uint32_t, self, 0xF0) = 0;
    CSF_AT(uint32_t, self, 0xF4) = 0;
    CSF_AT(uint32_t, self, 0xF8) = 0;
    CSF_AT(uint32_t, self, 0xFC) = 10;
    CSF_AT(uint32_t, self, 0x100) = 0;
    CSF_AT(uint32_t, self, 0x104) = 0;
    return self;
}

HOOK(0x006D2640, GfxLights_InitializeState, "auto");
