#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bindings for original callees (addresses from the packet).
CSF_FUNC(int, __fastcall, sub_480220, 0x00480220, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, IsInnerObjectReadyForDispatch, 0x004950F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00495140_ForwardInnerObjectVirtualCall, 0x00495140, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Inner_RunActionWithFlag, 0x00498F50, (void* self, void* edx, int a2, char a3));

// Original: 0x00482400, __thiscall, ret 0.
// Advances the action state machine. Always pings the vtable slot first; then, if the
// inner object is ready, runs the action and dispatches with (512, 1); otherwise forwards.
static int __fastcall CCharActionFsm_ProcessStateAndDispatch(CCharActionFsm* self, void* /*edx*/)
{
    self->vSlot1E8();

    void* innerObject = reinterpret_cast<void*>(self->field_0008);
    if (!IsInnerObjectReadyForDispatch(innerObject, nullptr))
    {
        innerObject = reinterpret_cast<void*>(self->field_0008);
        return C_00495140_ForwardInnerObjectVirtualCall(innerObject, nullptr, 0);
    }

    sub_480220(self, nullptr);
    innerObject = reinterpret_cast<void*>(self->field_0008);
    return Inner_RunActionWithFlag(innerObject, nullptr, 0x200, 1);
}

HOOK(0x00482400, CCharActionFsm_ProcessStateAndDispatch, "auto");
