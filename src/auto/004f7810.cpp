#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, CharacterModel_ConfigureUpdate, 0x004F5910, (void* self, void* edx, char arg0, int a5));
CSF_FUNC(int, __fastcall, ObjectProvider_CommitPositioning, 0x004F71B0, (void* self, void* edx, float a4));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_DegreesToRadians, 0x0083F184);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall CharacterModel_SetUpdateMode(
    void* self, void* /*edx*/, int mode)
{
    const int previousMode = CSF_AT(int, self, 0x08);
    if (previousMode == mode)
        return previousMode;

    switch (previousMode)
    {
    case 8:
    {
        void* settings = reinterpret_cast<void*>(GetGlobalValue_753A30());
        float angle = CSF_AT(float, settings, 0x13C) * g_DegreesToRadians;

        void* overrides = reinterpret_cast<void*>(GetValue8B0554());
        if (CSF_AT(uint32_t, overrides, 0x90C) != 0xBF800000)
        {
            angle = CSF_AT(float,
                static_cast<uint8_t*>(overrides) + 0x54, 0x8B8)
                * g_DegreesToRadians;
        }

        void* provider = CSF_AT(void*, self, 0x04);
        ((void(__fastcall*)(void*, void*, float))
            CSF_AT(void**, provider, 0)[0x20 / 4])(
                provider, nullptr, angle);
        break;
    }
    case 9:
        CSF_AT(int, self, 0xD4) = -1;
        CSF_AT(int, self, 0xD8) = -1;
        CSF_AT(int, self, 0xE0) = -1;
        CSF_AT(int, self, 0xE4) = -1;
        break;
    }

    int result;
    switch (mode)
    {
    case 0:
    {
        void* resolved = reinterpret_cast<void*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x74)));

        alignas(8) uint8_t positionBytes[0x08];
        void* position = ((void*(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, resolved, 0)[0x38 / 4])(
                resolved, nullptr, positionBytes);

        CSF_AT(int, self, 0x9C) = CSF_AT(int, position, 0);
        result = CSF_AT(int, position, 4);
        CSF_AT(int, positionBytes, 0) = 0;
        CSF_AT(int, positionBytes, 4) = 0;
        CSF_AT(int, self, 0xA0) = result;
        CSF_AT(int, self, 0x8C) = 0;
        CSF_AT(int, self, 0x90) = 0;
        CSF_AT(int, self, 0x94) = 0;
        CSF_AT(int, self, 0x98) = 0;
        break;
    }
    case 2:
        result = CharacterModel_ConfigureUpdate(self, nullptr, 1, 0);
        break;
    case 8:
        result = ObjectProvider_CommitPositioning(self, nullptr, 0.0f);
        break;
    default:
        result = static_cast<int>(static_cast<uint32_t>(mode) - 8u);
        break;
    }

    CSF_AT(int, self, 0x08) = mode;
    return result;
}

HOOK(0x004F7810, CharacterModel_SetUpdateMode, "auto");
