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

// Resets the three movement-task state words of the object: +0x4 = 1, +0xC = 0, +0x10 = 0.
// Returns 0. Original: __thiscall, one stack arg, ret 4.
static int __fastcall MoveTasks_ResetState(void* self, void* /*edx*/, int /*unusedArg*/)
{
    CSF_AT(int, self, 0x4) = 1;
    CSF_AT(int, self, 0xC) = 0;
    CSF_AT(int, self, 0x10) = 0;
    return 0;
}

HOOK(0x004ABAF0, MoveTasks_ResetState, "auto");
