#include <cstdint>
#include "hook.h"

// Target: 0x00725330 (28 bytes)
// __thiscall int sub_725330(CGuiButton* self, int a2)   -> ret 4
//
// Semantics (from disassembly):
//   1. eax = self->m_pText (CGuiButton offset 0x134)
//   2. if (eax == 0)  -> return 0 (null pointer value), popping the single arg
//   3. otherwise      -> tail-jump into textObj->vtbl[0xb8/4 = 46]
//                        with ecx = textObj and the single stack argument a2
//                        forwarded unchanged (the slot's callee pops the 4 bytes).
//
// The call target lives in an indirect vtable slot, so the tail jump is
// reproduced as a naked thunk that mirrors the original instructions.

static __declspec(naked) void CGuiButton_ForwardToTextObject()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x134]   // eax = self->m_pText
        test eax, eax
        je  no_text
        mov ecx, eax                       // ecx = text object (this for the virtual)
        mov eax, dword ptr [ecx]           // eax = text->vtbl
        jmp dword ptr [eax + 0xb8]         // tail-jump to vtbl slot 46 (ret 4)
    no_text:
        ret 4                              // return 0, pop the stack argument
    }
}

HOOK(0x725330, CGuiButton_ForwardToTextObject, "auto");
