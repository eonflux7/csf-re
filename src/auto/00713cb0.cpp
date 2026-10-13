#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall GuiEffect_ForwardToChildSlot38(void* self, const uint32_t* args)
{
    (void)args;
    auto* effect = reinterpret_cast<C_008656d0*>(self);
    uint32_t childAddress = effect->field_0008;
    if (childAddress != 0) {
        void* child = reinterpret_cast<void*>(childAddress);
        return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x38 / 4]);
    }
    return hook::tail_ret(0);
}

HOOK_TAIL(0x713CB0, GuiEffect_ForwardToChildSlot38, "gui_effects");
