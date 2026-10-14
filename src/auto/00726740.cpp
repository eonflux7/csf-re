#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

// Original 00726740 (thiscall, ret 0xc): first `call 0x7264c0` with (a3, a4) on the stack and ecx = this, edx as they
// came in (the helper's return value is ignored); then eax = a2, ecx = [0x8b68c0], pushes a4, a3, a2 and calls
// [ecx+0x30] (cdecl: the caller pops, `add esp, 0xc`), returning the callee's eax with `ret 0xc`.
// The indirect callee sees ecx = the singleton, which a C++ function-pointer call cannot set, so both calls go
// through hook::call_regs on the registers as they came in (HOOK_REGS).
// args[0..2] = a2..a4 (the caller's stack arguments, read only).
static uint32_t __cdecl C_008655e8_InvokeCallback(hook::Regs* regs, const uint32_t* args) {
    const uint32_t preArgs[2] = {args[1], args[2]};  // a3, a4
    hook::call_regs(CSF_ADDR_FN(0x007264C0), regs, preArgs, 2);

    void* engine = g_pRwEngineInstance;
    const uintptr_t callback = CSF_AT(uintptr_t, engine, 0x30);
    regs->eax = args[0];
    regs->ecx = reinterpret_cast<uint32_t>(engine);
    const uint32_t stack[3] = {args[0], args[1], args[2]};
    return hook::call_regs(callback, regs, stack, 3);
}

HOOK_REGS(0x00726740, C_008655e8_InvokeCallback, 12, "auto");
