#include "hook.h"

static __declspec(naked) void CGuiStatic_InvokeTextOrReturnArg()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x134]
        test eax, eax
        jne has_text
        mov eax, dword ptr [esp + 4]
        ret 8
    has_text:
        mov ecx, dword ptr [ecx + 0x134]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0xc8]
    }
}

HOOK(0x720D10, CGuiStatic_InvokeTextOrReturnArg, "auto");
