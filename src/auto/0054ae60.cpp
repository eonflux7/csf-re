#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall C_008442a0_ForwardOperationAt64(
    void* self, const uint32_t* /*args*/)
{
    auto* instance = static_cast<C_008442a0*>(self);
    void* obj = reinterpret_cast<void*>(instance->field_00a4);
    if (obj)
        return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x64 / 4]);

    return hook::tail_ret(1, 8);
}

HOOK_TAIL(0x0054AE60, C_008442a0_ForwardOperationAt64, "type_registry");
