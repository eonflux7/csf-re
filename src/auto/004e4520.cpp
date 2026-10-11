#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores the argument in field_0008 and zeroes the four state fields
// at +0x18..+0x24. Returns 0. __thiscall with one stack arg, so ret 4.
static int __fastcall C_0083e7d8_SetFieldsAndClear(C_0083e7d8* self, void* /*edx*/, int a2) {
    self->field_0008 = static_cast<uint32_t>(a2);
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    return 0;
}

HOOK(0x004E4520, C_0083e7d8_SetFieldsAndClear, "auto");
