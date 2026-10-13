#include <cstdint>
#include "globals.h"
#include "hook.h"

// Target: 0x004DB310 (22 bytes)
// __thiscall int TCharTask_ForwardToChildVirtual(TCharTask* this, int a2) -> ret 4
//
// Semantics (from disassembly):
//   1. eax = this->field_0010 (offset 0x10)
//   2. if (eax == 0) -> return eax (0), popping the 4-byte argument
//   3. otherwise     -> tail-jump into child->vtbl[0x50 / 4 = 20]
//                       with ecx = child; the caller's stack (a2) is forwarded
//                       untouched and the callee pops it.

static hook::Tail __fastcall TCharTask_ForwardToChildVirtual(void* self, const uint32_t* args)
{
    (void)args; // the stack argument is forwarded by the jump, not read here

    uint32_t child = CSF_AT(uint32_t, self, 0x10);
    if (child == 0)
        return hook::tail_ret(0u, 4);

    void* obj = reinterpret_cast<void*>(child);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x50 / 4]);
}

HOOK_TAIL(0x4DB310, TCharTask_ForwardToChildVirtual, "ai_char_task_forwarders");
