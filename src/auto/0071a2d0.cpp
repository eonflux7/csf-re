#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Reads the byte flag at +0x98, clears it, and returns the previous value.
static char __fastcall C_00864508_ConsumeFlag98(C_00864508* self, void* /*edx*/)
{
    char old_value = static_cast<char>(self->field_0098);
    self->field_0098 = 0;
    return old_value;
}

HOOK(0x0071A2D0, C_00864508_ConsumeFlag98, "auto");
