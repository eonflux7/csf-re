#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetMemberValueD4, 0x00476090, (void* self, void* edx));
CSF_FUNC(int, __fastcall, EvaluateWeaponMemberCount, 0x0070E390, (void* self, void* edx));

// Chooses the next weapon FSM state from a member value read through vSlotD8.
static int __fastcall CCharWeaponFsm_DispatchWeaponState(CCharWeaponFsm* self, void* /*edx*/)
{
    int weaponHandle = self->vSlotD8();
    int memberValueMinusOne = GetMemberValueD4(reinterpret_cast<void*>(static_cast<uintptr_t>(weaponHandle)), nullptr) - 1;

    if (memberValueMinusOne != 0)
    {
        int result = memberValueMinusOne - 1;
        if (result == 0)
            return self->vSlot138(1);
        return result;
    }

    int weaponHandleAgain = self->vSlotD8();
    C_008639b0* weaponObject = reinterpret_cast<C_008639b0*>(static_cast<uintptr_t>(weaponHandleAgain));
    if (EvaluateWeaponMemberCount(weaponObject, nullptr) <= 0)
        return self->vSlot134(0, 0);

    return self->vSlot138(2);
}

HOOK(0x4802C0, CCharWeaponFsm_DispatchWeaponState, "auto");
