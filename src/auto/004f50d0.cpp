#include <cstdint>
#include "hook.h"

// 0x004F50D0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void InvokeInnerVirtualAt0x50()
{
    __asm {
        mov ecx, dword ptr [ecx + 4]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x50]
    }
}

HOOK(0x4F50D0, InvokeInnerVirtualAt0x50, "auto");
