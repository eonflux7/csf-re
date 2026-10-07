#include <cstdint>
#include "hook.h"

// 0x0050DDD0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void CallInnerObjectAt90_SecondVirtual()
{
    __asm {
        mov ecx, dword ptr [ecx + 0x90]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 4]
    }
}

HOOK(0x50DDD0, CallInnerObjectAt90_SecondVirtual, "auto");
