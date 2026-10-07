#include <cstdint>
#include "hook.h"

// 0x004183B0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void InvokeInnerVtbl34()
{
    __asm {
        mov ecx, dword ptr [ecx + 0x10]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x34]
    }
}

HOOK(0x4183B0, InvokeInnerVtbl34, "auto");
