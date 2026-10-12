#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"

// Forwards a call to the object stored at +0x8C via its vtable slot 5 (+0x14).
// Returns 0 when the pointer at +0x8C is null.
static hook::Tail __fastcall ForwardToChildSlot5At8C(void* self, const uint32_t* args) {
    (void)args;
    void* child = CSF_AT(void*, self, 0x8C);
    if (child == nullptr) {
        return hook::tail_ret(0);
    }
    void* obj = child;
    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x14 / 4]);
}

HOOK_TAIL(0x6B1590, ForwardToChildSlot5At8C, "game");
