#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateSelectedWeaponSlot, 0x004833C0, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_UpdateWeaponActionState, 0x004849A0, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, Character_SetActionMode, 0x00492760, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, Character_RefreshStanceState, 0x004963E0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CharActionFsm_SetCharacterToggleState, 0x004975E0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, Character_DisableSpecialMode, 0x0049A970, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CharacterSubState_SetEnabled, 0x004A1260, (void* self, void* edx, char a2));

static char __fastcall C_00834050_ResetActionStateAndRefocus(C_00834050* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x2E0) |= 0x10000u;
    Character_DisableSpecialMode(self, nullptr);

    CCharActionFsm* actionFsm = reinterpret_cast<CCharActionFsm*>(self->field_0044);
    CCharActionFsm_UpdateWeaponActionState(actionFsm, nullptr);
    CCharActionFsm_UpdateSelectedWeaponSlot(actionFsm, nullptr);

    Character_SetActionMode(self, nullptr, 0);

    if ((CSF_AT(uint8_t, self, 0x2E0) & 4) == 0)
    {
        CSF_AT(uint32_t, self, 0x2E4) |= 4u;
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0xB4 / 4])(self, nullptr, 1);
        Character_RefreshStanceState(self, nullptr);

        if ((CSF_AT(uint8_t, self, 0x2E0) & 4) != 0)
        {
            void* model = reinterpret_cast<void*>(self->field_002c);
            float heightDelta = CSF_AT(float, model, 0xE4) - CSF_AT(float, model, 0xE8);

            void* position = reinterpret_cast<void*>(GetObjectPositionVector(self, nullptr));

            float posZ = CSF_AT(float, position, 8);
            int posXBits = CSF_AT(int, position, 0);
            float focusY = heightDelta + CSF_AT(float, position, 4);

            int focusPos[3];
            focusPos[0] = posXBits;
            CSF_AT(float, focusPos, 4) = focusY;
            CSF_AT(float, focusPos, 8) = posZ;

            SceneLoader_ConfigureFocusEntry(self, nullptr, focusPos);
        }
    }

    if (CSF_AT(uint8_t, self, 0x3D0) == 0 && CSF_AT(uint8_t, self, 0x228) == 0)
    {
        self->vSlot6C(0);
    }

    CharActionFsm_SetCharacterToggleState(self, nullptr, 0);
    return CharacterSubState_SetEnabled(reinterpret_cast<uint8_t*>(self) + 0x344, nullptr, 0);
}

HOOK(0x0049AE30, C_00834050_ResetActionStateAndRefocus, "auto");
