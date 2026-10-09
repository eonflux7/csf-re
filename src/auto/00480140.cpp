#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Dispatches on the weapon FSM state id (field +0x0C, unsigned range 0..9).
// Each state forwards to one virtual slot. The original ends in a tail jmp; here the
// slot is called with no extra arguments and its eax is returned, which is equivalent
// for a method that takes only `this`. Out-of-range states return the stored value.
static int __fastcall CCharWeaponFsm_DispatchStateVirtual(CCharWeaponFsm* self, void* /*edx*/)
{
    const uint32_t state = self->m_stateCode;
    int result = (int)state;

    switch (state) {
    case 0:
        return self->vUpdateChildIfReady();
    case 1:
        // Slot 0x148 is the void wrapper vResetField0028; read eax through an int-returning
        // pointer so the callee's eax is preserved as the original does.
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x148 / 4])(self, nullptr);
    case 2:
        return self->vInvokeVirtualSlot71With12();
    case 3:
        return self->vEnableSubobjectThenQuery();
    case 4:
        return self->vDispatchWeaponKindAction();
    case 5:
        return self->vDispatchWeaponState();
    case 6:
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x15C / 4])(self, nullptr);
    case 7:
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x160 / 4])(self, nullptr);
    case 8:
        return self->vInvokeVirtual138();
    case 9:
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x168 / 4])(self, nullptr);
    default:
        return result;
    }
}

HOOK(0x00480140, CCharWeaponFsm_DispatchStateVirtual, "auto");
