#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

// Bound callees (addresses from the packet).
CSF_FUNC(void, __fastcall, CharWeaponFsm_ClearTransientState, 0x004847C0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, InnerObject_TryHandleCall, 0x004950F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00495140_ForwardInnerObjectVirtualCall, 0x00495140, (void* self, void* edx, int mode));

static char __fastcall CCharActionFsm_CommitPendingActionAndForward(CCharActionFsm* self, void* /*edx*/)
{
    // Stop the active item/handle object if one is set.
    if (self->field_0024 != 0xFFFFFFFFu)
    {
        // vSlotD8 yields the object pointer as a raw int (eax), used as `this` below.
        int activeValue = self->vSlotD8();
        void* activeObj = reinterpret_cast<void*>(static_cast<uintptr_t>(activeValue));
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, activeObj, 0)[12])(activeObj, nullptr, 0);
    }

    // Move a pending object into the "previous" slot and release it.
    uint32_t pendingValue = self->field_009c;
    if (pendingValue != 0)
    {
        self->field_00a0 = pendingValue;
        self->field_009c = 0;
        void* pendingObj = reinterpret_cast<void*>(static_cast<uintptr_t>(pendingValue));
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, pendingObj, 0)[12])(pendingObj, nullptr, 0);
    }

    self->field_00a4 = self->field_0088;
    CharWeaponFsm_ClearTransientState(self, nullptr);

    int slotResult = self->vSlot11C(46);
    char result = static_cast<char>(slotResult);
    if (slotResult != -1)
    {
        void* innerObj = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008));
        result = InnerObject_TryHandleCall(innerObj, nullptr) ? 1 : 0;
        if (result == 0)
        {
            void* innerObj2 = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0008));
            result = static_cast<char>(C_00495140_ForwardInnerObjectVirtualCall(innerObj2, nullptr, 1));
        }
    }
    return result;
}

HOOK(0x00484A40, CCharActionFsm_CommitPendingActionAndForward, "auto");
