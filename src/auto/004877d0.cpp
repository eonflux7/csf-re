#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(void*, __fastcall, SceneLoader_SetActionFsmMode, 0x00484420, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, CharActionFsm_FindActionIndex, 0x00485A20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharActionFsm_SetCharacterItem, 0x00494E90, (void* self, void* edx, int a4));
CSF_FUNC(int, __fastcall, CharActionFsm_SetCharacterFlag, 0x00498F50, (void* self, void* edx, int a2, char a3));
CSF_FUNC(void, __fastcall, CharActionFsm_EnableItemAction, 0x0049AF10, (void* self, void* edx, char a3));
CSF_FUNC(int, __fastcall, CharActionFsm_SetProviderValue, 0x004F71B0, (void* self, void* edx, float a4));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(void, __fastcall, CharActionFsm_NotifyCharacterAction, 0x00525B30, (void* self, void* edx, int a3, int a4));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __fastcall CCharActionFsm_ActivateItemAction(
    CCharActionFsm* self, void* /*edx*/)
{
    void* selectedAction = nullptr;
    const int actionIndex = CharActionFsm_FindActionIndex(self, nullptr, 28);
    if (actionIndex != -1)
    {
        void* action = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_0078), 4 * actionIndex);
        if (CSF_AT(int, action, 0x80))
            selectedAction = action;
    }

    const int item = CSF_AT(int, selectedAction, 0x80);
    CharActionFsm_SetCharacterItem(
        reinterpret_cast<void*>(self->field_0008), nullptr, item);
    CharActionFsm_EnableItemAction(
        reinterpret_cast<void*>(self->field_0008), nullptr, 1);
    CharActionFsm_SetCharacterFlag(
        reinterpret_cast<void*>(self->field_0008), nullptr, 4096, 1);

    const int globalValue = GetGlobalValue_4FBDE0();
    const int firstElement =
        GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* provider =
        GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
    CharActionFsm_SetProviderValue(provider, nullptr, 0.0f);

    C_008319c0* currentAction =
        reinterpret_cast<C_008319c0*>(self->field_009c);
    if (!currentAction ||
        C_008319c0_GetSubObjectField74(currentAction, nullptr) != 28 ||
        (CSF_AT(std::uint8_t,
                reinterpret_cast<void*>(self->field_0008), 0x1F0) & 2) != 0)
    {
        SceneLoader_SetActionFsmMode(self, nullptr, 1);
    }

    const int character = static_cast<int>(self->field_0008);
    const int actionManager = Get_008b0678();
    CharActionFsm_NotifyCharacterAction(
        reinterpret_cast<void*>(actionManager), nullptr, character, 108);

    return self->vUpdateWeaponSelection(51);
}

HOOK(0x004877D0, CCharActionFsm_ActivateItemAction, "auto");
