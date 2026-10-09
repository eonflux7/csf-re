#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __fastcall, MenuAccounts_ResetConfiguration, 0x00565FA0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(uint32_t*, __cdecl, MenuMpOnline_GetAccountEditContext, 0x006A7420, ());
CSF_FUNC(int, __fastcall, SchedulerBase_GetSchedulerProxy, 0x00734A10, (void* self, void* edx));

CSF_GLOBAL(int, g_MenuMpOnlineSelectedAccountIndex, 0x89A5E8);
CSF_GLOBAL(uint32_t, g_MenuMpOnlineAccountEditScreenState, 0x89A5EC);
CSF_GLOBAL(int, g_MenuMpOnlineNetworkType, 0x89A5F0);
CSF_GLOBAL(uint32_t*, g_MenuMpOnlineAccountEditContext, 0x89A5FC);

static int __fastcall CMenuMpOnline_ProcessPendingOperation(
    void* self, void* /*edx*/, int /*unused*/)
{
    int result = SchedulerBase_GetSchedulerProxy(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);

    if (result == 4)
    {
        MenuAccounts_ResetConfiguration(
            &g_MenuMpOnlineSelectedAccountIndex, nullptr);
        g_MenuMpOnlineNetworkType = 3;
        g_MenuMpOnlineAccountEditScreenState = 2;
        g_MenuMpOnlineAccountEditContext = MenuMpOnline_GetAccountEditContext();

        void* networkState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        void** accountVtable = CSF_AT(void**, g_MenuMpOnlineAccountEditContext, 0);
        int account = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, networkState, 0)[63])(networkState, nullptr);

        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            accountVtable[1])(g_MenuMpOnlineAccountEditContext, nullptr, account);

        networkState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, networkState, 0)[64])(networkState, nullptr);

        void* menu = CSF_AT(void*, self, 0x14);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, menu, 0)[42])(menu, nullptr, 1);
        CSF_AT(int, self, 0x18) = 17;
    }
    else
    {
        result = SchedulerBase_GetSchedulerProxy(
            reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);

        if (result == 8)
        {
            void* networkState = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, networkState, 0)[64])(networkState, nullptr);
            CSF_AT(int, self, 0x18) = 4;
        }
    }

    return result;
}

HOOK(0x0055F550, CMenuMpOnline_ProcessPendingOperation, "auto");
