#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Clears the 23 state fields of C_00840754 to zero, in the original's write order.
// Returns 0 (the original does `xor eax, eax` and `ret`).
static int __fastcall C_00840754_ResetFields(C_00840754* self, void* /*edx*/) {
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_006c = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_0070 = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0074 = 0;
    return 0;
}

HOOK(0x0050A0C0, C_00840754_ResetFields, "auto");
