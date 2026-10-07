#include "hook.h"
#include "types/auto_classes.h"

// 0x004F0CC0 — tail-call thunk: forwards to the virtual method at vtable offset 0x8C (slot 35),
// passing `this` unchanged and returning its result.
static int __fastcall C_0083ef30_ForwardVtbl8C(C_0083ef30* self, void* /*edx*/)
{
    return ((int(__fastcall*)(void*, void*))self->vtbl[0x8c / 4])(self, nullptr);
}

HOOK(0x004F0CC0, C_0083ef30_ForwardVtbl8C, "auto");
