#include <cstdint>
#include "hook.h"

// 0x0051F7A0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void InvokeMember90VirtualSlot5()
{
    __asm {
        mov ecx, dword ptr [ecx + 0x90]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x14]
    }
}

HOOK(0x51F7A0, InvokeMember90VirtualSlot5, "auto");
