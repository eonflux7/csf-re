#include <cstdint>
#include "globals.h"
#include "hook.h"

// Target: 0x00713D90 (22 bytes)
// __thiscall int GuiEffect_ForwardToChildEffect(C_008656d0* this, int a2)  -> ret 4
//
// Semantics (from disassembly):
//   1. eax = this->field_0008 (offset 0x08)
//   2. if (eax == 0) -> return eax (0), popping the 4-byte argument
//   3. otherwise     -> tail-jump into child->vtbl[0x68 / 4 = 26]
//                       with ecx = child; the caller's stack (a2) is forwarded
//                       untouched and the callee pops it.

static hook::Tail __fastcall GuiEffect_ForwardToChildEffect(void* self, const uint32_t* args)
{
    (void)args; // the stack argument is forwarded by the jump, not read here

    uint32_t child = CSF_AT(uint32_t, self, 0x08);
    if (child == 0)
        return hook::tail_ret(0u, 4);

    void* obj = reinterpret_cast<void*>(child);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x68 / 4]);
}

HOOK_TAIL(0x713D90, GuiEffect_ForwardToChildEffect, "gui_effects");
