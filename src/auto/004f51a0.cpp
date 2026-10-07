#include <cstdint>
#include "hook.h"

// 0x004F51A0: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void C_Unknown_InvokeSubobjectVfunc2C()
{
    __asm {
        mov ecx, dword ptr [ecx + 4]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x2c]
    }
}

HOOK(0x4F51A0, C_Unknown_InvokeSubobjectVfunc2C, "auto");
