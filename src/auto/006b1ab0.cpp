#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Concurrency::details::EventWaitNode::Reset (0x006B1AB0)
// Loads the node's inner pointer at +4, then tail-jumps to that object's
// vtable slot 1 (byte offset 4), forwarding the caller's stack arguments.
static hook::Tail __fastcall EventWaitNode_ResetInner(void* self, const uint32_t* args) {
    (void)args;
    void* obj = CSF_AT(void*, self, 0x04);
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x04 / 4]);
}

HOOK_TAIL(0x6B1AB0, EventWaitNode_ResetInner, "game");
