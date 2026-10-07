#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x0069A390
// Tail-forwards to the virtual method in slot 1 (vtable offset +4) of the object
// held in this->field_0004, with that inner object passed as `this` (ecx).
static int __fastcall C_0085e760_CallInnerVirtualSlot1(C_0085e760* self, void* /*edx*/)
{
    void* inner = reinterpret_cast<void*>(self->field_0004);
    void** inner_vtbl = *reinterpret_cast<void***>(inner);
    return (reinterpret_cast<int(__fastcall*)(void*, void*)>(inner_vtbl[1]))(inner, nullptr);
}

HOOK(0x0069A390, C_0085e760_CallInnerVirtualSlot1, "auto");
