#include <cstdint>
#include "hook.h"

// 0x0069FA40: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void CallGlobalObjectVtblSlot0x2C()
{
    __asm {
        mov ecx, dword ptr ds:[0x8bb404]
        jmp dword ptr [ecx + 0x2c]
    }
}

HOOK(0x69FA40, CallGlobalObjectVtblSlot0x2C, "auto");
