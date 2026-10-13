#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: int __thiscall sub_4DB8C0(C_0083dfa0* this)
// Non-null: forwards to the function pointer stored at (*(this->field_0004) + 0x30), passing
// &this->field_0004 as `this` and &this->field_0008 as its single stack argument.
// Null: reads the dword at address 0 and calls through it at +0x30 with ecx = 0 and argument 8.
// Returns the callee's eax unchanged; pops nothing (ret 0).
static int __fastcall C_0083dfa0_QueryDelegateState(C_0083dfa0* self, void* /*edx*/) {
    if (self != nullptr) {
        // edx = [ecx+4] (delegate object); call [edx+0x30] with ecx = ecx+4, pushed = ecx+8
        void* delegateObject = reinterpret_cast<void*>(self->field_0004);
        void* delegate = CSF_AT(void*, delegateObject, 0x30);
        void* subobjectArg = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 8);
        void* thisForCall = reinterpret_cast<void*>(&self->field_0004);
        return reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(delegate)(
            thisForCall, nullptr, subobjectArg);
    }

    // Null path: edx = [0]; call [edx+0x30] with ecx = 0 and pushed = 0 + 8.
    void* nullBase = nullptr;
    void* zeroPageValue = CSF_AT(void*, nullBase, 0);
    void* delegate = CSF_AT(void*, zeroPageValue, 0x30);
    return reinterpret_cast<int (__fastcall*)(void*, void*, uintptr_t)>(delegate)(
        nullptr, nullptr, static_cast<uintptr_t>(8));
}

HOOK(0x4DB8C0, C_0083dfa0_QueryDelegateState, "auto");
