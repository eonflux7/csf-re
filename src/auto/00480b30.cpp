#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"

// Tick of the weapon FSM: if m_stateCode == 3, delegate to the virtual at slot 0x134
// with (5, 0) and return its result. Otherwise clear field_0064 when it equals 3
// and return 3.
static int __fastcall CCharWeaponFsm_TickStateTransition(CCharWeaponFsm* self, void* /*edx*/) {
    int result = 3;
    if (self->m_stateCode == 3) {
        return self->vBeginWeaponStateChange(5, 0);
    }
    if (self->field_0064 == 3) {
        self->field_0064 = 0;
    }
    return result;
}

HOOK(0x480B30, CCharWeaponFsm_TickStateTransition, "auto");
