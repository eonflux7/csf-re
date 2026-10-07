#include <cstdint>
#include "hook.h"

// 0x006EECA0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void CallInnerObjectVtableSlot70()
{
    __asm {
        add ecx, 8
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x70]
    }
}

HOOK(0x6EECA0, CallInnerObjectVtableSlot70, "auto");
