#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Dispatches on the kind code returned by the object from vSlotD8 (slot 0x48 on that object).
// Kind 0 -> slot 0x134(field_0064, 0); kinds 1..3 -> slot 0x138(5/6/7); otherwise returns the kind.
static int __fastcall CCharWeaponFsm_DispatchWeaponKindAction(CCharWeaponFsm* self, void* /*edx*/) {
    int weaponObjectValue = self->vSlotD8();
    void* weaponObject = reinterpret_cast<void*>(static_cast<uintptr_t>(weaponObjectValue));

    // vtable slot 0x48 => index 18 on the returned object (its type is not declared here).
    int weaponKind = ((int(__fastcall*)(void*, void*))(CSF_AT(void**, weaponObject, 0)[0x48 / 4]))(weaponObject, nullptr);

    int result = weaponKind;
    switch (weaponKind) {
    case 0:
        result = self->vSlot134(static_cast<int>(self->field_0064), 0);
        break;
    case 1:
        result = self->vSlot138(5);
        break;
    case 2:
        result = self->vSlot138(6);
        break;
    case 3:
        result = self->vSlot138(7);
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x00480250, CCharWeaponFsm_DispatchWeaponKindAction, "auto");
