// Module: char_action_fsm
// CCharWeaponFsm::CommitPendingWeaponState (0x00483810)

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Commits the pending sub-object at +0xA0 into the active slot at +0x9C,
// notifies it (virtual slot 0x30, arg 1), then refreshes the weapon
// sub-object when the field at +0x24 is valid, and finally issues the
// virtual state request 13 (0x0D) on this FSM.
static int __fastcall CCharWeaponFsm_CommitPendingWeaponState(CCharWeaponFsm* self, void* /*edx*/)
{
    uint32_t pendingObjectAddr = CSF_AT(uint32_t, self, 0xA0);
    if (pendingObjectAddr != 0)
    {
        void* pendingObject = reinterpret_cast<void*>(static_cast<uintptr_t>(pendingObjectAddr));

        CSF_AT(uint32_t, self, 0x9C) = pendingObjectAddr;
        CSF_AT(uint32_t, self, 0xA0) = 0;

        // pendingObject->vtbl[0x30 / 4](1)  (thiscall, one stack argument)
        ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, pendingObject, 0)[0x30 / 4])(pendingObject, nullptr, 1);

        if (self->field_0024 != static_cast<uint32_t>(-1))
        {
            void* weaponObject = reinterpret_cast<void*>(static_cast<uintptr_t>(self->vSlotD8()));

            // weaponObject->vtbl[0x30 / 4](1)
            ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, weaponObject, 0)[0x30 / 4])(weaponObject, nullptr, 1);
        }
    }

    return self->vSlot11C(0x0D);
}

HOOK(0x00483810, CCharWeaponFsm_CommitPendingWeaponState, "auto");
