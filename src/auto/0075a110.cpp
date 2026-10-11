#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Global data referenced by the original (bound by address).
CSF_GLOBAL(uint32_t, g_DefaultParamA_8BB42C, 0x8BB42C);
CSF_GLOBAL(uint32_t, g_BlackClearColor, 0x8BB430);

// Resets a 0x38-byte configuration/params block to its default state.
// Offsets (bytes): 0x00=0, 0x04=g_DefaultParamA, 0x08=g_BlackClearColor,
// 0x0C=0, 0x10=0, 0x14=1.0f, 0x18=1.0f, 0x1C=0, 0x20=0, 0x24=1.0f,
// 0x28=1.0f, 0x2C=0, 0x30=0, 0x34=0.
static int __fastcall GameConfig_InitDefaults(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x04) = g_DefaultParamA_8BB42C;
    CSF_AT(uint32_t, self, 0x08) = g_BlackClearColor;

    CSF_AT(float, self, 0x18) = 1.0f;
    CSF_AT(float, self, 0x28) = 1.0f;
    CSF_AT(float, self, 0x14) = 1.0f;
    CSF_AT(float, self, 0x24) = 1.0f;

    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;

    return 0;
}

HOOK(0x75A110, GameConfig_InitDefaults, "auto");
