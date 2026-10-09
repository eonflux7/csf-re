#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_4847C0, 0x004847C0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_4950F0, 0x004950F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00495140_ForwardInnerObjectVirtualCall, 0x00495140, (void* self, void* edx));

// Forwards the inner object (field_0008) through 0x4950F0 / 0x495140 when the
// predicate fails, refreshes the action state, then dispatches virtual slot 0x11C with id 13.
static int __fastcall CCharActionFsm_CheckInnerObjectAndDispatch(CCharActionFsm* self, void* /*edx*/)
{
    if (!sub_4950F0(reinterpret_cast<void*>(self->field_0008), nullptr))
    {
        C_00495140_ForwardInnerObjectVirtualCall(reinterpret_cast<void*>(self->field_0008), nullptr);
    }
    sub_4847C0(self, nullptr);
    return self->vUpdateWeaponSelection(13);
}

HOOK(0x484A10, CCharActionFsm_CheckInnerObjectAndDispatch, "auto");
