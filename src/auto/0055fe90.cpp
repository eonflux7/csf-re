#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, MenuMpOnline_RunUpdateNotification, 0x00506AC0, ());
CSF_FUNC(void*, __cdecl, MenuMpOnline_GetMenuState, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, MenuMpOnline_GetUpdateManager, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuMpOnline_UpdateManager, 0x005231D0,
         (void* self, void* edx, int updateArgument));
CSF_FUNC(void*, __cdecl, MenuMpOnline_GetOnlineContext, 0x00550CC0, ());
CSF_FUNC(void*, __cdecl, MenuMpOnline_GetNetworkState, 0x006A6D70, ());
CSF_FUNC(void*, __cdecl, MenuMpOnline_GetNetworkManager, 0x0073C020, ());
CSF_FUNC(char, __fastcall, MenuMpOnline_UpdateNetworkManager, 0x0073D550,
         (void* self, void* edx, int updateArgument));

CSF_FUNC(void, __fastcall, CMenuMpOnline_UpdateTransition, 0x0055EE90,
         (CMenuMpOnline* self, void* edx, float updateArgument));
CSF_FUNC(void*, __fastcall, CMenuMpOnline_DispatchSchedulerProxyThunk, 0x0055EEE0,
         (CMenuMpOnline* self, void* edx));
CSF_FUNC(void*, __fastcall, CMenuMpOnline_ProcessMatchSelection, 0x0055F0B0,
         (CMenuMpOnline* self, void* edx, int updateArgument));
CSF_FUNC(int, __fastcall, CMenuMpOnline_HandleOnlineState, 0x0055F360,
         (CMenuMpOnline* self, void* edx, int updateArgument));
CSF_FUNC(void*, __fastcall, CMenuMpOnline_InvokeSchedulerProxyCallback, 0x0055F390,
         (CMenuMpOnline* self, void* edx, int updateArgument));
CSF_FUNC(void*, __fastcall, CMenuMpOnline_DispatchOnSchedulerProxy, 0x0055F3B0,
         (CMenuMpOnline* self, void* edx, int updateArgument));
CSF_FUNC(void*, __fastcall, CMenuMpOnline_DispatchOnlineState, 0x0055F410,
         (CMenuMpOnline* self, void* edx, int updateArgument));
CSF_FUNC(void*, __fastcall, CMenuMpOnline_CheckSchedulerProxyAndDispatch, 0x0055F470,
         (CMenuMpOnline* self, void* edx, int updateArgument));
CSF_FUNC(void*, __fastcall, CMenuMpOnline_ResolveSchedulerProxy, 0x0055F4C0,
         (CMenuMpOnline* self, void* edx, int updateArgument));
CSF_FUNC(void*, __fastcall, CMenuMpOnline_ResolveAlternateSchedulerProxy, 0x0055F4F0,
         (CMenuMpOnline* self, void* edx, int updateArgument));
CSF_FUNC(void*, __fastcall, CMenuMpOnline_ProcessPendingOperation, 0x0055F550,
         (CMenuMpOnline* self, void* edx, int updateArgument));
CSF_FUNC(unsigned int, __fastcall, CMenuMpOnline_ProcessMatchSearch, 0x0055F610,
         (CMenuMpOnline* self, void* edx, int updateArgument));
CSF_FUNC(int, __fastcall, CMenuMpOnline_OpenAuUnPaMenu, 0x0055F8C0,
         (CMenuMpOnline* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpOnline_RouteNavigation, 0x0055F920,
         (CMenuMpOnline* self, void* edx, int updateArgument));
CSF_FUNC(unsigned int, __fastcall, CMenuMpOnline_ProcessInitialState, 0x0055FCB0,
         (CMenuMpOnline* self, void* edx, int updateArgument));

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

static void __fastcall CMenuMpOnline_UpdateAndDispatch(
    CMenuMpOnline* self, void* /*edx*/, int updateArgument)
{
    void* child = reinterpret_cast<void*>(self->field_0014);
    if (child &&
        !reinterpret_cast<std::uint8_t(__fastcall*)(void*, void*)>(
            CSF_AT(void**, child, 0)[0xBC / 4])(child, nullptr))
    {
        child = reinterpret_cast<void*>(self->field_0014);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, child, 0)[0xB8 / 4])(child, nullptr, 1);
    }

    void* manager = MenuMpOnline_GetUpdateManager();
    MenuMpOnline_UpdateManager(manager, nullptr, updateArgument);
    MenuMpOnline_RunUpdateNotification();

    manager = MenuMpOnline_GetNetworkManager();
    MenuMpOnline_UpdateNetworkManager(manager, nullptr, updateArgument);

    int updateNetworkState;
    if (MenuMpOnline_GetOnlineContext())
    {
        MenuMpOnline_GetOnlineContext();
        updateNetworkState = g_OnlineMatchFlag;
    }
    else
    {
        updateNetworkState = g_MpNetworkType == 3;
    }

    if (updateNetworkState)
    {
        void* networkState = MenuMpOnline_GetNetworkState();
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, networkState, 0)[0x18 / 4])(
                networkState, nullptr, updateArgument);
    }

    void* menuState = MenuMpOnline_GetMenuState();
    if (reinterpret_cast<std::uint8_t(__fastcall*)(void*, void*)>(
            CSF_AT(void**, menuState, 0)[0x6C / 4])(menuState, nullptr))
    {
        return;
    }

    switch (self->field_0018)
    {
    case 0:
        CMenuMpOnline_ProcessInitialState(self, nullptr, updateArgument);
        break;
    case 2:
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, static_cast<void*>(self), 0)[0x50 / 4])(
                self, nullptr, updateArgument);
        break;
    case 3:
        CMenuMpOnline_ProcessMatchSelection(self, nullptr, updateArgument);
        break;
    case 4:
        CMenuMpOnline_RouteNavigation(self, nullptr, updateArgument);
        break;
    case 5:
        CMenuMpOnline_HandleOnlineState(self, nullptr, updateArgument);
        break;
    case 6:
        CMenuMpOnline_InvokeSchedulerProxyCallback(self, nullptr, updateArgument);
        break;
    case 7:
        CMenuMpOnline_ProcessMatchSearch(self, nullptr, updateArgument);
        break;
    case 8:
        CMenuMpOnline_DispatchOnSchedulerProxy(self, nullptr, updateArgument);
        break;
    case 9:
        CMenuMpOnline_DispatchOnlineState(self, nullptr, updateArgument);
        break;
    case 10:
        CMenuMpOnline_CheckSchedulerProxyAndDispatch(self, nullptr, updateArgument);
        break;
    case 11:
        CMenuMpOnline_ResolveSchedulerProxy(self, nullptr, updateArgument);
        break;
    case 12:
        CMenuMpOnline_ResolveAlternateSchedulerProxy(self, nullptr, updateArgument);
        break;
    case 13:
        CMenuMpOnline_ProcessPendingOperation(self, nullptr, updateArgument);
        break;
    case 14:
        CMenuMpOnline_DispatchSchedulerProxyThunk(self, nullptr);
        break;
    case 15:
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, static_cast<void*>(self), 0)[0x5C / 4])(self, nullptr);
        break;
    case 17:
        CMenuMpOnline_OpenAuUnPaMenu(self, nullptr);
        break;
    case 18:
    {
        float transitionArgument;
        std::memcpy(&transitionArgument, &updateArgument, sizeof(transitionArgument));
        CMenuMpOnline_UpdateTransition(self, nullptr, transitionArgument);
        break;
    }
    default:
        break;
    }
}

HOOK(0x0055FE90, CMenuMpOnline_UpdateAndDispatch, "auto");
