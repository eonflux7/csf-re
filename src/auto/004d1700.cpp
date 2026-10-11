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

// Resets the per-task state block of a C_0083b958 (TCharTask-derived) object
// to its initial values and returns -1.
static int __fastcall CharTask_ResetState(C_0083b958* self, void* /*edx*/)
{
    self->field_0038 = 1;
    self->field_0039 = 1;
    self->field_003a = 0;
    self->field_0018 = 2;
    self->field_001c = 0;
    self->field_0030 = static_cast<uint32_t>(-1);
    self->field_0034 = static_cast<uint32_t>(-1);
    return -1;
}

HOOK(0x004D1700, CharTask_ResetState, "auto");
