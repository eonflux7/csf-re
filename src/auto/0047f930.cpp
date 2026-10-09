#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Resets the weapon FSM bookkeeping fields. If the active weapon index (+0x24)
// is valid (!= -1), it notifies the weapon object obtained via vSlotD8 through
// vtable slot 9 and then through vtable slot 14 with argument 0; the result of
// that last call is returned. Otherwise returns -1.
static int __fastcall CCharWeaponFsm_ResetWeaponState(CCharWeaponFsm* self, void* /*edx*/)
{
    const uint32_t activeWeaponIndex = self->field_0024;

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0064 = 0;
    self->field_002c = 0;
    self->field_0028 = 0xFFFFFFFFu;
    self->field_0069 = 1;
    self->field_006c = 0.0f;
    self->field_0068 = 0;
    self->field_0070 = 0;
    self->field_0071 = 0;

    if (activeWeaponIndex == 0xFFFFFFFFu)
        return -1;

    // First notification: vtable slot 9 (+0x24) on the object returned by vSlotD8.
    void* firstWeapon = reinterpret_cast<void*>(static_cast<uintptr_t>(self->vSlotD8()));
    ((void(__fastcall*)(void*, void*))(CSF_AT(void**, firstWeapon, 0)[9]))(firstWeapon, nullptr);

    // Second notification: vtable slot 14 (+0x38) with argument 0; its result is returned.
    void* secondWeapon = reinterpret_cast<void*>(static_cast<uintptr_t>(self->vSlotD8()));
    return ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, secondWeapon, 0)[14]))(secondWeapon, nullptr, 0);
}

HOOK(0x0047F930, CCharWeaponFsm_ResetWeaponState, "auto");
