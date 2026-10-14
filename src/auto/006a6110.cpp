#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stored callback: a function pointer at +0x24 and its user-data argument at +0x28.
// The original loads ecx = [a2+0x28] and eax = a2, then pushes userData and a1 and calls [a2+0x24] (cdecl, the
// caller pops). The callee sees ecx = userData at entry, so the call is made through hook::call_regs with the
// registers of entry: ecx is set here, every other register is the one the original's callee would get.
// args[0] = a1, args[1] = a2 (the caller's stack arguments, read only).
static uint32_t __cdecl InvokeStoredCallback(hook::Regs* regs, const uint32_t* args) {
    void* context = reinterpret_cast<void*>(static_cast<uintptr_t>(args[1]));
    const uint32_t callback = CSF_AT(uint32_t, context, 0x24);
    const uint32_t userData = CSF_AT(uint32_t, context, 0x28);

    regs->eax = args[1];
    regs->ecx = userData;
    const uint32_t stack[2] = {userData, args[0]};
    return hook::call_regs(callback, regs, stack, 2);
}

HOOK_REGS(0x6A6110, InvokeStoredCallback, 0, "auto");
