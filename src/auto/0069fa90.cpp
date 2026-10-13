#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0069FA90: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static const uintptr_t dispatch_global = CSF_ADDR_DATA(0x008BB404);

static __declspec(naked) void InvokeGlobalHandlerSlot4C()
{
    __asm {
        mov edx, dword ptr [dispatch_global]
        mov edx, dword ptr [edx]
        jmp dword ptr [edx + 0x4c]
    }
}

HOOK(0x69FA90, InvokeGlobalHandlerSlot4C, "auto");
