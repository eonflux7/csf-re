#include "types/auto_classes.h"
#include "hook.h"

// Calls the virtual method at vtable slot +0x48 on the object stored in field_0004,
// passing that object as the single (callee-popped) stack argument.
static int __fastcall C_00863cd4_InvokeInnerVtbl48(C_00863cd4* self, void* /*edx*/)
{
    void* inner = reinterpret_cast<void*>(self->field_0004);
    void** inner_vtbl = *reinterpret_cast<void***>(inner);
    return (reinterpret_cast<int(__stdcall*)(void*)>(inner_vtbl[0x48 / 4]))(inner);
}

HOOK(0x007111F0, C_00863cd4_InvokeInnerVtbl48, "auto");
