#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_ZeroVector2X, 0x008BB448);
CSF_GLOBAL(float, g_ZeroVector2Y, 0x008BB44C);

static uint32_t Cheats_FloatBits(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static int __fastcall C_00830578_InitializeCheatScreen(
    C_00830578* self, void* /*edx*/)
{
    self->field_0020 = 1;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0.0f;
    self->field_0030 = 0.0f;
    self->field_003c = 1;
    self->field_003e = 0;
    self->field_003f = 1;
    self->field_0040 = 0;

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* activeObject = CSF_AT(void*, state, 0x10);
    if (activeObject) {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, activeObject, 0)[0x24 / 4])(activeObject, nullptr);
    }

    void* manager = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    void* context = CSF_AT(void*, manager, 0x18);
    void* settings = CSF_AT(void*, context, 0x10);
    self->field_00e0 = reinterpret_cast<uint32_t>(settings);

    float firstLimit = static_cast<float>(
        ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, settings, 0)[0x38 / 4])(settings, nullptr));
    settings = reinterpret_cast<void*>(self->field_00e0);
    float secondLimit = static_cast<float>(
        ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, settings, 0)[0x34 / 4])(settings, nullptr));
    self->field_0034 = secondLimit < firstLimit ? firstLimit : secondLimit;

    settings = reinterpret_cast<void*>(self->field_00e0);
    firstLimit = static_cast<float>(
        ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, settings, 0)[0x4C / 4])(settings, nullptr));
    settings = reinterpret_cast<void*>(self->field_00e0);
    secondLimit = static_cast<float>(
        ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, settings, 0)[0x48 / 4])(settings, nullptr));
    self->field_0038 = secondLimit < firstLimit ? firstLimit : secondLimit;

    self->field_0044 = CSF_AT(uint32_t,
        reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x18);
    self->field_0048 = CSF_AT(uint32_t,
        reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x1C);

    void* defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    self->field_004c = CSF_AT(uint32_t, defaults, 0x20);
    self->field_0050 = CSF_AT(uint32_t, defaults, 0x24);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    self->field_0054 = CSF_AT(uint32_t, defaults, 0x28);
    self->field_0058 = CSF_AT(uint32_t, defaults, 0x2C);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    self->field_005c = CSF_AT(uint32_t, defaults, 0x30);
    self->field_0060 = CSF_AT(uint32_t, defaults, 0x34);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    self->field_0064 = CSF_AT(uint32_t, defaults, 0x38);
    self->field_0068 = CSF_AT(uint32_t, defaults, 0x3C);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    self->field_006c = CSF_AT(uint32_t, defaults, 0x40);
    self->field_0070 = CSF_AT(uint32_t, defaults, 0x44);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    float x = CSF_AT(float, defaults, 0x48);
    float y = CSF_AT(float, defaults, 0x4C);
    self->field_0074 = Cheats_FloatBits(x + x);
    self->field_0078 = Cheats_FloatBits(y + y);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    x = CSF_AT(float, defaults, 0x50);
    y = CSF_AT(float, defaults, 0x54);
    self->field_007c = Cheats_FloatBits(x + x);
    self->field_0080 = Cheats_FloatBits(y + y);

    self->field_0084 = CSF_AT(uint32_t,
        reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x58);
    self->field_0088 = CSF_AT(uint32_t,
        reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x5C);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    self->field_008c = CSF_AT(uint32_t, defaults, 0x60);
    self->field_0090 = CSF_AT(uint32_t, defaults, 0x64);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    self->field_0094 = CSF_AT(uint32_t, defaults, 0x68);
    self->field_0098 = CSF_AT(uint32_t, defaults, 0x6C);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    self->field_009c = CSF_AT(uint32_t, defaults, 0x70);
    self->field_00a0 = CSF_AT(uint32_t, defaults, 0x74);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    self->field_00a4 = CSF_AT(uint32_t, defaults, 0x78);
    self->field_00a8 = CSF_AT(uint32_t, defaults, 0x7C);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    self->field_00ac = CSF_AT(uint32_t, defaults, 0x80);
    self->field_00b0 = CSF_AT(uint32_t, defaults, 0x84);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    x = CSF_AT(float, defaults, 0x88);
    y = CSF_AT(float, defaults, 0x8C);
    self->field_00b4 = Cheats_FloatBits(x + x);
    self->field_00b8 = Cheats_FloatBits(y + y);

    defaults = reinterpret_cast<void*>(GetGlobalValue_753A30());
    x = CSF_AT(float, defaults, 0x90);
    y = CSF_AT(float, defaults, 0x94);
    self->field_00bc = Cheats_FloatBits(x + x);
    self->field_00c0 = Cheats_FloatBits(y + y);

    self->field_00c4 = Cheats_FloatBits(g_ZeroVector2X);
    self->field_00c8 = Cheats_FloatBits(g_ZeroVector2Y);
    self->field_00cc = Cheats_FloatBits(g_ZeroVector2X);
    self->field_00d0 = Cheats_FloatBits(g_ZeroVector2Y);
    settings = reinterpret_cast<void*>(self->field_00e0);
    self->field_00d4 = Cheats_FloatBits(g_ZeroVector2X);
    self->field_00d8 = Cheats_FloatBits(g_ZeroVector2Y);

    uint8_t enabled = ((uint8_t(__fastcall*)(void*, void*))
        CSF_AT(void**, settings, 0)[0x28 / 4])(settings, nullptr);
    void* provider = reinterpret_cast<void*>(self->field_0014);
    self->field_003d = enabled;
    int result = ((int(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, provider, 0)[0])(provider, nullptr, &self->field_00e4);
    self->field_00f4 = static_cast<uint8_t>(result);
    return result;
}

HOOK(0x0042B380, C_00830578_InitializeCheatScreen, "auto");
