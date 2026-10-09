#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee bindings (by address).
CSF_FUNC(int, __fastcall, GetWeaponItemKey, 0x00479310, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CCharActionFsm_HandleNoWeaponItem, 0x004846D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

// Reads the weapon object's state code (vtable slot 0x4C), then dispatches:
// code 11 resolves the current item and, if it is empty, runs the no-weapon handler;
// code 13 runs the no-weapon handler directly. Always ends with the vSlot134 call.
static int __fastcall CCharActionFsm_UpdateWeaponActionState(CCharActionFsm* self, void* /*edx*/)
{
    int stateCode = 0;
    if (self->field_009c)
    {
        void* weaponObj = reinterpret_cast<void*>(self->field_009c);
        auto getStateCode = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, weaponObj, 0)[0x4C / 4]);
        stateCode = getStateCode(weaponObj, nullptr);
    }

    if (stateCode == 11)
    {
        void* weaponObj = reinterpret_cast<void*>(self->field_009c);
        int itemKey = GetWeaponItemKey(weaponObj, nullptr);
        void* item = reinterpret_cast<void*>(self->vSlotEC(itemKey));
        if (!item)
            return self->vSlot134(0, 0);
        if (GetCurrentItemCount(item, nullptr) == 0)
            CCharActionFsm_HandleNoWeaponItem(self, nullptr);
    }
    else if (stateCode == 13)
    {
        CCharActionFsm_HandleNoWeaponItem(self, nullptr);
    }

    return self->vSlot134(0, 0);
}

HOOK(0x004849A0, CCharActionFsm_UpdateWeaponActionState, "auto");
