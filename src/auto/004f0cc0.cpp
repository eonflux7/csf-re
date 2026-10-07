#include <cstdint>
#include "hook.h"

// 0x004F0CC0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_0083ef30_ForwardVtbl8C()
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x8c]
    }
}

HOOK(0x4F0CC0, C_0083ef30_ForwardVtbl8C, "auto");
