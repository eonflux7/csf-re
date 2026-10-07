#include <cstdint>
#include <cstddef>
#include "types/auto_classes.h"
#include "hook.h"

// Resets a few fields of this object: clears two counters (0x20, 0x50, 0x58)
// and a byte flag (0x74). Returns 0.
static int __fastcall C_0082faf8_ClearState(C_0082faf8* self, void* /*edx*/)
{
    self->field_0020 = 0;
    self->field_0050 = 0.0f;
    self->field_0058 = 0.0f;
    self->field_0074 = 0;
    return 0;
}

HOOK(0x00424070, C_0082faf8_ClearState, "auto");
