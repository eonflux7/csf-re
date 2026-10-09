#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, Scene_SetActionMode, 0x00452320,
         (void* self, void* edx, char a2));
CSF_FUNC(void, __fastcall, C_008317c8_SetActionTarget, 0x00479120,
         (C_008317c8* self, void* edx, C_00862490* a2, uint32_t a3));
CSF_FUNC(int, __fastcall, CharActionFsm_GetCurrentState, 0x004834F0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_SetActionBlend, 0x00483540,
         (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, Character_SetFlagEnabled, 0x00498F50,
         (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GlobalValue_SetSelection, 0x004FC3B0,
         (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall CCharActionFsm_ClearCurrentAction(
    CCharActionFsm* self, void* /*edx*/)
{
    void* action = reinterpret_cast<void*>(self->field_009c);
    if (!action)
        return;

    const int actionType =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, action, 0)[0x4C / 4])(action, nullptr);

    switch (actionType)
    {
    case 10:
    {
        void* target = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, target, 0)[0x90 / 4])(target, nullptr);
        break;
    }

    case 11:
    {
        auto* target = reinterpret_cast<C_008317c8*>(self->vGetIndexedEntryOrZero());
        C_008317c8_SetActionTarget(target, nullptr, nullptr, 0);
        break;
    }

    case 12:
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x1F0)
            &= ~2u;
        break;

    case 13:
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x2E4)
            &= ~0x40000u;
        Character_SetFlagEnabled(
            reinterpret_cast<void*>(self->field_0008), nullptr, 0x40000, 0);

        if (CharActionFsm_GetCurrentState(self, nullptr) == 13)
            Scene_SetActionMode(g_pCurrentScene, nullptr, 0);

        {
            void* globalValue =
                reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
            GlobalValue_SetSelection(globalValue, nullptr, -1);
        }
        CharActionFsm_SetActionBlend(self, nullptr, 0.0f);
        break;

    default:
        break;
    }

    action = reinterpret_cast<void*>(self->field_009c);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, action, 0)[0x3C / 4])(action, nullptr);
    self->field_009c = 0;
}

HOOK(0x004846D0, CCharActionFsm_ClearCurrentAction, "auto");
