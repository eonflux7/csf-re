#include "globals.h"
#include "hook.h"

static __declspec(naked) void C_00495140_ForwardInnerObjectVirtualCall()
{
    __asm
    {
        mov ecx, dword ptr [ecx + 0x16C]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x3C]
    }
}

HOOK(0x00495140, C_00495140_ForwardInnerObjectVirtualCall, "auto");
