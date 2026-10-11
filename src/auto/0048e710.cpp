#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static hook::Tail __fastcall ForwardToDelegate(
    void* self, const uint32_t* /*args*/, uint32_t /*argument*/)
{
    void* delegate = CSF_AT(void*, self, 0x34);
    if (delegate)
        return hook::tail_jump(delegate, (*reinterpret_cast<void***>(delegate))[0xB8 / 4]);

    return hook::tail_ret(0);
}

HOOK_TAIL(0x0048E710, ForwardToDelegate, "auto");
