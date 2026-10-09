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

// Copies the action parameter block (a2) into the weapon FSM, then switches the
// FSM into state (6, 3) through its virtual slot 0x134. Returns the slot result,
// or the failing check's value when either precondition is false.
static int __fastcall CCharWeaponFsm_BeginWeaponActionFromParams(CCharWeaponFsm* self, void* /*edx*/, int a2) {
    auto* params = reinterpret_cast<uint8_t*>(a2);

    int result = self->vIsField000cNull();
    if (static_cast<uint8_t>(result)) {
        result = self->vHasUsableWeapon();
        if (static_cast<uint8_t>(result)) {
            self->m_pendingActionFlag = 1;
            self->field_003c = CSF_AT(uint32_t, params, 0x4);
            self->field_0044 = CSF_AT(uint32_t, params, 0xC);
            self->field_0048 = CSF_AT(uint32_t, params, 0x10);
            self->field_004c = CSF_AT(uint32_t, params, 0x14);
            CSF_AT(uint32_t, self, 0x50) = CSF_AT(uint32_t, params, 0x18);
            CSF_AT(uint32_t, self, 0x54) = CSF_AT(uint32_t, params, 0x1C);
            self->field_005c = CSF_AT(uint8_t, params, 0x20);
            self->field_005f = CSF_AT(uint8_t, params, 0x21);
            self->field_0060 = CSF_AT(uint8_t, params, 0x22);
            self->field_0068 = 0;
            return self->vBeginWeaponStateChange(6, 3);
        }
    }
    return result;
}

HOOK(0x480A30, CCharWeaponFsm_BeginWeaponActionFromParams, "auto");
