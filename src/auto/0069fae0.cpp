#include <cstdint>
#include "hook.h"

// 0x0069FAE0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void InvokeTargetObjectHandler()
{
    __asm {
        mov ecx, dword ptr ds:[0x8bb404]
        jmp dword ptr [ecx + 0x44]
    }
}

HOOK(0x69FAE0, InvokeTargetObjectHandler, "auto");
