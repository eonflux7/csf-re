#include <cstdint>
#include "hook.h"

// 0x00437080: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_00864288_ForwardToVirtualSlot57()
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0xe4]
    }
}

HOOK(0x437080, C_00864288_ForwardToVirtualSlot57, "auto");
