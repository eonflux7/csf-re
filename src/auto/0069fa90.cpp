#include <cstdint>
#include "hook.h"

// 0x0069FA90: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void InvokeGlobalHandlerSlot4C()
{
    __asm {
        mov edx, dword ptr ds:[0x8bb404]
        jmp dword ptr [edx + 0x4c]
    }
}

HOOK(0x69FA90, InvokeGlobalHandlerSlot4C, "auto");
