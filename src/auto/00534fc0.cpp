#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_CurrentScene, 0x008AFC90);
CSF_GLOBAL(uint8_t, g_FriendsMode, 0x008B067C);

CSF_FUNC(void, __fastcall, MenuMpPause_PrepareInitialization, 0x007438A0,
         (CMenuMpPause* self, void* edx));
CSF_FUNC(void*, __fastcall, GetSceneMenuContext, 0x0044FC60,
         (void* self, void* edx));
CSF_FUNC(uint32_t, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440,
         (uint32_t identifier, int variant));
CSF_FUNC(void*, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(uint32_t, __fastcall, MenuMpPause_GetMatchButtonState, 0x00551B70,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpNetworkType_ScrollViewportToTarget, 0x00534F20,
         (CMenuMpPause* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(void*, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, MenuManager_SetGameplayControllerState, 0x0042DF80,
         (void* self, void* edx, int mode));
CSF_FUNC(char, __fastcall, MenuMpPause_ResetMenuController, 0x0042CAB0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuMpPause_RefreshGlobalState, 0x0050EE10,
         (void* self, void* edx));

static void* MenuMpPause_AsPointer(uint32_t value)
{
    return reinterpret_cast<void*>(value);
}

static uint32_t MenuMpPause_FindControl(void* screen, const char* name)
{
    return reinterpret_cast<uint32_t(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, screen, 0)[0xFC / 4])(screen, nullptr, name);
}

static void MenuMpPause_SetControlState(void* control, uint32_t enabled,
                                       int update, int propagate)
{
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t, int, int)>(
        CSF_AT(void**, control, 0)[0x68 / 4])(
            control, nullptr, enabled, update, propagate);
}

static void MenuMpPause_SetControlHidden(void* control, int hidden)
{
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, control, 0)[0xA8 / 4])(control, nullptr, hidden);
}

static int __fastcall CMenuMpPause_InitializeControls(
    CMenuMpPause* self, void* /*edx*/, uint32_t ownerToken,
    uint32_t /*unused*/)
{
    MenuMpPause_PrepareInitialization(self, nullptr);

    void* screen = MenuMpPause_AsPointer(self->field_0038);
    self->field_0040 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0044 = 0;
    self->field_0014 = UINT32_MAX;
    self->field_0045 = 0;
    self->field_0048 = 0.0f;
    self->field_004c = 0.0f;
    self->field_0034 = 0;
    self->field_0010 = ownerToken;

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

    self->field_0018 = MenuMpPause_FindControl(
        MenuMpPause_AsPointer(self->field_0038),
        CSF_STR(0x008412B8, "BotonContinue"));
    self->field_001c = MenuMpPause_FindControl(
        MenuMpPause_AsPointer(self->field_0038),
        CSF_STR(0x00841E18, "BotonCharacter"));
    self->field_0020 = MenuMpPause_FindControl(
        MenuMpPause_AsPointer(self->field_0038),
        CSF_STR(0x00841E0C, "BotonExit"));
    self->field_0024 = MenuMpPause_FindControl(
        MenuMpPause_AsPointer(self->field_0038),
        CSF_STR(0x00841DFC, "BotonSuicide"));

    void* character = GetSceneMenuContext(g_CurrentScene, nullptr);
    const uint32_t canSuicide =
        character && CSF_AT(uint8_t, character, 0x3A4) == 0 ? 1u : 0u;
    MenuMpPause_SetControlState(
        MenuMpPause_AsPointer(self->field_0024), canSuicide, 1, 1);

    // The original push edi here saves EDI across the remaining calls.
    // The lookup itself consumes only its name argument.
    self->field_0028 = MenuMpPause_FindControl(
        MenuMpPause_AsPointer(self->field_0038),
        CSF_STR(0x00841DEC, "BotonOpciones"));

    const uint32_t friendsButton = MenuMpPause_FindControl(
        MenuMpPause_AsPointer(self->field_0038),
        CSF_STR(0x00841DDC, "BotonFriends"));
    self->field_002c = friendsButton;
    if (friendsButton != 0)
    {
        const uint32_t notifications = MenuMpPause_FindControl(
            MenuMpPause_AsPointer(self->field_0038),
            CSF_STR(0x00841DC4, "StaticNotificaciones"));
        self->field_0034 = notifications;
        if (notifications != 0)
            MenuMpPause_SetControlHidden(
                MenuMpPause_AsPointer(notifications), 1);

        const bool friendsMode = g_FriendsMode != 0;
        void* friends = MenuMpPause_AsPointer(self->field_002c);
        void* caption = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, friends, 0)[0x110 / 4])(friends, nullptr);
        void** captionVtable = CSF_AT(void**, caption, 0);
        const uint32_t text = MenuMessageBox_ResolveEntryByKey(
            friendsMode ? 0x494E4738u : 0x494E4739u, 1);
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
            captionVtable[0x40 / 4])(caption, nullptr, text);
    }

    const uint32_t matchButton = MenuMpPause_FindControl(
        MenuMpPause_AsPointer(self->field_0038),
        CSF_STR(0x00841DB4, "BotonPartida"));
    self->field_0030 = matchButton;
    if (matchButton != 0)
    {
        void** matchVtable =
            CSF_AT(void**, MenuMpPause_AsPointer(matchButton), 0);
        void* networkState = GetGlobal8B0688();
        // Preserve the full EAX value: the original pushes EAX, not AL.
        const uint32_t enabled =
            MenuMpPause_GetMatchButtonState(networkState, nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t, int, int)>(
            matchVtable[0x68 / 4])(
                MenuMpPause_AsPointer(self->field_0030), nullptr,
                enabled, 1, 1);
    }

    CMenuMpNetworkType_ScrollViewportToTarget(self, nullptr);

    void* state = GetGlobalState_50D190();
    void* stateListener = CSF_AT(void*, state, 0x10);
    if (stateListener != nullptr)
    {
        void* globalValue = GetGlobalValue_753A30();
        const uint32_t value = CSF_AT(uint32_t, globalValue, 0x200);
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, stateListener, 0)[0x20 / 4])(
                stateListener, nullptr, value);
    }

    void* inputManager = GetGlobal8afc78();
    MenuManager_SetGameplayControllerState(inputManager, nullptr, 0);

    state = GetGlobalState_50D190();
    void* activeController = CSF_AT(void*, state, 0x74);
    if (activeController != nullptr)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, activeController, 0)[0x0C / 4])(
                activeController, nullptr);
    }

    MenuMpPause_SetControlHidden(
        MenuMpPause_AsPointer(self->field_0038), 0);

    state = GetGlobalState_50D190();
    MenuMpPause_ResetMenuController(CSF_AT(void*, state, 0x18), nullptr);
    state = GetGlobalState_50D190();
    MenuMpPause_RefreshGlobalState(state, nullptr);

    return static_cast<int>(self->field_0038);
}

HOOK(0x00534FC0, CMenuMpPause_InitializeControls, "auto");
