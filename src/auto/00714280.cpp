// Commandos: Strike Force (2006) — reimplementation of 0x00714280.
// Tail-forwards the call to virtual slot 11 (byte offset 0x2C) of the sub-object
// stored at this+0x17C, with that sub-object as `this` and no stack arguments.
// The callee's stack cleanup (ret N) is unknown, so this stays a naked thunk
// that reproduces the original register/stack forwarding exactly.

#include <cstdint>
#include "hook.h"

static __declspec(naked) void ForwardToEmbeddedObjectVtableSlot11()
{
    __asm
    {
        mov ecx, dword ptr [ecx + 0x17C]   // ecx = sub-object at this+0x17C
        mov eax, dword ptr [ecx]           // eax = its vftable
        jmp dword ptr [eax + 0x2C]         // tail-call slot 11, args unchanged
    }
}

HOOK(0x00714280, ForwardToEmbeddedObjectVtableSlot11, "auto");
