#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall C_00833908_ForwardToAttachedObject(void* self, const uint32_t* /*args*/)
{
    void* obj = CSF_AT(void*, self, 0x34);
    if (!obj)
        return hook::tail_ret(0);

    return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0xC0 / 4]);
}

HOOK_TAIL(0x0048EA30, C_00833908_ForwardToAttachedObject, "auto");
