#include <cstdint>
#include "hook.h"

// 0x006EEB00: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_00862110_ForwardToEmbeddedObjectSlot5()
{
    __asm {
        add ecx, 8
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x14]
    }
}

HOOK(0x6EEB00, C_00862110_ForwardToEmbeddedObjectSlot5, "auto");
