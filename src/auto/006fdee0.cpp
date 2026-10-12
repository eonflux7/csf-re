#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Constructor for a 137+ byte water-parameter block. Every field is written in the
// original's order (the 0x6C, 0x70 and 0x74 slots are written twice, as in the disassembly).
static uint32_t* __fastcall WaterParams_Construct(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x00) = 0xff000000u;
    CSF_AT(float, self, 0x10) = 20.0f;
    CSF_AT(uint32_t, self, 0x04) = 0xffffffffu;
    CSF_AT(uint32_t, self, 0x08) = 0xffffffffu;
    CSF_AT(uint32_t, self, 0x1C) = 0u;
    CSF_AT(uint32_t, self, 0x20) = 0u;
    CSF_AT(float, self, 0x18) = 0.1f;
    CSF_AT(float, self, 0x24) = 0.1f;
    CSF_AT(float, self, 0x2C) = 0.1f;
    CSF_AT(float, self, 0x0C) = 1.0f;
    CSF_AT(float, self, 0x14) = 1.0f;
    CSF_AT(float, self, 0x28) = 1.0f;
    CSF_AT(uint32_t, self, 0x48) = 15u;
    CSF_AT(uint32_t, self, 0x44) = 0u;
    CSF_AT(uint8_t, self, 0x34) = 0u;
    CSF_AT(uint32_t, self, 0x4C) = 0u;
    CSF_AT(float, self, 0x50) = 0.1f;
    CSF_AT(float, self, 0x54) = 0.1f;
    CSF_AT(float, self, 0x58) = 0.3f;
    CSF_AT(uint8_t, self, 0x5C) = 0u;
    CSF_AT(uint32_t, self, 0x6C) = 0u;
    CSF_AT(uint32_t, self, 0x70) = 0u;
    CSF_AT(uint32_t, self, 0x74) = 0u;
    CSF_AT(uint32_t, self, 0x60) = 0u;
    CSF_AT(uint32_t, self, 0x64) = 0u;
    CSF_AT(uint32_t, self, 0x68) = 0u;
    CSF_AT(float, self, 0x6C) = 1.0f;
    CSF_AT(uint32_t, self, 0x70) = 0u;
    CSF_AT(uint32_t, self, 0x74) = 0u;
    CSF_AT(uint8_t, self, 0x88) = 1u;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006FDEE0, WaterParams_Construct, "auto");
