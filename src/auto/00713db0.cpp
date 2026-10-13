#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall GuiEffect_ForwardToChildSlot60(void* self, const uint32_t* args)
{
    (void)args;
    auto* owner = reinterpret_cast<C_008656d0*>(self);
    uint32_t childAddress = owner->field_0008;
    if (childAddress != 0) {
        void* child = reinterpret_cast<void*>(childAddress);
        return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x60 / 4]);
    }
    return hook::tail_ret(0);
}

HOOK_TAIL(0x713DB0, GuiEffect_ForwardToChildSlot60, "gui_effects");
