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
// The original ends each state in an indirect tail jmp through a vtable slot, and the
// jump table at 0x4801A0 does not map state i to slot 0x144+4i: state 0 -> 0x144,
// 1 -> 0x14C, 2 -> 0x150, ..., 8 -> 0x168, 9 -> 0x148. Here the slot is called with no
// extra arguments and its eax is returned, which is equivalent for a method that takes
// only `this`. Out-of-range states return the stored value.
static int __fastcall CCharWeaponFsm_DispatchStateVirtual(CCharWeaponFsm* self, void* /*edx*/)
{
    const uint32_t state = self->m_stateCode;
    int result = (int)state;

    switch (state) {
    case 0:
        return self->vUpdateChildIfReady();
    case 1:
        return self->vInvokeVirtualSlot71With12();
    case 2:
        return self->vEnableSubobjectThenQuery();
    case 3:
        return self->vDispatchWeaponKindAction();
    case 4:
        return self->vDispatchWeaponState();
    case 5:
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x15C / 4])(self, nullptr);
    case 6:
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x160 / 4])(self, nullptr);
    case 7:
        return self->vInvokeVirtual138();
    case 8:
        // Slot 0x168 is vLaunchProjectile (char-typed in the header): read the full eax as the original does.
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x168 / 4])(self, nullptr);
    case 9:
        // Slot 0x148 is the void wrapper vResetField0028; read eax through an int-returning
        // pointer so the callee's eax is preserved as the original does.
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x148 / 4])(self, nullptr);
    default:
        return result;
    }
}

HOOK(0x00480140, CCharWeaponFsm_DispatchStateVirtual, "auto");
