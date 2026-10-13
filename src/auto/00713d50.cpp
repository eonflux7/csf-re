#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Target: 0x00713D50 (22 bytes)
// __thiscall int GuiEffect_ForwardToAttachedEffect(C_00863f60* this, int a2)  -> ret 4
//
// Semantics (from disassembly):
//   1. eax = this->field_0008 (offset 0x08)
//   2. if (eax == 0) -> return eax (0), popping the 4-byte argument
//   3. otherwise     -> tail-jump into child->vtbl[0x54 / 4 = 21]
//                       with ecx = child; the caller's stack (a2) is forwarded
//                       untouched and the callee pops it.

static hook::Tail __fastcall GuiEffect_ForwardToAttachedEffect(void* self, const uint32_t* args)
{
    (void)args; // the stack argument is forwarded by the jump, not read here

    C_00863f60* effect = reinterpret_cast<C_00863f60*>(self);
    uint32_t attached = effect->field_0008;
    if (attached == 0)
        return hook::tail_ret(0u, 4);

    void* obj = reinterpret_cast<void*>(attached);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x54 / 4]);
}

HOOK_TAIL(0x713D50, GuiEffect_ForwardToAttachedEffect, "gui_effects");
