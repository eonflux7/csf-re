#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static hook::Tail __fastcall ForwardToAttachedObject(
    void* self, const uint32_t* /*args*/)
{
    void* obj = CSF_AT(void*, self, 0x48);
    if (obj)
        return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x20 / 4]);

    return hook::tail_ret(0, 4);  // ret 4 on the path without a target
}

HOOK_TAIL(0x0048E5F0, ForwardToAttachedObject, "auto");
