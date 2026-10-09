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
#include "legacy/string.h"

// If the weapon FSM is not in state 9, mark the flag at +0x68 and dispatch
// virtual slot 0x134 with arguments (6, 4). Otherwise do nothing.
static int __fastcall CCharWeaponFsm_DispatchStateIfNotNine(CCharWeaponFsm* self, void* /*edx*/) {
    if (self->m_stateCode != 9) {
        self->field_0068 = 1;
        return self->vBeginWeaponStateChange(6, 4);
    }
    return 0;
}

HOOK(0x480950, CCharWeaponFsm_DispatchStateIfNotNine, "auto");
