#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callees of HudAlertIcons_InitStateByType (bound by address, copied from the packet).
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, GameConfig_InitDefaults, 0x0075A110, (void* self, void*));

// Shared empty-string data referenced by the original.
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
// Default parameter word and black clear color used by the per-type parameter setups.
CSF_GLOBAL(uint32_t, g_DefaultParamA_8BB42C, 0x008BB42C);
CSF_GLOBAL(uint32_t, g_BlackClearColor, 0x008BB430);

// Per-type parameter setups. The original tail-jumps into small writers that take
// ecx = this + 0x4C; their bodies are written out here at the same offsets.
static int __fastcall AlertParams_SetupModeZero(uint8_t* params)
{
    CSF_AT(uint8_t, params, 0x00) = 0;
    CSF_AT(uint8_t, params, 0x01) = 0;
    CSF_AT(uint8_t, params, 0x02) = 0;
    CSF_AT(uint32_t, params, 0x03) = g_DefaultParamA_8BB42C;
    CSF_AT(uint32_t, params, 0x07) = g_DefaultParamA_8BB42C;
    CSF_AT(uint32_t, params, 0x0C) = 0xFF;
    CSF_AT(uint32_t, params, 0x10) = 0xFF;
    CSF_AT(uint8_t, params, 0x14) = 0;
    CSF_AT(uint8_t, params, 0x15) = 0;
    return 0; // eax still holds the switch index (0)
}

static int __fastcall AlertParams_SetupModeOne(uint8_t* params)
{
    CSF_AT(uint32_t, params, 0x00) = g_DefaultParamA_8BB42C;
    CSF_AT(uint32_t, params, 0x04) = g_DefaultParamA_8BB42C;
    CSF_AT(float, params, 0x08) = 100.0f;
    CSF_AT(float, params, 0x10) = 4000.0f;
    CSF_AT(float, params, 0x14) = 1.0f;
    CSF_AT(float, params, 0x0C) = 1.0f;
    return 0x3F800000; // eax left holding 1.0f
}

static int __fastcall AlertParams_SetupModeTwo(uint8_t* params)
{
    CSF_AT(uint32_t, params, 0x00) = 0;
    CSF_AT(uint32_t, params, 0x04) = 0;
    CSF_AT(uint32_t, params, 0x08) = g_DefaultParamA_8BB42C;
    CSF_AT(uint8_t, params, 0x34) = 0;
    CSF_AT(uint32_t, params, 0x0C) = 0;
    CSF_AT(uint32_t, params, 0x24) = 1;
    CSF_AT(uint32_t, params, 0x28) = 1;
    CSF_AT(uint32_t, params, 0x2C) = 1;
    CSF_AT(uint32_t, params, 0x30) = 1;
    CSF_AT(uint32_t, params, 0x10) = 0;
    CSF_AT(uint32_t, params, 0x14) = 0;
    CSF_AT(uint32_t, params, 0x18) = 0;
    CSF_AT(uint32_t, params, 0x1C) = 0;
    CSF_AT(uint32_t, params, 0x20) = 0;
    return 0;
}

static int __fastcall AlertParams_SetupModeFour(uint8_t* params)
{
    CSF_AT(uint32_t, params, 0x00) = 0;
    CSF_AT(uint32_t, params, 0x04) = 0;
    CSF_AT(uint32_t, params, 0x0C) = g_DefaultParamA_8BB42C;
    CSF_AT(float, params, 0x08) = 10.0f;
    CSF_AT(uint32_t, params, 0x10) = 0;
    CSF_AT(uint32_t, params, 0x14) = 0;
    CSF_AT(float, params, 0x18) = 100.0f;
    CSF_AT(uint32_t, params, 0x1C) = 0;
    CSF_AT(uint32_t, params, 0x20) = 0;
    CSF_AT(uint32_t, params, 0x24) = 0;
    return 0;
}

static int __fastcall AlertParams_SetupModeFive(uint8_t* params)
{
    CSF_AT(uint32_t, params, 0x00) = g_DefaultParamA_8BB42C;
    CSF_AT(uint32_t, params, 0x15) = g_BlackClearColor;
    CSF_AT(float, params, 0x04) = 100.0f;
    CSF_AT(uint32_t, params, 0x08) = 0;
    CSF_AT(uint32_t, params, 0x0C) = 0;
    CSF_AT(uint32_t, params, 0x10) = 0;
    CSF_AT(uint8_t, params, 0x14) = 0;
    CSF_AT(uint32_t, params, 0x1C) = 0;
    CSF_AT(uint32_t, params, 0x20) = 0;
    CSF_AT(uint32_t, params, 0x24) = 0;
    return static_cast<int>(g_DefaultParamA_8BB42C); // eax holds the default param
}

static int __fastcall HudAlertIcons_InitStateByType(void* self, void* /*edx*/)
{
    AssignStringRange_429B60(static_cast<uint8_t*>(self) + 0x04, nullptr,
                             reinterpret_cast<char*>(&g_byte_82E65C), 0);
    AssignStringRange_429B60(static_cast<uint8_t*>(self) + 0x20, nullptr,
                             reinterpret_cast<char*>(&g_byte_82E65C), 0);

    CSF_AT(uint8_t, self, 0x43) = 1;
    CSF_AT(uint8_t, self, 0x44) = 1;

    const int alertType = CSF_AT(int, self, 0x00);

    CSF_AT(int, self, 0x3C) = -1;
    CSF_AT(uint8_t, self, 0x40) = 0;
    CSF_AT(uint8_t, self, 0x41) = 0;
    CSF_AT(uint8_t, self, 0x42) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;

    uint8_t* params = static_cast<uint8_t*>(self) + 0x4C;

    switch (alertType)
    {
    case 0:
        return AlertParams_SetupModeZero(params);
    case 1:
        return AlertParams_SetupModeOne(params);
    case 2:
        return AlertParams_SetupModeTwo(params);
    case 3:
        return GameConfig_InitDefaults(params, nullptr);
    case 4:
        return AlertParams_SetupModeFour(params);
    case 5:
        return AlertParams_SetupModeFive(params);
    default:
        return alertType;
    }
}

HOOK(0x004A0A60, HudAlertIcons_InitStateByType, "auto");
