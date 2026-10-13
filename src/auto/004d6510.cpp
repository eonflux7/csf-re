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

// CharTask_ResetState: resets the base TCharTask state (bound by address).
CSF_FUNC(int, __fastcall, CharTask_ResetState, 0x004D1700, (C_0083b958* self, void*));

// Clears the per-task flag at +0x40 after resetting the base task state.
static int __fastcall C_0083c388_ResetTaskFlag(C_0083c388* self, void* /*edx*/)
{
    int result = CharTask_ResetState(self, nullptr);
    self->field_0040 = 0;
    return result;
}

HOOK(0x004D6510, C_0083c388_ResetTaskFlag, "auto");
