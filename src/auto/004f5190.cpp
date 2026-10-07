// Commandos: Strike Force -- reimplementation of 0x004F5190
//
// Original (12 bytes):
//   004F5190  push ebp
//   004F5191  mov  ebp, esp
//   004F5193  mov  ecx, [ecx+4]          ; inner object pointer stored at self+4
//   004F5196  mov  eax, [ecx]            ; inner object's vtable
//   004F5198  pop  ebp
//   004F5199  jmp  dword ptr [eax+0x28]  ; tail-call vtable slot 10 with ecx = inner
//
// The tail jump forwards the caller's (empty) stack arguments unchanged and
// returns the callee's value directly to the original caller.

#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall InvokeInnerObjectVirtual28(void** self, void* /*edx*/)
{
    void*  inner = CSF_AT(void*, self, 4);
    void** vtbl  = CSF_AT(void**, inner, 0);
    return ((int(__fastcall*)(void*, void*))vtbl[10])(inner, nullptr);
}

HOOK(0x4F5190, InvokeInnerObjectVirtual28, "auto");
