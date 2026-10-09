#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"

// Returns a2 unchanged unless the FSM is in state 9 or the link check passes;
// in that case it forwards to the virtual slot 0x134 (vSlot134) with (0, 0).
static int __fastcall CCharWeaponFsm_TryForwardWhenLinked(CCharWeaponFsm* self, void* /*edx*/, uint32_t* a2) {
    int result = 0;

    if (self->m_stateCode != 9) {
        result = static_cast<int>(reinterpret_cast<uintptr_t>(a2));
        if (reinterpret_cast<CCharWeaponFsm*>(a2[1]) == self &&
            reinterpret_cast<CCharWeaponFsm*>(a2[0]) != self) {
            return self->vBeginWeaponStateChange(0, 0);
        }
    }
    return result;
}

HOOK(0x004807F0, CCharWeaponFsm_TryForwardWhenLinked, "auto");
