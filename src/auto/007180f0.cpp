#include <cstdint>
#include "hook.h"

// Target: 0x007180F0 (28 bytes)
// __thiscall int sub_7180F0(CGuiControl* self, int a2)   -> ret 4
//
// Semantics (from disassembly):
//   1. eax = self->m_pParent (offset 0xa4)
//   2. if (eax == 0) -> return 0 (eax holds the null pointer)
//   3. otherwise     -> tail-jump into parent->vtbl[0x21c / 4 = 135]
//                       with ecx = parent and the single stack argument a2.
//                       The callee pops the 4 argument bytes.
//
// The call target lives in an indirect vtable slot, so the tail jump is
// reproduced as a naked thunk that matches the original instructions exactly.

static __declspec(naked) void CGuiControl_SendEventToParent()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0xa4]   // eax = self->m_pParent
        test eax, eax
        je  no_parent
        mov ecx, eax                      // ecx = parent (this for the virtual)
        mov eax, dword ptr [ecx]          // eax = parent->vtbl
        jmp dword ptr [eax + 0x21c]       // tail-jump to vtbl slot 135 (ret 4)
    no_parent:
        ret 4                             // return 0, pop the stack argument
    }
}

HOOK(0x7180F0, CGuiControl_SendEventToParent, "auto");
