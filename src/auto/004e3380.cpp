#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4E1300, 0x004E1300, (void* self, void* edx));

// Resets the C_0083e248 state fields to their defaults. The 5.0f value
// stored in field_0030 is the original's 0x40a00000 literal.
static int __fastcall C_0083e248_ResetState(C_0083e248* self, void* /*edx*/)
{
    sub_4E1300(self, nullptr);
    self->field_0018 = 0;
    self->field_0020 = 0.0f;
    self->field_0028 = 0.0f;
    self->field_002c = 0;
    self->field_002d = 0;
    self->field_0030 = 5.0f;
    self->field_0044 = 0;
    self->field_003c = 0;
    return 0;
}

HOOK(0x004E3380, C_0083e248_ResetState, "auto");
