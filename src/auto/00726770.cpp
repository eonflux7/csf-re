#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

// Original 00726770 (stdcall, ret 0x14): eax = a2, ecx = a1, edx = [0x8b68c0]; pushes a5, a4, a3, a2, a1 and calls
// [edx+0x34] (cdecl: the caller pops, `add esp, 0x14`), then returns the callee's eax with `ret 0x14`.
// The callee sees ecx = a1 at the call, which a C++ function-pointer call cannot set, so the call goes through
// hook::call_regs; HOOK_REGS keeps the other registers as they came in and drops the stack words after the call.
// args[0..4] = a1..a5 (the caller's stack arguments, read only).
static uint32_t __cdecl ForwardToCdeclHandler(hook::Regs* regs, const uint32_t* args) {
    void* engine = g_pRwEngineInstance;
    const uintptr_t handler = CSF_AT(uintptr_t, engine, 0x34);

    regs->eax = args[1];
    regs->ecx = args[0];
    regs->edx = reinterpret_cast<uint32_t>(engine);
    const uint32_t stack[5] = {args[0], args[1], args[2], args[3], args[4]};
    return hook::call_regs(handler, regs, stack, 5);
}

HOOK_REGS(0x00726770, ForwardToCdeclHandler, 20, "auto");
