#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Reads the child state object held in the owner's +0x48 slot. Its vtable slot 4 is a
// "can proceed" predicate (__thiscall, one stack arg: the weapon FSM). If it returns true,
// the owner-side child's slot 8 is invoked with the same argument.
static int __fastcall CCharWeaponFsm_UpdateChildIfReady(CCharWeaponFsm* self, void* /*edx*/) {
    int result = (int)self->field_0008;

    void* owner = reinterpret_cast<void*>(self->field_0008);
    uint32_t childHandle = CSF_AT(uint32_t, owner, 0x48);
    if (childHandle != 0) {
        void* child = reinterpret_cast<void*>(childHandle);
        result = ((int(__fastcall*)(void*, void*, CCharWeaponFsm*))
                  CSF_AT(void**, child, 0)[4])(child, nullptr, self);

        if ((uint8_t)result != 0) {
            // Original re-reads the owner and its child slot after the predicate call.
            owner = reinterpret_cast<void*>(self->field_0008);
            childHandle = CSF_AT(uint32_t, owner, 0x48);
            child = reinterpret_cast<void*>(childHandle);
            result = ((int(__fastcall*)(void*, void*, CCharWeaponFsm*))
                      CSF_AT(void**, child, 0)[8])(child, nullptr, self);
        }
    }
    return result;
}

HOOK(0x4801D0, CCharWeaponFsm_UpdateChildIfReady, "auto");
