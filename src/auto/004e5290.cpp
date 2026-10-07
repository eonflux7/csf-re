#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Tail-call thunk: forwards to the virtual method at vtable offset 0xB0 (slot 44),
// passing the same `this` pointer. The callee's return value is returned unchanged.
static int __fastcall C_0083e9d0_InvokeVtblB0(C_0083e9d0* self, void* /*edx*/)
{
    return ((int(__fastcall*)(void*, void*))self->vtbl[0xb0 / 4])(self, nullptr);
}

HOOK(0x4E5290, C_0083e9d0_InvokeVtblB0, "auto");
