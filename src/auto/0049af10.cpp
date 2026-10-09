#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, Scene_SetSelectionEnabled, 0x0044F3A0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, CScene_AddObjectFlags, 0x00452800, (CScene* self, void* , int handleAddress, int flags));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategories, 0x004528D0, (CScene* self, void* , int handleAddress, int categoryMask));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, CharActionFsm_ForwardToStateDispatch, 0x00482840, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SetMenuContextState, 0x0049A0E0, (void* self, void* edx, int a4, char a5));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Returns the character's current action object (vtable slot 0xD8 / 4), or null.
static inline C_008319c0* Character_GetCurrentAction(void* character)
{
    return ((C_008319c0*(__fastcall*)(void*, void*))CSF_AT(void**, character, 0)[0xD8 / 4])(character, nullptr);
}

static void __fastcall Character_SetActionEnabled(void* self, void* /*edx*/, char disabled)
{
    if (CSF_AT(char, self, 0x229) == disabled)
        return;

    void* character = CSF_AT(void*, self, 0x44);

    if (disabled)
    {
        SetMenuContextState(self, nullptr, 1, 0);
        CScene_RemoveObjectFromCategories(reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                                          (int)((uint8_t*)self + 0x1C), 1);
        Scene_SetSelectionEnabled(g_pCurrentScene, nullptr, 0);

        ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x54 / 4])(self, nullptr);

        void* visual = CSF_AT(void*, self, 0x4C);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, visual, 0)[0x2C / 4])(visual, nullptr, 0);

        if (Character_GetCurrentAction(character))
        {
            C_008319c0* action = Character_GetCurrentAction(character);
            if (C_008319c0_GetSubObjectField74(action, nullptr) == 15)
                CharActionFsm_ForwardToStateDispatch(character, nullptr, 0);
        }
    }
    else
    {
        SetMenuContextState(self, nullptr, 0, 0);

        void* visual = CSF_AT(void*, self, 0x4C);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, visual, 0)[0x2C / 4])(visual, nullptr, 1);

        CScene_AddObjectFlags(reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                              (int)((uint8_t*)self + 0x1C), 1);
        Scene_SetSelectionEnabled(g_pCurrentScene, nullptr, 1);

        if (CSF_AT(int, self, 0x398) == 0)
        {
            if (Character_GetCurrentAction(character))
            {
                C_008319c0* action = Character_GetCurrentAction(character);
                if (C_008319c0_GetSubObjectField74(action, nullptr) == 15)
                    CharActionFsm_ForwardToStateDispatch(character, nullptr, 1);
            }
        }
    }

    CSF_AT(char, self, 0x229) = disabled;
}

HOOK(0x0049AF10, Character_SetActionEnabled, "auto");
