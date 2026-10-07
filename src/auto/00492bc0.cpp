#include <cstdint>
#include "hook.h"

// 0x00492BC0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void CallInnerObjectVirtualMethod110()
{
    __asm {
        mov ecx, dword ptr [ecx + 0x44]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x110]
    }
}

HOOK(0x492BC0, CallInnerObjectVirtualMethod110, "auto");
