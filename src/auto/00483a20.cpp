#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee: sets or clears a status flag on the owner object (bind as given).
CSF_FUNC(int, __fastcall, Owner_SetStatusFlag, 0x00498F50, (void* self, void* edx, int a2, char a3));

// Callee: returns the number of items currently loaded (bind as given).
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

static int __fastcall CCharWeaponFsm_EvaluateReloadOrAttack(CCharWeaponFsm* self, void* /*edx*/)
{
    // First virtual: resolve the weapon object for this state machine.
    void* weaponObject = reinterpret_cast<void*>(self->vGetIndexedEntryOrZero());

    if (GetCurrentItemCount(weaponObject, nullptr) == 0) {
        // Ask the weapon object (vtbl slot 0x68 / 4 = 26) whether it can act.
        void* weaponVtableEntry = CSF_AT(void**, weaponObject, 0)[26];
        bool weaponCanAct = reinterpret_cast<bool(__fastcall*)(void*, void*)>(weaponVtableEntry)(weaponObject, nullptr);

        if (weaponCanAct) {
            // Set bit 0x80 in the owner's status word at +0x2E4 and mark the owner.
            void* owner = reinterpret_cast<void*>(self->field_0008);
            Owner_SetStatusFlag(owner, nullptr, 0x4000, 0);

            owner = reinterpret_cast<void*>(self->field_0008);
            CSF_AT(uint32_t, owner, 0x2E4) |= 0x80u;

            owner = reinterpret_cast<void*>(self->field_0008);
            Owner_SetStatusFlag(owner, nullptr, 0x80, 1);

            return self->vBeginWeaponStateChange(4, 0);
        }
    }

    void* owner = reinterpret_cast<void*>(self->field_0008);
    Owner_SetStatusFlag(owner, nullptr, 0x4000, 1);
    return self->vUpdateWeaponSelection(45);
}

HOOK(0x00483A20, CCharWeaponFsm_EvaluateReloadOrAttack, "auto");
