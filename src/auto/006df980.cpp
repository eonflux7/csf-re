#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x006DF980: stores the argument into field_0060 and returns it.
static int __fastcall C_00861988_SetField0060(C_00861988* self, void* /*edx*/, int value)
{
    self->field_0060 = value;
    return value;
}

HOOK(0x006DF980, C_00861988_SetField0060, "auto");
