#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00681F40 GetCurrentItemCount, __fastcall with ecx = this, no stack args.
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

// Returns true when the weapon FSM has a valid weapon slot (field_0024 != -1 path),
// the weapon object reports ready through its vtable slot +0x9C, and, when the
// slot is set, the item count is positive.
static char __fastcall CCharWeaponFsm_HasUsableWeapon(CCharWeaponFsm* self, void* /*edx*/)
{
    const bool hasWeaponSlot = self->field_0024 != 0xFFFFFFFFu;

    if (!self->vGetIndexedEntryOrZero_37())
        return static_cast<char>(hasWeaponSlot);

    int weaponHandle = self->vGetIndexedEntryOrZero_37();
    void* weapon = reinterpret_cast<void*>(weaponHandle);

    // Virtual slot +0x9C (index 39), thiscall with no stack arguments.
    unsigned char weaponReady =
        ((unsigned char (__fastcall*)(void*, void*))CSF_AT(void**, weapon, 0)[39])(weapon, nullptr);
    if (!weaponReady)
        return static_cast<char>(hasWeaponSlot);

    if (hasWeaponSlot) {
        int itemCountOwner = self->vGetIndexedEntryOrZero_37();
        void* itemOwner = reinterpret_cast<void*>(itemCountOwner);
        if (GetCurrentItemCount(itemOwner, nullptr) > 0)
            return 1;
    }
    return 0;
}

HOOK(0x00480BB0, CCharWeaponFsm_HasUsableWeapon, "auto");
