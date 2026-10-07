#include <cstdint>
#include "hook.h"

// Target: 0x00718860 (25 bytes)
// __thiscall int sub_718860(CGuiControl* self, int a2)   -> ret 4
//
// Semantics (from disassembly):
//   1. eax = self->m_pEffects (offset 0x128)
//   2. if (eax == 0) -> return 0 (the null pointer value, eax already 0)
//   3. otherwise     -> ecx = effect object, eax = effect->vtbl,
//                       tail-jump to effect->vtbl[0x74/4 = 29] forwarding a2.
//                       The virtual callee pops the 4 argument bytes (ret 4).
//
// The call target lives in an indirect vtable slot, so the tail jump is
// reproduced as a naked thunk matching the original instructions exactly.

static __declspec(naked) void CGuiControl_ForwardArgToFirstEffectVtblSlot29()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x128]   // eax = self->m_pEffects
        test eax, eax
        je  no_effect
        mov ecx, eax                       // ecx = effect object (this)
        mov eax, dword ptr [ecx]           // eax = effect->vtbl
        jmp dword ptr [eax + 0x74]         // tail-jump to vtbl slot 29 (ret 4)
    no_effect:
        ret 4                              // return 0 (eax == 0), pop the argument
    }
}

HOOK(0x718860, CGuiControl_ForwardArgToFirstEffectVtblSlot29, "auto");
