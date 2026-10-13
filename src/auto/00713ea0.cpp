#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"

// Forwards the call to the child object stored at +0x08 through its vtable slot 0x2C.
// If there is no child, returns 0 with a plain ret.
static hook::Tail __fastcall C_00865870_ResetBaseForward(void* self, const uint32_t* /*args*/) {
    C_008657e0* owner = reinterpret_cast<C_008657e0*>(self);
    uint32_t childAddress = owner->field_0008;
    if (childAddress != 0) {
        void* child = reinterpret_cast<void*>(childAddress);
        return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x2C / 4]);
    }
    return hook::tail_ret(0);
}

HOOK_TAIL(0x713EA0, C_00865870_ResetBaseForward, "gui_effects");
