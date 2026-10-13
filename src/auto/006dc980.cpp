#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x006DC980: loads this->field_0094, calls its vtable slot 5 (+0x14)
// to get a sub-object, then tail-jumps to that sub-object's vtable slot 4 (+0x10)
// with the sub-object in ecx, forwarding the caller's stack arguments unchanged.
static hook::Tail __fastcall RenderEngine_ForwardTailViaField94(void* self, const uint32_t* /*args*/) {
    auto* engine = reinterpret_cast<C_00861508*>(self);

    void* holder = reinterpret_cast<void*>(engine->field_0094);

    using ReturnsPointerFn = void* (__fastcall*)(void* self, void* edx);
    auto getSubObject = reinterpret_cast<ReturnsPointerFn>(CSF_AT(void**, holder, 0)[0x14 / 4]);
    void* subObject = getSubObject(holder, nullptr);

    return hook::tail_jump(subObject, (*reinterpret_cast<void***>(subObject))[0x10 / 4]);
}

HOOK_TAIL(0x6DC980, RenderEngine_ForwardTailViaField94, "gfx_render_engine");
