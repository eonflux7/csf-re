#include <cstdint>
#include "hook.h"

// 0x0044D9A0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_0044d9a0_InvokeMemberVirtual()
{
    __asm {
        mov ecx, dword ptr [ecx + 0x1c]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1c]
    }
}

HOOK(0x44D9A0, C_0044d9a0_InvokeMemberVirtual, "auto");
