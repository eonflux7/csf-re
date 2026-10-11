#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static hook::Tail __fastcall C_0082f9f8_TryAcquire(void* self, const uint32_t* /*args*/)
{
    void* delegate = CSF_AT(void*, self, 0x48);
    if (delegate)
        return hook::tail_jump(delegate, (*reinterpret_cast<void***>(delegate))[0x1C / 4]);

    return hook::tail_ret(1, 0xC);  // mov al, 1; ret 0xC
}

HOOK_TAIL(0x0048E5D0, C_0082f9f8_TryAcquire, "auto");

