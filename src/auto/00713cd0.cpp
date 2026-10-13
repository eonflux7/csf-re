#include <cstdint>
#include "globals.h"
#include "hook.h"

// Target: 0x00713CD0 (22 bytes)
// __thiscall int GuiEffect_ForwardToInnerObject(C_00863f60* this, int a2, int a3)  -> ret 8
//
// Semantics (from disassembly):
//   1. eax = this->field_0008 (offset 0x08)
//   2. if (eax == 0) -> return eax (0), popping the 8 bytes of arguments
//   3. otherwise     -> tail-jump into inner->vtbl[0x44 / 4 = 17]
//                       with ecx = inner; the caller's stack (a2, a3) is forwarded
//                       untouched and the callee pops it.

static hook::Tail __fastcall GuiEffect_ForwardToInnerObject(void* self, const uint32_t* args)
{
    (void)args; // the stack arguments are forwarded by the jump, not read here

    uint32_t inner = CSF_AT(uint32_t, self, 0x08);
    if (inner == 0)
        return hook::tail_ret(0u, 8);

    void* obj = reinterpret_cast<void*>(inner);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x44 / 4]);
}

HOOK_TAIL(0x713CD0, GuiEffect_ForwardToInnerObject, "gui_effects");
