#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __cdecl, NetGatheringDdl_PrepareNetworkAccount, 0x006A7350, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(char, __fastcall, C_0085e760_SetAccountScreen, 0x0073F120, (C_0085e760* self, void* , C_0085e760* accountScreen));

CSF_GLOBAL(uint32_t, g_dwAccountEditScreenState, 0x0089A5EC);
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);
CSF_GLOBAL(int, g_MpSessionState, 0x008B0690);

static int __fastcall CMenuMpOnline_RefreshForState(
    CMenuMpOnline* self, void* /*edx*/)
{
    auto* accountScreen = CSF_AT(
        C_0085e760*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x10);
    auto* accountManager =
        reinterpret_cast<C_0085e760*>(GetGlobalDword_73C020());
    C_0085e760_SetAccountScreen(accountManager, nullptr, accountScreen);

    int onlineMatch;
    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        onlineMatch = g_OnlineMatchFlag;
    }
    else
    {
        onlineMatch = g_MpNetworkType == 3;
    }

    if (onlineMatch && !GetGlobalState_6A6D70() &&
        static_cast<std::uint8_t>(NetGatheringDdl_PrepareNetworkAccount()))
    {
        void* screen = CSF_AT(
            void*, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x10);
        void* networkAccount =
            reinterpret_cast<void*>(GetGlobalState_6A6D70());
        void** screenVtable = CSF_AT(void**, screen, 0);
        void** accountVtable = CSF_AT(void**, networkAccount, 0);

        int screenData =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                screenVtable[0x64 / 4])(screen, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            accountVtable[0x9C / 4])(networkAccount, nullptr, screenData);
    }

    int menuRequest = 0;
    switch (g_MpSessionState)
    {
    case 1:
        menuRequest = 1380266033;
        break;
    case 2:
        menuRequest = 1380266034;
        break;
    case 3:
        menuRequest = 1380266032;
        break;
    case 4:
    case 6:
        menuRequest = 1380266035;
        break;
    case 5:
        self->field_0018 = 15;
        break;
    default:
        break;
    }

    if (menuRequest)
    {
        void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_DispatchMenuRequest(
            loader, nullptr, menuRequest, 32, 1, 0);
        GetGlobal8B0688();
        g_MpSessionState = 0;
    }

    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        onlineMatch = g_OnlineMatchFlag;
    }
    else
    {
        onlineMatch = g_MpNetworkType == 3;
    }

    if (onlineMatch && GetGlobalState_6A6D70())
    {
        void* networkAccount =
            reinterpret_cast<void*>(GetGlobalState_6A6D70());
        int accountReady =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, networkAccount, 0)[0x1C / 4])(
                    networkAccount, nullptr);
        if (accountReady)
        {
            self->field_0018 = 4;
        }
        else if (g_MpSessionState == 5)
        {
            self->field_0018 = 15;
        }
    }

    int result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        self->vtbl[0x58 / 4])(self, nullptr);

    if (g_dwAccountEditScreenState == 2)
    {
        void* screen = reinterpret_cast<void*>(self->field_0014);
        self->field_0018 = 17;
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

        screen = reinterpret_cast<void*>(self->field_0014);
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, screen, 0)[0xA8 / 4])(screen, nullptr, 1);
    }

    return result;
}

HOOK(0x0055ECA0, CMenuMpOnline_RefreshForState, "auto");
