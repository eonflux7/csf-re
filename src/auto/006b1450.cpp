#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0085e7b8_GetSceneRegionAt, 0x004FC130, (C_0085e7b8* self, void* , int index));
CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedValue, 0x006C4BF0, (void* self, void* , int key, unsigned int value));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

static int __fastcall S_006b1450_ApplySceneSkinSettings(
    S_006b1450* self, void* /*edx*/)
{
    alignas(8) uint8_t parameterBytes[0x8];
    CSF_AT(uint8_t, parameterBytes, 0x7) = self->field_0050;

    void* owner = reinterpret_cast<void*>(self->field_005c);
    void* state = CSF_AT(void*, owner, 0xBC);
    uint8_t* settings = reinterpret_cast<uint8_t*>(self) + 0x18;
    if (CSF_AT(uint8_t, state, 0x34))
    {
        CSF_AT(uint8_t, parameterBytes, 0x7) = 1;
        settings = reinterpret_cast<uint8_t*>(self) + 0x34;
    }

    uint32_t color =
        static_cast<uint32_t>(static_cast<int64_t>(CSF_AT(float, settings, 0x10)));
    color <<= 8;
    color |= static_cast<uint32_t>(
        static_cast<int64_t>(CSF_AT(float, settings, 0x4)));
    color <<= 8;
    color |= static_cast<uint32_t>(
        static_cast<int64_t>(CSF_AT(float, settings, 0x8)));
    color <<= 8;
    color |= static_cast<uint32_t>(
        static_cast<int64_t>(CSF_AT(float, settings, 0xC)));

    CSF_AT(int, parameterBytes, 0) = CSF_AT(int, settings, 0x14);
    int mode = CSF_AT(int, settings, 0);

    void* globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* region = reinterpret_cast<void*>(
        C_0085e7b8_GetSceneRegionAt(
            CSF_AT(C_0085e7b8*, globals, 0x4), nullptr, 0));

    GfxSkin_SetKeyedValue(
        region, nullptr, 14, CSF_AT(uint8_t, parameterBytes, 0x7) != 0);
    GfxSkin_SetKeyedValue(region, nullptr, 15, color);
    GfxSkin_SetKeyedValue(
        region, nullptr, 17,
        static_cast<unsigned int>(reinterpret_cast<uintptr_t>(parameterBytes)));

    unsigned int mappedMode = 0;
    switch (mode)
    {
    case 0:
        mappedMode = 1;
        break;
    case 1:
        mappedMode = 2;
        break;
    case 2:
        mappedMode = 3;
        break;
    }
    return GfxSkin_SetKeyedValue(region, nullptr, 16, mappedMode);
}

HOOK(0x006B1450, S_006b1450_ApplySceneSkinSettings, "auto");
