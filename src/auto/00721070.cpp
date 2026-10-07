#include <cstdint>
#include "hook.h"

// Target: 0x00721070 (25 bytes)
// __thiscall int sub_721070(CGuiEdit* self, int a2) -> ret 4
//
// Semantics (from disassembly):
//   1. eax = self->m_pText (CGuiEdit + 0x134)
//   2. if (eax == 0) -> return 0 (the null pointer value)
//   3. otherwise     -> tail-jump into pText->vtbl[24] (byte offset 0x60),
//                       with ecx = pText and the single stack argument a2.
//                       The callee pops the 4 argument bytes.
//
// The call target lives in an indirect vtable slot, so the tail jump is
// reproduced as a naked thunk that matches the original instructions.

static __declspec(naked) void CGuiEdit_ForwardTextCallToVtblSlot24()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x134]   // eax = self->m_pText
        test eax, eax
        je  no_text
        mov ecx, eax                       // ecx = pText (this for the virtual)
        mov eax, dword ptr [ecx]           // eax = pText->vtbl
        jmp dword ptr [eax + 0x60]         // tail-jump to vtbl slot 24 (ret 4)
    no_text:
        ret 4                              // return 0, pop the stack argument
    }
}

HOOK(0x721070, CGuiEdit_ForwardTextCallToVtblSlot24, "auto");
