#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Gets the child object from virtual slot 0x3C of `this` (same class/offset as the
// original's `call [eax+0x3c]`, which takes no stack args), then tail-jumps to the
// child's virtual slot 0x1C with the child as `this`. If the child is null, returns 0.
static hook::Tail __fastcall C_0082dcf0_ForwardToChildSlot1C(void* self, const uint32_t* args) {
    (void)args;
    using SubobjectGetter = void* (__fastcall*)(void* self, void* edx);
    SubobjectGetter getChild = reinterpret_cast<SubobjectGetter>(CSF_AT(void**, self, 0)[0x3C / 4]);
    void* child = getChild(self, nullptr);
    if (child == nullptr) {
        return hook::tail_ret(0);
    }
    return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x1C / 4]);
}

HOOK_TAIL(0x41E0D0, C_0082dcf0_ForwardToChildSlot1C, "auto");
