#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall GuiEffect_ForwardToChildSlot40(void* self, const uint32_t* args)
{
    (void)args;
    auto* owner = reinterpret_cast<C_00865758*>(self);
    uint32_t childAddress = owner->field_0008;
    if (childAddress != 0) {
        void* child = reinterpret_cast<void*>(childAddress);
        return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x40 / 4]);
    }
    return hook::tail_ret(0);
}

HOOK_TAIL(0x713ED0, GuiEffect_ForwardToChildSlot40, "gui_effects");
