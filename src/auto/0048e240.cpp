#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall C_00833908_ForwardEnabledState(
    void* self, const uint32_t* /*args*/)
{
    auto* owner = static_cast<C_00833908*>(self);
    if (owner->vGetField016c() == 0)
        return hook::tail_ret(0, 4);  // eax is the getter's 0; ret 4

    void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(owner->vGetField016c()));
    return hook::tail_jump(target, (*reinterpret_cast<void***>(target))[0x3c / 4]);
}

HOOK_TAIL(0x0048E240, C_00833908_ForwardEnabledState, "auto");
