#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Forwards a 2-argument call (stdcall-style, caller pops 8) to the object held
// in field_00a4 through its vtable slot 0x48. If no delegate is set, returns 0
// and pops the 8 argument bytes.
static hook::Tail __fastcall C_008442a0_ForwardToDelegate(void* self, const uint32_t* /*args*/)
{
    auto* instance = static_cast<C_008442a0*>(self);
    void* delegate = reinterpret_cast<void*>(instance->field_00a4);
    if (!delegate)
        return hook::tail_ret(0, 8);

    return hook::tail_jump(delegate, (*reinterpret_cast<void***>(delegate))[0x48 / 4]);
}

HOOK_TAIL(0x0054AD20, C_008442a0_ForwardToDelegate, "type_registry");
