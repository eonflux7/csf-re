#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall C_008442a0_ForwardToDelegate(
    void* self, const uint32_t* /*args*/)
{
    auto* instance = static_cast<C_008442a0*>(self);
    void* delegate = reinterpret_cast<void*>(instance->field_00a4);
    if (delegate)
        return hook::tail_jump(
            delegate, (*reinterpret_cast<void***>(delegate))[0x70 / 4]);

    return hook::tail_ret(1, 12);
}

HOOK_TAIL(0x0054ADD0, C_008442a0_ForwardToDelegate, "type_registry");
