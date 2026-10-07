#include <cstdint>
#include "hook.h"

// 0x006B2F30: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_00860288_ForwardToSubobjectVirtualAt88()
{
    __asm {
        mov eax, dword ptr [ecx + 0x30]
        add ecx, 0x30
        jmp dword ptr [eax + 0x88]
    }
}

HOOK(0x6B2F30, C_00860288_ForwardToSubobjectVirtualAt88, "auto");
