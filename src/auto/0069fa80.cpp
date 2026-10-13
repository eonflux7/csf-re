#include <cstdint>
#include "globals.h"
#include "hook.h"

// 0x0069FA80: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static const uintptr_t dispatch_global = CSF_ADDR_DATA(0x008BB404);

static __declspec(naked) void DispatchGlobalTableEntry34()
{
    __asm {
        mov eax, dword ptr [dispatch_global]
        mov eax, dword ptr [eax]
        jmp dword ptr [eax + 0x34]
    }
}

HOOK(0x69FA80, DispatchGlobalTableEntry34, "auto");
