#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static bool __fastcall C_008316d8_IsNonExcludedWeaponType(
    C_008316d8* self, void* /*edx*/)
{
    const int weaponType = CSF_AT(
        int, reinterpret_cast<void*>(self->field_0040), 0x74);

    return weaponType != 9
        && weaponType != 15
        && weaponType != 21
        && weaponType != 22
        && weaponType != 23
        && weaponType != 28
        && weaponType != 25
        && weaponType != 31
        && weaponType != 32;
}

HOOK(0x004767B0, C_008316d8_IsNonExcludedWeaponType, "auto");
