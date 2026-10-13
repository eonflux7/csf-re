#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee: tail-jumped to with ecx = this, no stack args, ret 0.
CSF_FUNC(void, __fastcall, RpPrtStd_ResetBaseState, 0x006C9FC0, (void* self, void* edx));

// Original-owned virtual slot 0x04 on the object stored in field_0094 (a __thiscall
// method taking one int argument). Called through its vtable with edx unused.
static void __fastcall C_00861508_ResetRenderState(C_00861508* self, void* /*edx*/) {
    // Slot 0x2C of this object: set flag 0x0C to 1.
    self->vSetFlag0C(1);

    // Release the secondary object held at +0x94, if any.
    uint32_t heldObject = self->field_0094;
    if (heldObject) {
        void* heldPtr = reinterpret_cast<void*>(heldObject);
        // Vtable slot +0x4 of the held object (index 1), called with argument 1.
        ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, heldPtr, 0)[1]))(heldPtr, nullptr, 1);
        self->field_0094 = 0;
    }

    // Tail jump to RpPrtStd_ResetBaseState with this in ecx.
    RpPrtStd_ResetBaseState(reinterpret_cast<void*>(self), nullptr);
}

HOOK(0x006DC950, C_00861508_ResetRenderState, "auto");
