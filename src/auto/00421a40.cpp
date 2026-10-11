#include <cstdint>
#include "globals.h"
#include "hook.h"

// Sets the pending task-state value on `self` and forwards the request to the
// child object's virtual slot 35 (+0x8C) with state code 42.
static int __fastcall TaskStates_SetStateAndDispatch(void* self, void* /*edx*/, int newState) {
    // Mark the owner's child (self->owner->child) with flag bit 8 at +0x1F0.
    void* ownerChild = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14);
    CSF_AT(uint32_t, ownerChild, 0x1F0) |= 8u;

    // Store the new state in this object's +0x14 slot.
    CSF_AT(int, self, 0x14) = newState;

    // Dispatch through the child's vtable slot 35 (+0x8C) with args (42, 0, -1, 0).
    void* dispatchTarget = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x14);
    using DispatchFn = int(__fastcall*)(void*, void*, int, int, int, int);
    DispatchFn dispatch = reinterpret_cast<DispatchFn>(CSF_AT(void**, dispatchTarget, 0)[0x8C / 4]);
    return dispatch(dispatchTarget, nullptr, 42, 0, -1, 0);
}

HOOK(0x00421A40, TaskStates_SetStateAndDispatch, "auto");
