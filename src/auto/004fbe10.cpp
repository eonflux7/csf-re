#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

static char __fastcall CommandoAnims_PrepareRegionManagers(void* self, void* /*edx*/)
{
    alignas(8) uint8_t requestBytes[0xC];

    void* settings = reinterpret_cast<void*>(GetGlobalValue_8B0568());
    CSF_AT(uint16_t, requestBytes, 0x8) = 257;
    CSF_AT(uint8_t, requestBytes, 0xA) = 1;
    if (!CSF_AT(uint8_t, settings, 0xF5))
        CSF_AT(uint8_t, requestBytes, 0x8) = 0;

    void* gameState = reinterpret_cast<void*>(GetValue8B0554());
    void* regionState = CSF_AT(void*, gameState, 0x1FDC);
    if (!CSF_AT(uint8_t, regionState, 0x2C))
        CSF_AT(uint8_t, requestBytes, 0xA) = 0;

    CSF_AT(int, requestBytes, 0x4) = 0;
    CSF_AT(int, requestBytes, 0x0) = 6;

    char result = 0;
    for (unsigned int region = 0; region < 3; ++region)
    {
        result = static_cast<char>(CSF_AT(uint8_t, requestBytes, 0x8 + region));
        if (!result)
            continue;

        void*& regionManager = CSF_AT(void*, self, 0x60 + region * 4);
        if (regionManager)
            continue;

        auto* regionParameters =
            reinterpret_cast<uint8_t*>(self) + 0x90 + region * 0x10;
        CSF_AT(int, requestBytes, 0x4) = CSF_AT(int, regionParameters, 0x0);

        int globalManager = GetGlobalInt();
        void* factory = reinterpret_cast<void*>(ResolveRwManager(globalManager, nullptr));
        void* created =
            ((void*(__fastcall*)(void*, void*, int*))
                CSF_AT(void**, factory, 0)[0x20 / 4])(
                    factory, nullptr, reinterpret_cast<int*>(requestBytes));

        regionManager = created;
        if (!created)
        {
            result = 0;
            continue;
        }

        ((int(__fastcall*)(void*, void*, int, uint32_t))
            CSF_AT(void**, created, 0)[0x1C / 4])(
                created, nullptr, 0, CSF_AT(uint32_t, regionParameters, 0x4));

        void* current = regionManager;
        ((int(__fastcall*)(void*, void*, int, uint32_t))
            CSF_AT(void**, current, 0)[0x1C / 4])(
                current, nullptr, 1, CSF_AT(uint32_t, regionParameters, 0x8));

        current = regionManager;
        result = static_cast<char>(
            ((int(__fastcall*)(void*, void*, int, uint32_t))
                CSF_AT(void**, current, 0)[0x1C / 4])(
                    current, nullptr, 2, CSF_AT(uint32_t, regionParameters, 0xC)));
    }
    return result;
}

HOOK(0x004FBE10, CommandoAnims_PrepareRegionManagers, "auto");
