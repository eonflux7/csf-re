#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall IsStateCompatible(void* self, void* /*edx*/, int requestedState)
{
    const int state = CSF_AT(int, self, 0x78);
    return state == 2 || state == -1 || requestedState == state;
}

HOOK_KEEP_ECX(0x0048EF90, IsStateCompatible, 1, "auto");
