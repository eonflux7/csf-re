#include <cstdint>
#include "globals.h"
#include "hook.h"

// Forwards a virtual call (slot 14, byte offset 0x38) to the child task stored at +0x10.
// Child task at +0x10 is null: the original returns 0 without jumping.
static hook::Tail __fastcall TCharTask_TailForwardToSubtask(void* self, const uint32_t* args) {
    (void)args;  // The stack arguments are forwarded untouched by the jump; this function does not read them.

    uint32_t childTaskValue = CSF_AT(uint32_t, self, 0x10);
    if (childTaskValue == 0) {
        return hook::tail_ret(0);
    }

    void* childTask = reinterpret_cast<void*>(childTaskValue);
    return hook::tail_jump(childTask, (*reinterpret_cast<void***>(childTask))[0x38 / 4]);
}

HOOK_TAIL(0x004DB2E0, TCharTask_TailForwardToSubtask, "ai_char_task_forwarders");
