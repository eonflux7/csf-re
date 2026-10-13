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

// Clears the "active" bit (bit 0) of the task's flags word and moves the
// state from 5 to 4 when the task is in state 5. Does nothing otherwise.
static void __fastcall C_00835228_ResetStateFiveToFour(C_00835228* self, void* /*edx*/)
{
    if (self->field_0070 == 5)
    {
        self->field_0018 &= ~1u;
        self->field_0070 = 4;
    }
}

HOOK(0x004A8F20, C_00835228_ResetStateFiveToFour, "auto");
