#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Sets the weapon FSM's current action. A non-(-1) action with a nonzero
// action-slot query notifies that slot object via its vtable entry at +0x38
// with argument 0.
static int __fastcall CCharWeaponFsm_SetCurrentWeaponAction(CCharWeaponFsm* self, void* /*edx*/, int a2)
{
    // Release the previously active action, if any.
    if (static_cast<int>(self->field_0024) != -1) {
        int previousSlotObject = self->vGetIndexedEntryOrZero();
        void* previousObj = reinterpret_cast<void*>(previousSlotObject);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, previousObj, 0)[15])(previousObj, nullptr);
    }

    int result = a2;
    self->field_0024 = static_cast<uint32_t>(a2);

    if (a2 != -1) {
        result = self->vGetIndexedEntryOrZero();
        if (result) {
            int slotObject = self->vGetIndexedEntryOrZero();
            void* obj = reinterpret_cast<void*>(slotObject);
            return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[14])(obj, nullptr, 0);
        }
    }
    return result;
}

HOOK(0x480970, CCharWeaponFsm_SetCurrentWeaponAction, "auto");
