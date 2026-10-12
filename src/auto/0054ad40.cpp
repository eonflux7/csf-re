#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall C_008442a0_ForwardToDelegateSlot44(
    void* self, const uint32_t* /*args*/)
{
    auto* instance = static_cast<C_008442a0*>(self);
    void* delegate = reinterpret_cast<void*>(instance->field_00a4);
    if (!delegate)
        return hook::tail_ret(0, 8);

    return hook::tail_jump(delegate, (*reinterpret_cast<void***>(delegate))[0x44 / 4]);
}

HOOK_TAIL(0x0054AD40, C_008442a0_ForwardToDelegateSlot44, "type_registry");
