#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: int __thiscall sub_4DB950(C_0083dfa0* this)
// Loads the object pointer at field_0004 and calls the function pointer stored at +0x3C inside
// that object, with ecx = &field_0004 and one stack argument = this + 8. The callee pops its argument.
// With a null `this`, the original reads the dword at address 0 as the object pointer, then calls the
// function pointer at +0x3C of that object with ecx = 0 and argument 8.
static int __fastcall C_0083dfa0_ForwardQueryThroughSubobjectVtbl(C_0083dfa0* self, void* /*edx*/) {
    if (self != nullptr) {
        // edx = [ecx+4]; ecx = &field_0004; push (ecx+8); call [edx+0x3C]
        void* subobject = reinterpret_cast<void*>(self->field_0004);
        auto target = reinterpret_cast<int (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void*, subobject, 0x3C));
        void* thisForCall = reinterpret_cast<void*>(&self->field_0004);
        void* subobjectArg = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 8);
        return target(thisForCall, nullptr, subobjectArg);
    }

    // Null path: edx = [0]; ecx = 0; push 8; call [edx+0x3C].
    void* nullBase = nullptr;
    void* zeroPageValue = CSF_AT(void*, nullBase, 0);
    auto target = reinterpret_cast<int (__fastcall*)(void*, void*, uintptr_t)>(
        CSF_AT(void*, zeroPageValue, 0x3C));
    return target(nullptr, nullptr, static_cast<uintptr_t>(8));
}

HOOK(0x4DB950, C_0083dfa0_ForwardQueryThroughSubobjectVtbl, "auto");
