#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Forwards the call to the inner task object stored at +0x10, if any.
// Otherwise returns 0 (the original's eax holds the null pointer).
static hook::Tail __fastcall C_0082fbe8_ForwardToInnerTask(void* self, const uint32_t* args)
{
    (void)args;  // The original forwards the caller's stack args untouched via the jump.

    uint32_t innerTask = CSF_AT(uint32_t, self, 0x10);
    if (innerTask != 0) {
        void* obj = reinterpret_cast<void*>(innerTask);
        return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x18 / 4]);
    }
    return hook::tail_ret(0);
}

HOOK_TAIL(0x0040FC40, C_0082fbe8_ForwardToInnerTask, "auto");
