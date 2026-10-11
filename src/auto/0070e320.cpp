#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Stores the argument in the object's field at +0x10, sets the byte flag at +0x46, and returns the argument.
static int __fastcall C_008639b0_SetValueAndEnableFlag(C_008639b0* self, void* /*edx*/, int value) {
    self->field_0010 = static_cast<uint32_t>(value);
    self->field_0046 = 1;
    return value;
}

HOOK(0x0070E320, C_008639b0_SetValueAndEnableFlag, "auto");
