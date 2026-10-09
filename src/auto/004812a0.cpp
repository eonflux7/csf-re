#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot 0x60 on the target object (vtbl[24]), called with a single 0 argument.
// Returns the callee's result.
static int __fastcall CCharWeaponFsm_ActivateWeaponSlot(CCharWeaponFsm* self, void* /*edx*/, int a2)
{
    int slotIndex = self->vFindProviderIndex(a2);
    if (slotIndex == -1)
        return slotIndex;

    uint32_t* slotTable = reinterpret_cast<uint32_t*>(self->field_0018);
    void* weapon = reinterpret_cast<void*>(slotTable[slotIndex]);
    void** weaponVtbl = CSF_AT(void**, weapon, 0);
    return ((int(__fastcall*)(void*, void*, int))weaponVtbl[0x60 / 4])(weapon, nullptr, 0);
}

HOOK(0x4812A0, CCharWeaponFsm_ActivateWeaponSlot, "auto");
