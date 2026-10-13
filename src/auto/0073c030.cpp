#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Reads the pointer at this+0xD4. If non-null, forwards to the vtable slot +0x0C
// of the sub-object at (pointer + 0x6B0), keeping the caller's stack arguments.
// Otherwise returns 0 with ret 4.
static hook::Tail __fastcall ForwardValueToSubobject(void* self, const uint32_t* args) {
    (void)args;
    uint32_t subobjectOwner = CSF_AT(uint32_t, self, 0xD4);
    if (subobjectOwner != 0) {
        void* subobject = reinterpret_cast<void*>(subobjectOwner + 0x6B0);
        return hook::tail_jump(subobject, (*reinterpret_cast<void***>(subobject))[0x0C / 4]);
    }
    return hook::tail_ret(0, 4);
}

HOOK_TAIL(0x73C030, ForwardValueToSubobject, "auto");
