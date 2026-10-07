#include <cstdint>
#include "hook.h"

// 0x004F5190: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void InvokeInnerObjectVirtual28()
{
    __asm {
        mov ecx, dword ptr [ecx + 4]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x28]
    }
}

HOOK(0x4F5190, InvokeInnerObjectVirtual28, "auto");
