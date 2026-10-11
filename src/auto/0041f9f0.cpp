#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Resets the per-task state fields of C_0082e6e8 and reports success (0).
static int __fastcall C_0082e6e8_ResetTaskState(C_0082e6e8* self, void* /*edx*/) {
    self->field_0044 = 1;
    self->field_0040 = 0.0f;
    self->field_003c = 0;
    self->field_0048 = 0x3E4CCCCD; // IEEE-754 bit pattern of 0.2f
    return 0;
}

HOOK(0x41F9F0, C_0082e6e8_ResetTaskState, "auto");
