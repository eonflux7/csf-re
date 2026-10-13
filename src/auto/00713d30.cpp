#include <cstdint>
#include "globals.h"
#include "hook.h"

// Target: 0x00713D30 (22 bytes)
// __thiscall int C_00863f60_SetNamedByteValue(C_00863f60* this, int a2, int a3)  -> ret 8
//
// Semantics (from disassembly):
//   1. eax = this->field_0008 (offset 0x08)
//   2. if (eax == 0) -> return eax (0), popping the 8 bytes of stack arguments
//   3. otherwise     -> tail-jump into child->vtbl[0x50 / 4 = 20]
//                       with ecx = child; the caller's stack (a2, a3) is forwarded
//                       untouched and the callee pops it.

static hook::Tail __fastcall C_00863f60_ForwardSetNamedByteValueToChild(void* self, const uint32_t* args)
{
    (void)args; // the stack arguments are forwarded by the jump, not read here

    uint32_t child = CSF_AT(uint32_t, self, 0x08);
    if (child == 0)
        return hook::tail_ret(0u, 8);

    void* obj = reinterpret_cast<void*>(child);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x50 / 4]);
}

HOOK_TAIL(0x713D30, C_00863f60_ForwardSetNamedByteValueToChild, "gui_effects");
