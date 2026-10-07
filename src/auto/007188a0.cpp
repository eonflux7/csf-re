#include "hook.h"

// Tail-forwards to m_pEffects' virtual method at vtable offset +0x60 (slot 24) when an
// effects object is attached; otherwise yields 0. No stack args are consumed (ret 0).
static __declspec(naked) void CGuiControl_ForwardToEffectSlot60()
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
        jmp dword ptr [eax + 0x60]
    }
}

HOOK(0x7188A0, CGuiControl_ForwardToEffectSlot60, "auto");
