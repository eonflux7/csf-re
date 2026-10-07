#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns field_0030 when field_002c is non-zero, otherwise 0.
static int __fastcall C_008655e8_GetValueOrZero(C_008655e8* self, void* /*edx*/)
{
    if (self->field_002c)
        return static_cast<int>(self->field_0030);
    return 0;
}

HOOK(0x00726C50, C_008655e8_GetValueOrZero, "auto");
