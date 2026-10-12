#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpSessions_ScrollListToHeldTarget, 0x00560810, (CMenuMpSessions* self, void*));
CSF_FUNC(void, __fastcall, CMenuMpSessions_RefreshOptionControls, 0x00560960, (CMenuMpSessions* self, void*));
CSF_FUNC(int, __fastcall, MenuMpSessions_HandleState1Or3, 0x00560BD0, (void* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpSessions_SetControlsActive, 0x00560F80, (CMenuMpSessions* self, void*));
CSF_FUNC(int, __fastcall, CMenuMpSessions_EnterScreen, 0x00561650, (CMenuMpSessions* self, void* , int unusedA2, int unusedA3));
CSF_FUNC(void*, __fastcall, CMenuMpSessions_UpdateStateThree, 0x00561C30, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, CMenuMpSessions_SetupStateZero, 0x00563140, (void* self, void* edx, int a3, int a4));
CSF_FUNC(bool, __fastcall, CMenuMpSessions_SetupStateTwo, 0x00563280, (void* self, void* edx, int a3, int a4));
CSF_FUNC(int, __fastcall, CMenuMpSessions_ShowFriendlyFireOptionState, 0x00563350, (CMenuMpSessions* self, void* , int , int));

static uint32_t __fastcall CMenuMpSessions_InitializeScreenForState(
    CMenuMpSessions* self, void* /*edx*/, int a2, int a3)
{
    void* progressInput = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    MenuProgressInput_UpdateGlobalState(progressInput, nullptr);

    if (!self->field_0040)
    {
        void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
        ((void(__fastcall*)(void*, void*, CMenuMpSessions*))
            CSF_AT(void**, globalState, 0)[0x18 / 4])(
                globalState, nullptr, self);
    }

    self->vInitializeState();

    void* screen = reinterpret_cast<void*>(self->field_0040);
    self->field_0044 = a2;
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

    const uint32_t initialState = self->field_01e0;
    self->field_01f0 = 0;
    switch (initialState)
    {
    case 0:
        CMenuMpSessions_SetupStateZero(self, nullptr, a2, a3);
        break;
    case 1:
    case 3:
        CMenuMpSessions_EnterScreen(self, nullptr, a2, a3);
        break;
    case 2:
        CMenuMpSessions_SetupStateTwo(self, nullptr, a2, a3);
        break;
    case 4:
        CMenuMpSessions_ShowFriendlyFireOptionState(self, nullptr, a2, a3);
        break;
    default:
        break;
    }

    const uint32_t backButton = self->field_0040;
    self->field_01f1 = 0;
    self->vSetBackButton(static_cast<int>(backButton));
    CMenuMpSessions_ScrollListToHeldTarget(self, nullptr);

    switch (self->field_01e0)
    {
    case 0:
        CMenuMpSessions_RefreshOptionControls(self, nullptr);
        break;
    case 1:
    case 3:
        MenuMpSessions_HandleState1Or3(self, nullptr);
        break;
    case 2:
    case 4:
        CMenuMpSessions_SetControlsActive(self, nullptr);
        break;
    default:
        break;
    }

    if (self->field_01e0 == 3)
        CMenuMpSessions_UpdateStateThree(self, nullptr);

    return self->field_0040;
}

HOOK(0x005633F0, CMenuMpSessions_InitializeScreenForState, "auto");
