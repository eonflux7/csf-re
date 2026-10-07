#include <cstdint>
#include "hook.h"

// 0x006FD170: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void CallInnerObjectVtblSlot4()
{
    __asm {
        add ecx, 0x10
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x10]
    }
}

HOOK(0x6FD170, CallInnerObjectVtblSlot4, "auto");
