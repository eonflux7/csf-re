#include <cstdint>
#include "hook.h"

// Target: 0x00718C00 (28 bytes)
// __thiscall int sub_718C00(CGuiControl* self, int a2)  -> ret 4
//
// Semantics (from the disassembly):
//   eax = self->m_pEffects (offset 0x128)
//   if (eax == 0)  -> return 0 (the null pointer value), popping the 4-byte arg
//   else           -> tail-jump into effects->vtbl[0x80 / 4 = 32] with ecx = effects,
//                     forwarding a2 unchanged; the callee pops the 4 argument bytes.
//
// The call target is an unknown indirect vtable slot, so the tail jump is
// reproduced as a naked thunk matching the original instructions exactly.

static __declspec(naked) void CGuiControl_ForwardEventToFirstEffect()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x128]   // eax = self->m_pEffects
        test eax, eax
        je  no_effects
        mov ecx, eax                       // ecx = effects (this for the virtual)
        mov eax, dword ptr [ecx]           // eax = effects->vtbl
        jmp dword ptr [eax + 0x80]         // tail-jump to vtbl slot 32 (ret 4)
    no_effects:
        ret 4                              // return 0, pop the stack argument
    }
}

HOOK(0x718C00, CGuiControl_ForwardEventToFirstEffect, "auto");
