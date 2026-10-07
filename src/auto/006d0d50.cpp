#include <cstdint>
#include "hook.h"

// 0x006D0D50: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_00860e68_InvokeInnerVirtual34()
{
    __asm {
        mov ecx, dword ptr [ecx + 0x2c]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x34]
    }
}

HOOK(0x6D0D50, C_00860e68_InvokeInnerVirtual34, "auto");
