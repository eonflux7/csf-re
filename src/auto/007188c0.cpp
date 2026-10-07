#include "hook.h"

// Forwards the call to the effect object's virtual method at vtable offset +0x38.
// If no effect exists (m_pEffects == 0) the function returns 0.
static __declspec(naked) void CGuiControl_ForwardToEffectSlot38()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x128]
        test eax, eax
        jne has_effect
        mov eax, 0
        ret
    has_effect:
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x38]
    }
}

HOOK(0x7188C0, CGuiControl_ForwardToEffectSlot38, "auto");
