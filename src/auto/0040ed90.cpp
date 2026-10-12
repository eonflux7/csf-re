#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Resets the hit-info block embedded in a character action FSM to its idle defaults.
// Layout (offsets from self): 0x00 float max, 0x04-0x18 zeroed dwords, 0x1C -1,
// 0x20 byte 0xFF, 0x24 zero, 0x28 byte 0, 0x2C -1, 0x30 byte 0xFF, 0x5C zero,
// 0x60 zero, 0x64 = 10.
static uint32_t* __fastcall CharActionFsm_InitializeHitInfo(uint8_t* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x5C) = 0;
    CSF_AT(uint8_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0xFFFFFFFFu;
    CSF_AT(uint8_t, self, 0x30) = 0xFF;
    CSF_AT(uint32_t, self, 0x60) = 0;
    CSF_AT(uint32_t, self, 0x64) = 10;
    CSF_AT(float, self, 0x00) = FLT_MAX;
    CSF_AT(uint32_t, self, 0x1C) = 0xFFFFFFFFu;
    CSF_AT(uint8_t, self, 0x20) = 0xFF;
    CSF_AT(uint32_t, self, 0x24) = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0040ED90, CharActionFsm_InitializeHitInfo, "auto");
