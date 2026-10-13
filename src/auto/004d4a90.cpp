#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards the request to the virtual dispatcher of the object held in
// owner->field_0014->field_0030, passing the sub-object at owner+0x3D8 and flag 0.
static int __fastcall AiGiveObject_InvokeOwnerDispatch(void* self, void* /*edx*/) {
    // self->field_0008 holds the owner object pointer.
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x08);

    // owner->field_0014 holds an intermediate object pointer.
    uint8_t* intermediate = CSF_AT(uint8_t*, owner, 0x14);

    // intermediate->field_0030 holds the dispatch target object.
    void* dispatchTarget = CSF_AT(void*, intermediate, 0x30);

    // Address of the embedded sub-object at owner+0x3D8 (not a load).
    void* subObject = owner + 0x3D8;

    // Virtual slot 0xAC / 4 = 43 on the dispatch target's vtable.
    using DispatchFn = int(__fastcall*)(void* self, void* edx, void* arg, int flag);
    DispatchFn dispatch = reinterpret_cast<DispatchFn>(CSF_AT(void**, dispatchTarget, 0)[43]);

    return dispatch(dispatchTarget, nullptr, subObject, 0);
}

HOOK(0x4D4A90, AiGiveObject_InvokeOwnerDispatch, "auto");
