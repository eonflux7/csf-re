#include "hook.h"

static __declspec(naked) void CGuiControl_ForwardToEffectSlot70()
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
        jmp dword ptr [eax + 0x70]
    }
}

HOOK(0x718840, CGuiControl_ForwardToEffectSlot70, "auto");
