#include <cstdint>
#include "hook.h"

// 0x006CA500: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void CallInnerVirtualSlot78()
{
    __asm {
        mov ecx, dword ptr [ecx + 4]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x78]
    }
}

HOOK(0x6CA500, CallInnerVirtualSlot78, "auto");
