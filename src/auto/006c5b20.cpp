#include <cstdint>
#include "globals.h"
#include "hook.h"

// Pure tail-jump thunk: push ebp; mov ebp, esp; pop ebp; jmp dword ptr [0x8BB4B4].
// The jmp forwards the caller's stack arguments and registers untouched (the target's signature is unknown),
// so this must stay a jmp, not a call.
static const uintptr_t& dispatch_target = *reinterpret_cast<const uintptr_t*>(CSF_ADDR_DATA(0x008BB4B4));

static __declspec(naked) void InvokeTargetFunction()
{
    __asm { jmp dword ptr [dispatch_target] }
}

HOOK(0x6C5B20, InvokeTargetFunction, "auto");
