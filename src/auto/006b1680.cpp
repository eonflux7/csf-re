#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns whether the gate object held at +0x8C is open. A null gate is
// treated as open; otherwise the query is forwarded to the object's
// vtable slot 4 (offset 0x10) with the same stack arguments.
static hook::Tail __fastcall IsGlobalIntGateOpen(void* self, const uint32_t* args) {
    (void)args;
    void* gateObject = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x8C));
    if (gateObject == nullptr) {
        return hook::tail_ret(1);
    }
    return hook::tail_jump(gateObject, (*reinterpret_cast<void***>(gateObject))[0x10 / 4]);
}

HOOK_TAIL(0x006B1680, IsGlobalIntGateOpen, "auto");
