#include <cstdint>
#include "hook.h"

// 0x00526850: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_008410b0_InvokeSubobjectVirtual4()
{
    __asm {
        mov ecx, dword ptr [ecx + 0x40]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x10]
    }
}

HOOK(0x526850, C_008410b0_InvokeSubobjectVirtual4, "auto");
