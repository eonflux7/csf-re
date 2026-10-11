#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static hook::Tail __fastcall ForwardToAttachedObject(
    void* self, const uint32_t* /*args*/, uint32_t /*argument*/)
{
    void* obj = CSF_AT(void*, self, 0x48);
    if (obj)
        return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x20 / 4]);

    return hook::tail_ret(0);
}

HOOK_TAIL(0x0048E5F0, ForwardToAttachedObject, "auto");
