#include <cstdint>
#include "hook.h"

// 0x00510470: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_008406e8_InvokeVtblSlot2()
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 8]
    }
}

HOOK(0x510470, C_008406e8_InvokeVtblSlot2, "auto");
