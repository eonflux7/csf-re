#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x004183B0 (8 bytes) -- tiny tail-call thunk:
//   mov ecx, [ecx+0x10]      ; inner = this->field_0010
//   mov eax, [ecx]           ; vtbl = *inner
//   jmp dword ptr [eax+0x34] ; tail-call vtbl[0x34/4](inner)
//
// No stack arguments, so the callee pops nothing (size 8, no `ret N`).

static int __fastcall InvokeInnerVtbl34(void* self, void* /*edx*/)
{
    // Re-read the inner object from the owner (this+0x10).
    void* inner = CSF_AT(void*, self, 0x10);

    // Virtual dispatch: slot at byte offset 0x34 == index 13.
    void** vtbl = CSF_AT(void**, inner, 0x0);
    return ((int(__fastcall*)(void*, void*))vtbl[0x34 / 4])(inner, nullptr);
}

HOOK(0x4183B0, InvokeInnerVtbl34, "auto");
