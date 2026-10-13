#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0069FB30: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static const uintptr_t dispatch_global = CSF_ADDR_DATA(0x008BB404);

static __declspec(naked) void InvokeDispatchHandler48()
{
    __asm {
        mov ecx, dword ptr [dispatch_global]
        mov ecx, dword ptr [ecx]
        jmp dword ptr [ecx + 0x48]
    }
}

HOOK(0x69FB30, InvokeDispatchHandler48, "auto");
