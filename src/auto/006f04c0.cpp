#include <cstdint>
#include "hook.h"

// 0x006F04C0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_008622e8_InvokeInnerVirtualSlot7()
{
    __asm {
        mov ecx, dword ptr [ecx + 0x20]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1c]
    }
}

HOOK(0x6F04C0, C_008622e8_InvokeInnerVirtualSlot7, "auto");
