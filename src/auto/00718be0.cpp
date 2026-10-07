#include <cstdint>
#include "hook.h"

// Target: 0x00718BE0 (25 bytes)
// __thiscall int sub_718BE0(CGuiControl* self, int a2)  -> ret 4
//
// Semantics (from disassembly):
//   1. eax = self->m_pEffects (offset 0x128)
//   2. if (eax == 0) -> eax already holds 0, pop the stack argument and return
//   3. otherwise     -> tail-jump into obj->vtbl[7] (byte offset 0x1c) with
//                       ecx = obj and the single stack argument a2 unchanged.
//                       The callee is __thiscall (ret 4).
//
// The call target lives in an indirect vtable slot, so the tail jump is
// reproduced as a naked thunk that matches the original instructions exactly.

static __declspec(naked) void CGuiControl_ForwardArgToFirstEffect()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x128]   // obj = self->m_pEffects (first effect in list)
        test eax, eax
        je  no_effects
        mov ecx, eax                        // ecx = obj (this for the virtual call)
        mov eax, dword ptr [ecx]            // eax = obj->vtbl
        jmp dword ptr [eax + 0x1c]          // tail-jump to vtbl slot 7 (ret 4)
    no_effects:
        ret 4                               // return 0 (eax), pop the stack argument
    }
}

HOOK(0x718BE0, CGuiControl_ForwardArgToFirstEffect, "auto");
