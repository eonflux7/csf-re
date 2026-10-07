#include <cstdint>
#include "hook.h"

// 0x005266B0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void InvokeField0040VirtualMethod()
{
    __asm {
        mov ecx, dword ptr [ecx + 0x40]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x38]
    }
}

HOOK(0x5266B0, InvokeField0040VirtualMethod, "auto");
