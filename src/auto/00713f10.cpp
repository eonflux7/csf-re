#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Forwards to the vtable slot +0x58 of the linked object stored at +0x08.
// When the link is null, returns 0 without popping anything.
static hook::Tail __fastcall GuiEffect_ForwardToLinkedObject(void* self, const uint32_t* /*args*/) {
    C_008656d0* effect = reinterpret_cast<C_008656d0*>(self);
    uint32_t linkedValue = effect->field_0008;
    if (linkedValue != 0) {
        void* linkedObject = reinterpret_cast<void*>(linkedValue);
        return hook::tail_jump(linkedObject, (*reinterpret_cast<void***>(linkedObject))[0x58 / 4]);
    }
    return hook::tail_ret(0);
}

HOOK_TAIL(0x00713F10, GuiEffect_ForwardToLinkedObject, "gui_effects");
