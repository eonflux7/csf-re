#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Dispatches to the virtual handler selected by the state index at +0x0C.
// Case k calls vtbl[0x16C/4 + k] with (this, a2), and the result is returned unchanged.
// States above 9 return the raw state value.
// The original tail-jumps with `ret 4`, so this __fastcall version (one stack arg) also pops 4.
static int __fastcall CCharWeaponFsm_DispatchStateHandler(CCharWeaponFsm* self, void* /*edx*/, int a2)
{
    unsigned int state = self->m_stateCode;
    switch (state)
    {
    case 0:
        return self->vProcessPendingWeaponAction(a2);
    case 1:
        // Slot 0x170 has no typed wrapper in the packet (vNullSub_170).
        return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x170 / 4])(self, nullptr, a2);
    case 2:
        return self->vUpdateWhenFlagSet(a2);
    case 3:
        return self->vReturnFlagOrCallSlot134(a2);
    case 4:
        return self->vDispatchStateTransition(a2);
    case 5:
        return self->vDispatchStateVirtual_60(a2);
    case 6:
        return self->vTryIssueWeaponAction(a2);
    case 7:
        return self->vDispatchWithReloadCheck(a2);
    case 8:
        return self->vForwardToVSlot1E0WhenState9(a2);
    case 9:
        return self->vEnsureWeaponReadyOrRequestState(a2);
    default:
        return static_cast<int>(state);
    }
}

HOOK(0x0047FB00, CCharWeaponFsm_DispatchStateHandler, "auto");
