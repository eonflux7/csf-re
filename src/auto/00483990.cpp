#include <cstdint>
#include "globals.h"

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Bindings for original functions called by this routine.
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_483540, 0x00483540, (void* self, void* edx, float a2));

// Original: 0x00483990, __thiscall, ret 0.
// If the field at +0x24 is not -1, reads a float from the value block of the handle
// returned by the virtual slot 0xD8, applies it through sub_483540, then returns the
// result of the virtual slot 0x11C called with 0.
static int __fastcall CCharActionFsm_UpdateFromHandleValueAndFinish(CCharActionFsm* self, void* /*edx*/)
{
    if (self->field_0024 != -1)
    {
        int handleValue = self->vSlotD8();
        int valuePointer = Handle_GetValuePointer(
            reinterpret_cast<void*>(static_cast<uintptr_t>(handleValue)), nullptr);
        float paramValue = CSF_AT(float, reinterpret_cast<void*>(static_cast<uintptr_t>(valuePointer)), 0x1C0);
        sub_483540(self, nullptr, paramValue);
    }
    return self->vSlot11C(0);
}

HOOK(0x00483990, CCharActionFsm_UpdateFromHandleValueAndFinish, "auto");
