#include <cstdint>
#include "hook.h"

// 0x0069A3B0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_0085e760_ForwardVirtualCallSlot3()
{
    __asm {
        mov ecx, dword ptr [ecx + 4]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0xc]
    }
}

HOOK(0x69A3B0, C_0085e760_ForwardVirtualCallSlot3, "auto");
