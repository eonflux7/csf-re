#include <cstdint>
#include "hook.h"

// 0x00407F30: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_0082d5e0_DispatchVirtual17()
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x44]
    }
}

HOOK(0x407F30, C_0082d5e0_DispatchVirtual17, "auto");
