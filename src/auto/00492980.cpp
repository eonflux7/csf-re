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

// Returns 1 when the object's state code (field +0x80) is one of
// 1, 2, 3, 4, 5, 6 or 23; otherwise 0. No calls, no side effects.
static int __fastcall C_00833908_IsStateCodeInActiveSet(C_00833908* self, void* /*edx*/)
{
    uint32_t stateCode = self->field_0080;
    if (stateCode == 1 || stateCode == 2 || stateCode == 3 || stateCode == 4 ||
        stateCode == 23 || stateCode == 5 || stateCode == 6)
    {
        return 1;
    }
    return 0;
}

HOOK(0x492980, C_00833908_IsStateCodeInActiveSet, "auto");
