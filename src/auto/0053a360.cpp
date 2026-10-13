#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __cdecl, IsGlobal8AFC78NonZero, 0x0042D6E0, ());
CSF_FUNC(int, __stdcall, MenuManager_SetGameplayControllerState, 0x0042DF80, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __fastcall, MenuManager_SetMenuState, 0x0050DC80, (void* self, void* edx, char flag));
CSF_FUNC(int, __fastcall, MenuMpPause_RefreshGlobalState, 0x0050EE10, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuPause_CenterOnTarget, 0x0052F950, (CMenuPause* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

static uint32_t __fastcall CMenuPause_Initialize(
    CMenuPause* self, void* /*edx*/, uint32_t context, int /*unused*/)
{
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) &&
        !CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0x89))
    {
        MenuManager_SetMenuState(
            reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr, 1);
    }

    self->vResetState();
    self->field_0010 = context;

    MenuProgressInput_UpdateGlobalState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    void* screen = reinterpret_cast<void*>(self->field_0018);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, screen, 0)[0x16C / 4])(
        screen, nullptr);

    screen = reinterpret_cast<void*>(self->field_0018);
    self->field_0020 =
        ((uint32_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x008412B8, "BotonContinue"));

    screen = reinterpret_cast<void*>(self->field_0018);
    self->field_0024 =
        ((uint32_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x008424F0, "BotonRestart"));

    screen = reinterpret_cast<void*>(self->field_0018);
    uint32_t button =
        ((uint32_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x00841718, "BotonLoad"));
    self->field_0028 = button;
    if (button)
    {
        int flag = CSF_AT(uint8_t,
            reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xCC);
        void* loadButton = reinterpret_cast<void*>(self->field_0028);
        ((void(__fastcall*)(void*, void*, int, int, int))
            CSF_AT(void**, loadButton, 0)[0x68 / 4])(
            loadButton, nullptr, flag, 1, 1);
    }

    screen = reinterpret_cast<void*>(self->field_0018);
    button =
        ((uint32_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x008424E4, "BotonSave"));
    self->field_002c = button;
    if (button)
    {
        int flag = CSF_AT(uint8_t,
            reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xCC);
        void* saveButton = reinterpret_cast<void*>(self->field_002c);
        ((void(__fastcall*)(void*, void*, int, int, int))
            CSF_AT(void**, saveButton, 0)[0x68 / 4])(
            saveButton, nullptr, flag, 1, 1);
    }

    screen = reinterpret_cast<void*>(self->field_0018);
    self->field_0030 =
        ((uint32_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x00841DEC, "BotonOpciones"));

    screen = reinterpret_cast<void*>(self->field_0018);
    self->field_0034 =
        ((uint32_t(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x00841E0C, "BotonExit"));

    CMenuPause_CenterOnTarget(self, nullptr);

    void* target = CSF_AT(void*,
        reinterpret_cast<void*>(GetGlobalState_50D190()), 0x10);
    if (target)
    {
        uint32_t value = CSF_AT(uint32_t,
            reinterpret_cast<void*>(GetGlobalValue_753A30()), 0x200);
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, target, 0)[0x20 / 4])(target, nullptr, value);
    }

    if (static_cast<uint8_t>(IsGlobal8AFC78NonZero()))
    {
        GetGlobal8afc78();
        MenuManager_SetGameplayControllerState(0);
    }

    void* controller = CSF_AT(void*,
        reinterpret_cast<void*>(GetGlobalState_50D190()), 0x74);
    if (controller)
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0xC / 4])(controller, nullptr);
    }

    screen = reinterpret_cast<void*>(self->field_0018);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, screen, 0)[0xA8 / 4])(screen, nullptr, 0);

    ((void(__fastcall*)(CMenuPause*, void*))
        CSF_AT(void**, self, 0)[0x40 / 4])(self, nullptr);

    MenuMpPause_RefreshGlobalState(
        reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);

    return self->field_0018;
}

HOOK(0x0053A360, CMenuPause_Initialize, "auto");
