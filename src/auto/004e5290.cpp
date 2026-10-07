#include <cstdint>
#include "hook.h"

// 0x004E5290: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_0083e9d0_InvokeVtblB0()
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0xb0]
    }
}

HOOK(0x4E5290, C_0083e9d0_InvokeVtblB0, "auto");
