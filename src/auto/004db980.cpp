#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: int __thiscall sub_4DB980(C_0083dfa0* this)
// Forwards to the function pointer stored at (field_0004 + 0x40), passing &field_0004 as `this`
// and &field_0008 as its argument. With a null `this`, the original reads the dword at
// address 0 and calls through it at +0x40 with arguments (0, 8).
static int __fastcall C_0083dfa0_QueryStateViaSlot40Delegate(C_0083dfa0* self, void* /*edx*/) {
    if (self != nullptr) {
        // edx = [ecx+4]; call [edx+0x40] with ecx = &field_0004 and pushed = ecx+8
        void* delegateObject = reinterpret_cast<void*>(self->field_0004);
        void* delegate = CSF_AT(void*, delegateObject, 0x40);
        void* subobjectArg = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 8);
        void* thisForCall = reinterpret_cast<void*>(&self->field_0004);
        return reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(delegate)(
            thisForCall, nullptr, subobjectArg);
    }

    // Null path: edx = [0]; call [edx+0x40] with ecx = 0 and pushed = 0 + 8.
    void* nullBase = nullptr;
    void* zeroPageValue = CSF_AT(void*, nullBase, 0);
    void* delegate = CSF_AT(void*, zeroPageValue, 0x40);
    return reinterpret_cast<int (__fastcall*)(void*, void*, uintptr_t)>(delegate)(
        nullptr, nullptr, static_cast<uintptr_t>(8));
}

HOOK(0x4DB980, C_0083dfa0_QueryStateViaSlot40Delegate, "auto");
