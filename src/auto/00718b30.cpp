#include <cstdint>
#include "hook.h"

// Target: 0x00718B30 (25 bytes)
// __thiscall int sub_718B30(CGuiControl* self, int a2)   -> ret 4
//
// Semantics (from disassembly):
//   1. eax = self->m_pEffects (offset 0x128)
//   2. if (eax == 0)  ->  return 0 (the null pointer value), popping the stack arg
//   3. otherwise      -> tail-jump into obj->vtbl[21] (offset 0x54), with
//                        ecx = obj and the single stack argument a2 untouched.
//                        The vtable callee pops the 4 argument bytes (ret 4).
//
// The effect object type is unknown and the call target lives only in an
// indirect vtable slot, so the tail jump is reproduced as a naked thunk that
// matches the original instructions exactly.

static __declspec(naked) void CGuiControl_ForwardToEffectsVtblSlot21()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x128]  // eax = self->m_pEffects
        test eax, eax
        je  no_effect
        mov ecx, eax                      // ecx = effect object (this for the virtual)
        mov eax, dword ptr [ecx]          // eax = effect object's vtbl
        jmp dword ptr [eax + 0x54]        // tail-jump to vtbl slot 21 (ret 4)
    no_effect:
        ret 4                             // return 0, pop the stack argument
    }
}

HOOK(0x718B30, CGuiControl_ForwardToEffectsVtblSlot21, "auto");
