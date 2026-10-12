#include <cstdint>
#include "globals.h"
#include "hook.h"

// Target: 0x006B1640 (25 bytes)
// __thiscall int Object_ForwardArgsToSubobject35(int* this, int a2)  -> ret 4
//
// Semantics (from disassembly):
//   1. eax = this->field_008C (offset 0x8C, slot 35)
//   2. if (eax == 0) -> return eax (0), popping the 4-byte argument
//   3. otherwise     -> tail-jump into sub->vtbl[0x18 / 4 = 6]
//                       with ecx = sub; the caller's stack (a2) is forwarded
//                       untouched and the callee pops it.

static hook::Tail __fastcall Object_ForwardArgsToSubobject35(void* self, const uint32_t* args)
{
    (void)args; // the stack argument is forwarded by the jump, not read here

    uint32_t sub = CSF_AT(uint32_t, self, 0x8C);
    if (sub == 0)
        return hook::tail_ret(0u, 4);

    void* obj = reinterpret_cast<void*>(sub);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x18 / 4]);
}

HOOK_TAIL(0x6B1640, Object_ForwardArgsToSubobject35, "auto");
