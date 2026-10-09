#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x0047FE80, __thiscall, one stack argument (a2, unused), ret 4.
// If the active flag at +0x69 is set, fetch the weapon object via slot 0xD8,
// call its slot 0x70 with the byte at +0x70, then call this object's slot 0x134
// with (+0x64, 0) and return that result. Otherwise return the flag (0).
static char __fastcall CCharWeaponFsm_ApplyWeaponStateIfActive(CCharWeaponFsm* self, void* /*edx*/, int /*a2*/)
{
    char isActive = static_cast<char>(self->field_0069);
    if (!isActive)
    {
        return isActive;
    }

    void* weaponObject = reinterpret_cast<void*>(self->vSlotD8());
    uint8_t stateByte = self->field_0070;
    // Slot 0x70 / 4 = 28 on the weapon object's vtable.
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, weaponObject, 0)[0x70 / 4])(
        weaponObject, nullptr, static_cast<int>(stateByte));

    return static_cast<char>(self->vSlot134(static_cast<int>(self->field_0064), 0));
}

HOOK(0x0047FE80, CCharWeaponFsm_ApplyWeaponStateIfActive, "auto");
