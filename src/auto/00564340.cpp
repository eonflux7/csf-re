#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(void, __fastcall, GlobalMenu_SetSelectedScenarioName, 0x00501990, (S_004ff0f0* self, void* , char* String1, char a3));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ResetPending, 0x0050E1E0, (void* self, void*));
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, AudioSoundManager_ForwardToCurrentIfQuerySucceeds, 0x00521E80, (void* self, void* edx));
CSF_FUNC(void, __fastcall, S_00522010_AccumulateAndPropagateTime, 0x005231D0, (S_00522010* self, void* , int deltaBits));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetMpGameModeIndex, 0x005515F0, ());
CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_InitializeCharacterSelection, 0x00551AD0, (int sourceObject));
CSF_FUNC(std::uint32_t, __fastcall, CMenuMpSessions_ProcessSessionTransition, 0x00560280, (CMenuMpSessions* self, void*));
CSF_FUNC(void, __fastcall, CMenuMpSessions_ClearSessionSlotUnlessState3, 0x00560430, (CMenuMpSessions* self, void*));
CSF_FUNC(uint32_t*, __fastcall, CMenuMpSessions_UpdateControlPosition, 0x00561230, (CMenuMpSessions* self, void*));
CSF_FUNC(bool, __fastcall, CMenuMpSessions_RefreshOptionState, 0x00562770, (CMenuMpSessions* self, void* edx, char a3, char a4));
CSF_FUNC(unsigned int, __fastcall, CMenuMpSessions_RetrySessionRequest, 0x00562D50, (uint32_t* a1, int a2));
CSF_FUNC(unsigned int, __fastcall, CMenuMpSessions_StartSelectedSession, 0x00562ED0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CMenuMpSessions_UpdateSessionBrowser, 0x00563520, (void* self, int a2));
CSF_FUNC(void*, __fastcall, CMenuMpSessions_BuildSessionEntry, 0x00563AB0, (CMenuMpSessions* self, void* edx));
CSF_FUNC(void, __fastcall, CMenuMpSessions_UpdateSessionOptions, 0x00563EF0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660, (CGuiSlider* self, void*));
CSF_FUNC(void*, __fastcall, C_00861120_GetSchedulerProxy, 0x00734A10, (C_00861120* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, NetSession_ProcessLinkedObjectIfReady, 0x0073CAE0, (C_0085e760** self, void*));
CSF_FUNC(char*, __cdecl, NetSession_GetSpawnPointNameSuffix, 0x0073DF80, ());
CSF_FUNC(void*, __fastcall, NetSession_LookupStatsSlotHandle, 0x0073E770, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_ApplyConnectionSettings, 0x0073EDE0, (void* self, void* , char alternate));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ResetCharacterMenu, 0x0073F340, (void* self, void*));

CSF_GLOBAL(float, g_RotationWrapStep, 0x0082E1E4);
CSF_GLOBAL(const char*, g_pszMenu_P_Juego, 0x00899AF0);
CSF_GLOBAL(const char*, g_pszMenu_P_LbFrie, 0x00899B58);
CSF_GLOBAL(uint32_t, g_dwAccountEditScreenState, 0x0089A5EC);
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);
CSF_GLOBAL(uint32_t*, g_MenuMpOnlineAccountEditContext, 0x0089A5FC);
CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);
CSF_GLOBAL(int, g_MpSessionState, 0x008B0690);
CSF_GLOBAL(uint8_t, g_MpSessionsTransitionBlocked, 0x008B0903);

static bool MenuMpSessions_IsOnlineMatch()
{
    if (GetGlobal8B0688())
    {
        GetGlobal8B0688();
        return g_OnlineMatchFlag != 0;
    }
    return g_MpNetworkType == 3;
}

static int MenuMpSessions_GetNetworkState()
{
    void* network = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, network, 0)[0x1C / 4])(network, nullptr);
}

static int MenuMpSessions_GetNetworkMenuEntry()
{
    void* network = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, network, 0)[0x20 / 4])(network, nullptr);
}

static void MenuMpSessions_AcknowledgeNetworkRequest()
{
    void* network = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, network, 0)[0xE4 / 4])(network, nullptr);
}

static void MenuMpSessions_FinishNetworkRequest()
{
    void* network = reinterpret_cast<void*>(GetGlobalState_6A6D70());
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, network, 0)[0xE8 / 4])(network, nullptr);
}

static void MenuMpSessions_ResetPending()
{
    GameBddLoader_ResetPending(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
}

static int MenuMpSessions_Dispatch(int entry, int argument, int request, int configuration)
{
    return GameBddLoader_DispatchMenuRequest(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr,
        entry, argument, request, configuration);
}

static void* MenuMpSessions_GetSchedulerProxy()
{
    return C_00861120_GetSchedulerProxy(
        reinterpret_cast<C_00861120*>(GetGlobalState_50D190()), nullptr);
}

static void CMenuMpSessions_ForwardActiveControl(CMenuMpSessions* self)
{
    void* control = reinterpret_cast<void*>(self->field_0070);
    if (control &&
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x274 / 4])(control, nullptr) > 0)
    {
        CGuiSlider_ForwardSelfToParent(
            reinterpret_cast<CGuiSlider*>(self->field_0070), nullptr);
    }
}

static void CMenuMpSessions_ResetToIdle(CMenuMpSessions* self)
{
    MenuMpSessions_ResetPending();
    SceneLoader_NoOp();
    self->field_0014 = 0;
    self->field_01e9 = 1;
}

static int __fastcall CMenuMpSessions_UpdateSessionState(
    CMenuMpSessions* self, void* /*edx*/, int deltaBits)
{
    CMenuMpSessions_UpdateControlPosition(self, nullptr);

    if (CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_8B0568()), 0x5C) == 1)
    {
        AudioSoundManager_ForwardToCurrentIfQuerySucceeds(
            reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

        void* control = reinterpret_cast<void*>(self->field_0044);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, control, 0)[0x64 / 4])(control, nullptr, 0, 0);

        control = reinterpret_cast<void*>(self->field_0044);
        reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, control, 0)[0x58 / 4])(
                control, nullptr, g_pszMenu_P_Juego);

        control = reinterpret_cast<void*>(self->field_0044);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, control, 0)[0x9C / 4])(control, nullptr, 0);

        control = reinterpret_cast<void*>(self->field_0044);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr);
    }

    S_00522010_AccumulateAndPropagateTime(
        reinterpret_cast<S_00522010*>(GetGlobalValue_521BB0()), nullptr, deltaBits);
    SceneLoader_NoOp();

    switch (self->field_0014)
    {
    case 0:
        if (self->field_01e0 == 0)
            CMenuMpSessions_UpdateSessionBrowser(self, 0);
        else if (self->field_01e0 == 2)
            CMenuMpSessions_UpdateSessionOptions(self, nullptr);
        break;

    case 1:
        CMenuMpSessions_ProcessSessionTransition(self, nullptr);
        break;

    case 2:
        CMenuMpSessions_ClearSessionSlotUnlessState3(self, nullptr);
        break;

    case 3:
        if (!g_MpSessionsTransitionBlocked)
        {
            if (MenuMpSessions_GetSchedulerProxy())
            {
                if (MenuMpSessions_IsOnlineMatch())
                    MenuMpSessions_AcknowledgeNetworkRequest();
                else
                    NetSession_ProcessLinkedObjectIfReady(
                        reinterpret_cast<C_0085e760**>(GetGlobalDword_73C020()), nullptr);

                self->field_0014 = 0;
                self->field_01e9 = 1;
                SceneLoader_NoOp();
            }
            else if (MenuMpSessions_IsOnlineMatch())
            {
                if (MenuMpSessions_GetNetworkState() != 3)
                {
                    if (CMenuMpSessions_RefreshOptionState(self, nullptr, 0, 1))
                    {
                        if (MenuMpSessions_GetNetworkMenuEntry() == -1)
                        {
                            MenuMpSessions_FinishNetworkRequest();
                            self->field_0014 = 6;
                        }
                        else
                        {
                            MenuMpSessions_ResetPending();
                            SceneLoader_NoOp();
                            int entry = MenuMpSessions_GetNetworkMenuEntry();
                            MenuMpSessions_Dispatch(entry, 32, 1, 0);
                            self->field_0014 = 0;
                            self->field_01e9 = 1;
                        }
                    }
                    else if (self->field_01d8 == 1)
                    {
                        self->field_0014 = 13;
                    }
                    else
                    {
                        self->field_01d8 = 0;
                        CMenuMpSessions_RetrySessionRequest(
                            reinterpret_cast<uint32_t*>(self), 0);
                    }
                }
            }
            else
            {
                MenuMpSessions_ResetPending();
                SceneLoader_NoOp();
                CMenuMpSessions_RefreshOptionState(self, nullptr, 1, 1);
                CMenuMpSessions_ForwardActiveControl(self);
                self->field_0014 = 0;
                self->field_01e8 = 1;
            }
        }
        break;

    case 4:
        if (MenuMpSessions_IsOnlineMatch())
        {
            if (MenuMpSessions_GetNetworkState() == 3)
            {
                if (MenuMpSessions_GetSchedulerProxy())
                {
                    MenuMpSessions_AcknowledgeNetworkRequest();
                    SceneLoader_NoOp();
                    MenuMpSessions_Dispatch(1330459185, 1, 0, 0);
                    self->field_0014 = 5;
                }
            }
            else
            {
                bool hasMenuEntry = false;
                if (MenuMpSessions_GetNetworkMenuEntry() == -1)
                {
                    void* session = reinterpret_cast<void*>(GetGlobalDword_73C020());
                    void* stats = NetSession_LookupStatsSlotHandle(session, nullptr);
                    MpSpawnPoints_InitializeCharacterSelection(reinterpret_cast<int>(stats));

                    GetGlobalDword_73C020();
                    char* suffix = NetSession_GetSpawnPointNameSuffix();
                    GlobalMenu_SetSelectedScenarioName(
                        reinterpret_cast<S_004ff0f0*>(GetGlobalValue_8B0568()),
                        nullptr, suffix, 1);
                    MpSpawnPoints_ResetCharacterMenu(
                        reinterpret_cast<void*>(GetGlobalDword_73C020()), nullptr);

                    self->field_01c8 = 0;
                    int mode = GetMpGameModeIndex();
                    CSF_AT(int, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xCC) = mode;

                    char alternate = g_OnlineMatchFlag == 0;
                    NetSession_ApplyConnectionSettings(
                        reinterpret_cast<void*>(GetGlobalDword_73C020()), nullptr, alternate);
                }
                else
                {
                    if (self->field_01dc)
                    {
                        self->field_01dc = 0;
                        void* network = reinterpret_cast<void*>(GetGlobalState_6A6D70());
                        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                            CSF_AT(void**, network, 0)[0x60 / 4])(
                                network, nullptr, self->field_01c4);
                        g_MpSessionState = 0;
                        break;
                    }
                    hasMenuEntry = true;
                }

                MenuMpSessions_ResetPending();
                SceneLoader_NoOp();
                self->field_01c4 = 0;
                self->field_0014 = 0;

                if (hasMenuEntry)
                {
                    SceneLoader_NoOp();
                    if (self->field_01e0)
                    {
                        int entry = MenuMpSessions_GetNetworkMenuEntry();
                        MenuMpSessions_Dispatch(entry, 32, 1, 0);
                        if (self->field_01e0 == 4)
                            self->field_0014 = 10;
                        self->field_01e9 = 1;
                    }
                    else
                    {
                        int entry = MenuMpSessions_GetNetworkMenuEntry();
                        MenuMpSessions_Dispatch(entry, 32, 1, 0);
                        self->field_01e9 = 1;
                    }
                }
            }
        }
        else
        {
            int state = CSF_AT(int, reinterpret_cast<void*>(GetGlobalDword_73C020()), 0x98);
            if (state == 6)
            {
                void* session = reinterpret_cast<void*>(GetGlobalDword_73C020());
                void* stats = NetSession_LookupStatsSlotHandle(session, nullptr);
                MpSpawnPoints_InitializeCharacterSelection(reinterpret_cast<int>(stats));

                int mode = GetMpGameModeIndex();
                CSF_AT(int, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xCC) = mode;

                GetGlobalDword_73C020();
                char* suffix = NetSession_GetSpawnPointNameSuffix();
                GlobalMenu_SetSelectedScenarioName(
                    reinterpret_cast<S_004ff0f0*>(GetGlobalValue_8B0568()),
                    nullptr, suffix, 1);
                MpSpawnPoints_ResetCharacterMenu(
                    reinterpret_cast<void*>(GetGlobalDword_73C020()), nullptr);

                self->field_0014 = 0;
                char alternate = g_OnlineMatchFlag == 0;
                NetSession_ApplyConnectionSettings(
                    reinterpret_cast<void*>(GetGlobalDword_73C020()), nullptr, alternate);
            }
            else if (state != 5)
            {
                MenuMpSessions_ResetPending();
                SceneLoader_NoOp();
                MenuMpSessions_Dispatch(1229730354, 32, 1, 0);
                self->field_0014 = 11;
            }
        }
        break;

    case 5:
        if (MenuMpSessions_GetNetworkState() != 3)
            CMenuMpSessions_ResetToIdle(self);
        break;

    case 6:
        if (MenuMpSessions_GetNetworkState() != 3)
        {
            CMenuMpSessions_RefreshOptionState(self, nullptr, 1, 1);
            MenuMpSessions_ResetPending();
            SceneLoader_NoOp();

            void* control = reinterpret_cast<void*>(self->field_0070);
            self->field_0014 = 0;
            self->field_01e8 = 1;
            if (control &&
                reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, control, 0)[0x274 / 4])(control, nullptr) > 0)
            {
                CGuiSlider_ForwardSelfToParent(
                    reinterpret_cast<CGuiSlider*>(self->field_0070), nullptr);
            }
        }
        break;

    case 7:
        {
            void* network = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, network, 0)[0xBC / 4])(
                    network, nullptr, g_MenuMpOnlineAccountEditContext, 0);

            if (g_MenuMpOnlineAccountEditContext)
            {
                uint32_t* context = g_MenuMpOnlineAccountEditContext;
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, context, 0)[0])(context, nullptr, 1);
                g_MenuMpOnlineAccountEditContext = nullptr;
            }
            g_dwAccountEditScreenState = 3;
            self->field_01f8 = 0;
            self->field_0014 = 8;
        }
        break;

    case 8:
        if (MenuMpSessions_GetNetworkState() != 3)
        {
            bool ready = self->field_01f8 != 0;
            if (!ready)
            {
                void* network = reinterpret_cast<void*>(GetGlobalState_6A6D70());
                ready = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, network, 0)[0xD4 / 4])(network, nullptr);
            }
            if (ready)
            {
                CMenuMpSessions_RefreshOptionState(self, nullptr, 0, 0);
                if (self->field_0070)
                {
                    alignas(8) uint8_t menuBuffer[0xB4];
                    MenuBuffer_Init(menuBuffer, nullptr);
                    menuBuffer[24] = 1;
                    SceneLoader_NoOp();
                    MenuMpSessions_Dispatch(
                        1330458933, 1, 0, reinterpret_cast<int>(menuBuffer));
                    MenuMpSessions_FinishNetworkRequest();
                    self->field_0014 = 9;
                    SceneLoader_DestroyMenuBuffer(
                        reinterpret_cast<std::uint32_t*>(menuBuffer), nullptr);
                }
            }
            else
            {
                void* network = reinterpret_cast<void*>(GetGlobalState_6A6D70());
                reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*, int)>(
                    CSF_AT(void**, network, 0)[0xBC / 4])(network, nullptr, nullptr, 1);
                self->field_01f8 = 1;
                self->field_0014 = 8;
            }
        }
        break;

    case 9:
        if (MenuMpSessions_GetNetworkState() != 3)
        {
            CMenuMpSessions_RefreshOptionState(self, nullptr, 1, 0);
            MenuMpSessions_ResetPending();
            self->field_01e8 = 1;
            SceneLoader_NoOp();
            if (self->field_0070)
            {
                self->field_01cc = 0;
                CMenuMpSessions_StartSelectedSession(self, nullptr);
                if (self->field_0014 != 4)
                {
                    MenuMpSessions_Dispatch(1480734514, 128, 1, 0);
                    self->field_0014 = 10;
                }
            }
        }
        break;

    case 10:
        if (MenuMpSessions_GetSchedulerProxy())
        {
            self->vNavigateBack();
            void* control = reinterpret_cast<void*>(self->field_0044);
            reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
                CSF_AT(void**, control, 0)[0x58 / 4])(
                    control, nullptr, g_pszMenu_P_LbFrie);
        }
        break;

    case 11:
        if (MenuMpSessions_GetSchedulerProxy())
            self->field_0014 = 0;
        break;

    case 12:
        {
            void* result = MenuMpSessions_GetSchedulerProxy();
            if (result == reinterpret_cast<void*>(4))
                CMenuMpSessions_BuildSessionEntry(self, nullptr);
            else if (result == reinterpret_cast<void*>(8))
                self->field_0014 = 0;
        }
        break;

    case 13:
        {
            float delta;
            std::memcpy(&delta, &deltaBits, sizeof(delta));
            self->field_020c = delta + self->field_020c;
            if (self->field_020c > g_RotationWrapStep)
            {
                self->field_020c = 0.0f;
                CMenuMpSessions_ResetToIdle(self);
            }
        }
        break;

    default:
        break;
    }

    int result = self->field_01f1;
    if (result)
    {
        self->vNavigateBack();
        SceneLoader_NoOp();
        return MenuMpSessions_Dispatch(1330459189, 1, 1, 0);
    }
    return result;
}

HOOK(0x00564340, CMenuMpSessions_UpdateSessionState, "auto");
