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

// Resets the motion/state accumulator fields of a C_0082e6e8 task to zero.
// Original: 0x41FA50, __thiscall, ret 0. Always returns 0.
static int __fastcall C_0082e6e8_ResetMotionFields(C_0082e6e8* self, void* /*edx*/)
{
    self->field_0024 = 0.0f;
    self->field_0028 = 0.0f;
    self->field_002c = 0.0f;
    self->field_0030 = 0.0f;
    self->field_0034 = 0.0f;
    self->field_0038 = 0.0f;
    self->field_003c = 0;
    self->field_0040 = 0.0f;
    self->field_0044 = 0;
    return 0;
}

HOOK(0x0041FA50, C_0082e6e8_ResetMotionFields, "auto");
