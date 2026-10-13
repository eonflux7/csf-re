#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall CErrorLog_ForwardToChildSinkSlot1C(void* self, const uint32_t* args)
{
    (void)args;
    auto* owner = reinterpret_cast<C_0085ea2c*>(self);
    uint32_t childAddress = owner->field_0004;
    if (childAddress != 0) {
        void* child = reinterpret_cast<void*>(childAddress);
        return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x1C / 4]);
    }
    return hook::tail_ret(0);
}

HOOK_TAIL(0x69D5F0, CErrorLog_ForwardToChildSinkSlot1C, "core_error_log");
