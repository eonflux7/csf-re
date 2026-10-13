#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00830578_ResetMenuController, 0x0042CAB0, (C_00830578* self, void*));
CSF_FUNC(int, __cdecl, GetValue_8B0574, 0x00518980, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __cdecl, IsGlobalValueNonZero, 0x00521BF0, ());
CSF_FUNC(void, __fastcall, SceneLoader_UpdateLoadingState, 0x00521F40, (void* self, void* edx, int state, int flags));
CSF_FUNC(bool, __fastcall, AudioSoundManager_QueryActiveSound, 0x00521F60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void, __fastcall, Object_ForwardArgsToSubobject35, 0x006B1640, (void* self, void* edx, bool enabled));

static void __fastcall CMenuManager_SetMenuState(
    CMenuManager* self, void* /*edx*/, char state)
{
    const uint8_t requestedState = static_cast<uint8_t>(state);
    if (self->field_0070 == requestedState)
        return;

    self->field_0070 = requestedState;
    const bool globalAvailable =
        static_cast<uint8_t>(IsGlobalValueNonZero()) != 0;

    if (state)
    {
        if (globalAvailable && !self->field_0108)
        {
            void* loader = reinterpret_cast<void*>(GetGlobalValue_521BB0());
            SceneLoader_UpdateLoadingState(loader, nullptr, 0, 0);
        }
    }
    else
    {
        if (globalAvailable)
        {
            void* soundManager =
                reinterpret_cast<void*>(GetGlobalValue_521BB0());
            const bool activeSound =
                AudioSoundManager_QueryActiveSound(soundManager, nullptr);
            self->field_0108 = activeSound;
            if (!activeSound)
            {
                void* loader =
                    reinterpret_cast<void*>(GetGlobalValue_521BB0());
                SceneLoader_UpdateLoadingState(loader, nullptr, 1, 0);
            }
        }

        GetValue_8B0574();
        auto* gameScreen =
            reinterpret_cast<C_00830578*>(self->m_pScreenGame);
        if (gameScreen && !CSF_AT(uint8_t, gameScreen, 0x180))
            C_00830578_ResetMenuController(gameScreen, nullptr);
    }

    void* menuObject = reinterpret_cast<void*>(self->field_0090);
    if (menuObject && !self->field_00d0)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, menuObject, 0)[8 / 4])(menuObject, nullptr);
    }

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    Object_ForwardArgsToSubobject35(globalObject, nullptr, state == 0);
}

HOOK(0x0050D700, CMenuManager_SetMenuState, "auto");
