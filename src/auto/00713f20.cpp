#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Target: 0x00713F20 (25 bytes)
// __thiscall int C_00863f60_Sub713F20(C_00863f60* this, int a2) -> ret 4
//
// Semantics (from disassembly):
//   1. eax = this->field_0008 (offset 0x08)
//   2. if (eax == 0) -> return eax (0), popping the 4-byte argument
//   3. otherwise     -> tail-jump into child->vtbl[0x80 / 4 = 32]
//                       with ecx = child; the caller's stack (a2) is forwarded
//                       untouched and the callee pops it.

static hook::Tail __fastcall GuiEffect_ForwardValueToChild(void* self, const uint32_t* args)
{
    (void)args; // the stack argument is forwarded by the jump, not read here

    uint32_t child = CSF_AT(uint32_t, self, 0x08);
    if (child == 0)
        return hook::tail_ret(0u, 4);

    void* obj = reinterpret_cast<void*>(child);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x80 / 4]);
}

HOOK_TAIL(0x713F20, GuiEffect_ForwardValueToChild, "gui_effects");
