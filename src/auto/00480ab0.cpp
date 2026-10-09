#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies the 36-byte weapon-state block passed in a2 into the FSM and, if the
// weapon slot is empty and the second predicate holds, enters the state (6, 3).
static int __fastcall CCharWeaponFsm_CopyWeaponDataAndSetState(CCharWeaponFsm* self, void* /*edx*/, int a2)
{
    int result = self->vIsField000cNull();
    if (static_cast<uint8_t>(result))
    {
        char ready = self->vHasUsableWeapon();
        result = ready;
        if (static_cast<uint8_t>(ready))
        {
            uint8_t* src = reinterpret_cast<uint8_t*>(a2);

            self->m_pendingActionFlag = 0;
            self->field_003c = CSF_AT(uint32_t, src, 0x04);
            self->field_0044 = CSF_AT(uint32_t, src, 0x0C);
            self->field_0048 = CSF_AT(uint32_t, src, 0x10);
            self->field_004c = CSF_AT(uint32_t, src, 0x14);
            CSF_AT(uint32_t, self, 0x50) = CSF_AT(uint32_t, src, 0x18);
            CSF_AT(uint32_t, self, 0x54) = CSF_AT(uint32_t, src, 0x1C);
            self->field_005c = CSF_AT(uint8_t, src, 0x20);
            self->field_005f = CSF_AT(uint8_t, src, 0x21);
            uint8_t flagByte = CSF_AT(uint8_t, src, 0x22);
            self->field_0068 = 0;
            self->field_0060 = flagByte;
            return self->vBeginWeaponStateChange(6, 3);
        }
    }
    return result;
}

HOOK(0x480AB0, CCharWeaponFsm_CopyWeaponDataAndSetState, "auto");
