#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Returns true when the FSM's null-check passes and the equipped weapon object
// reports ready through its vtable slot +0x68.
static bool __fastcall CCharWeaponFsm_CanUseEquippedWeapon(CCharWeaponFsm* self, void* /*edx*/)
{
    bool result = false;

    if (static_cast<uint8_t>(self->vIsField000cNull()) != 0)
    {
        void* weapon = reinterpret_cast<void*>(static_cast<uintptr_t>(self->vSlotDC()));
        auto weaponSlot68 = reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(
            CSF_AT(void**, weapon, 0)[0x68 / 4]);

        if (weaponSlot68(weapon, nullptr) != 0)
            return true;
    }
    return result;
}

HOOK(0x480B80, CCharWeaponFsm_CanUseEquippedWeapon, "auto");
