#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00407F30 - tail-call thunk: forwards to the virtual method at vtable
// offset 0x44 (slot 17), passing the same `this` pointer and no other args.
static int __fastcall C_0082d5e0_DispatchVirtual17(C_0082d5e0* self, void* /*edx*/)
{
    return ((int(__fastcall*)(void*, void*))self->vtbl[0x44 / 4])(self, nullptr);
}

HOOK(0x00407F30, C_0082d5e0_DispatchVirtual17, "auto");
