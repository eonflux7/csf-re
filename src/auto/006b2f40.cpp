#include <cstdint>
#include "hook.h"

// 0x006B2F40: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_00860288_DispatchSubobjectVirtual70()
{
    __asm {
        add ecx, 0x30
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x70]
    }
}

HOOK(0x6B2F40, C_00860288_DispatchSubobjectVirtual70, "auto");
