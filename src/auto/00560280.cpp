#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static const auto GetSessionManager =
    reinterpret_cast<void* (__cdecl*)()>(0x004FDBB0);
static const auto SetSessionName =
    reinterpret_cast<void (__fastcall*)(void*, void*, char*, char)>(0x00501990);
static const auto NotifySessionTransition =
    reinterpret_cast<void (__cdecl*)(int)>(0x00506AC0);
static const auto GetMenuState =
    reinterpret_cast<void* (__cdecl*)()>(0x0050D190);
static const auto ResetSessionState =
    reinterpret_cast<int (__fastcall*)(void*, void*)>(0x0050E1E0);
static const auto SendMenuEvent =
    reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*, int, int, int, int)>(0x0050F790);
static const auto GetOnlineContext =
    reinterpret_cast<void* (__cdecl*)()>(0x00550CC0);
static const auto GetNetworkState =
    reinterpret_cast<void* (__cdecl*)()>(0x006A6D70);
static const auto CheckSessionTransitionReady =
    reinterpret_cast<std::uint32_t (__fastcall*)(void*, void*)>(0x00734A10);
static const auto GetMultiplayerController =
    reinterpret_cast<void* (__cdecl*)()>(0x0073C020);
static const auto SetControllerSession =
    reinterpret_cast<char (__fastcall*)(void*, void*, int)>(0x0073F120);
static const auto RefreshControllerSession =
    reinterpret_cast<int (__fastcall*)(void*, void*)>(0x0073F340);

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);
CSF_GLOBAL(int, g_MpSessionState, 0x008B0690);

static std::uint32_t __fastcall CMenuMpSessions_ProcessSessionTransition(
    CMenuMpSessions* self, void* /*edx*/)
{
    void* controller = GetMultiplayerController();
    if (CSF_AT(int, controller, 0x98) != 6)
    {
        self->field_0014 = 0;
        self->field_01e9 = 1;
        NotifySessionTransition(1);
        return SendMenuEvent(GetMenuState(), nullptr, 0x434D3237, 1, 1, 0);
    }

    int onlineMatch;
    if (GetOnlineContext())
    {
        GetOnlineContext();
        onlineMatch = g_OnlineMatchFlag;
    }
    else
    {
        onlineMatch = g_MpNetworkType == 3;
    }

    if (onlineMatch)
    {
        void* networkState = GetNetworkState();
        int state = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, networkState, 0)[0x1C / 4])(networkState, nullptr);

        if (state == 3)
        {
            std::uint32_t ready = CheckSessionTransitionReady(GetMenuState(), nullptr);
            if (!ready)
                return ready;

            self->field_0014 = 2;
            networkState = GetNetworkState();
            reinterpret_cast<void (__fastcall*)(void*, void*)>(
                CSF_AT(void**, networkState, 0)[0xE4 / 4])(networkState, nullptr);
            NotifySessionTransition(0);
            return SendMenuEvent(GetMenuState(), nullptr, 0x434D3138, 1, 0, 0);
        }

        ResetSessionState(GetMenuState(), nullptr);
        NotifySessionTransition(1);
        networkState = GetNetworkState();
        int sessionId = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, networkState, 0)[0x20 / 4])(networkState, nullptr);

        if (sessionId == -1)
        {
            SetSessionName(GetSessionManager(), nullptr,
                           reinterpret_cast<char*>(&self->field_00b7), 1);
            g_MpSessionState = 0;
            std::uint32_t result =
                RefreshControllerSession(GetMultiplayerController(), nullptr);
            self->field_0014 = 0;
            return result;
        }

        void* menuState = GetMenuState();
        int controllerSession = CSF_AT(int, menuState, 0x10);
        controller = GetMultiplayerController();
        SetControllerSession(controller, nullptr, controllerSession);
        NotifySessionTransition(1);

        networkState = GetNetworkState();
        sessionId = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, networkState, 0)[0x20 / 4])(networkState, nullptr);
        std::uint32_t result =
            SendMenuEvent(GetMenuState(), nullptr, sessionId, 32, 1, 0);
        self->field_0014 = 0;
        self->field_01e9 = 1;
        return result;
    }

    ResetSessionState(GetMenuState(), nullptr);
    NotifySessionTransition(1);
    SetSessionName(GetSessionManager(), nullptr,
                   reinterpret_cast<char*>(&self->field_00b7), 1);
    std::uint32_t result =
        RefreshControllerSession(GetMultiplayerController(), nullptr);
    self->field_0014 = 0;
    return result;
}

HOOK(0x00560280, CMenuMpSessions_ProcessSessionTransition, "auto");
