#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_0082e128_HasAnyActiveFlag(C_0082e128* self, void* /*edx*/)
{
    if (self->field_01fc != 0)
        return 1;
    if (self->field_0077 != 0)
        return 1;
    return 0;
}

HOOK(0x00417D80, C_0082e128_HasAnyActiveFlag, "auto");
