#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Tail-jump thunk: forwards to the __thiscall virtual method at vtable slot 15
// (byte offset 0x3c ). The original does `jmp [eax+0x3c]`, so no stack bytes are
// popped here -- the callee pops its own arguments.
static int __fastcall C_0082d5e0_InvokeVtblSlot15(C_0082d5e0* self, void* /*edx*/)
{
    return ((int(__fastcall*)(void*, void*))self->vtbl[15])(self, nullptr);
}

HOOK(0x00407ED0, C_0082d5e0_InvokeVtblSlot15, "auto");
