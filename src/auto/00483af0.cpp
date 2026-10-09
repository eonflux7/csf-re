#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, CharActionFsm_GetWeaponActionStatus, 0x00478280, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_SetWeaponActionMode, 0x004782E0, (void* self, void* edx, int a4));
CSF_FUNC(int, __fastcall, CharActionFsm_GetCurrentItemIndex, 0x00479310, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_AdvanceWeaponAction, 0x00480710, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharActionFsm_SetCharacterActionFlag, 0x00498F50, (void* self, void* edx, int a2, char a3));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

static int __fastcall CCharActionFsm_UpdateWeaponAction(
    CCharActionFsm* self, void* /*edx*/)
{
    const bool actionActive = self->vHasUsableWeapon() != 0;
    void* action = reinterpret_cast<void*>(self->field_009c);

    if (!actionActive)
    {
        if (action &&
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, action, 0)[0x4C / 4])(action, nullptr) == 11)
        {
            self->vBeginWeaponStateChange(12, 0);
        }
        else
        {
            self->vBeginWeaponStateChange(0, 0);
        }
        goto finish;
    }

    if (action &&
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, reinterpret_cast<void*>(self->field_009c), 0)[0x4C / 4])(
                reinterpret_cast<void*>(self->field_009c), nullptr) == 11)
    {
        void* weapon = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
        const int itemIndex = CharActionFsm_GetCurrentItemIndex(weapon, nullptr);
        void* item = reinterpret_cast<void*>(self->vLookupEntryByIndexOrZero(itemIndex));
        if (!item || !GetCurrentItemCount(item, nullptr))
        {
            self->vBeginWeaponStateChange(12, 0);
            goto finish;
        }
    }
    else
    {
        auto* weapon = reinterpret_cast<C_008319c0*>(self->vGetIndexedEntryOrZero());
        if (C_008319c0_GetSubObjectField74(weapon, nullptr) != 8)
        {
            CharActionFsm_AdvanceWeaponAction(self, nullptr);
            goto finish;
        }
    }

    {
        void* weapon = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, weapon, 0)[0xDC / 4])(weapon, nullptr, 0);

        weapon = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
        if (CharActionFsm_GetWeaponActionStatus(weapon, nullptr))
        {
            weapon = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());
            CharActionFsm_SetWeaponActionMode(weapon, nullptr, 1);
            self->vUpdateWeaponSelection(14);
        }
        else
        {
            self->vBeginWeaponStateChange(0, 0);
        }
    }

finish:
    CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0008), 0x2E4) &= ~0x10u;
    return CharActionFsm_SetCharacterActionFlag(
        reinterpret_cast<void*>(self->field_0008), nullptr, 16, 0);
}

HOOK(0x00483AF0, CCharActionFsm_UpdateWeaponAction, "auto");
