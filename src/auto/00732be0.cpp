#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008670f0_SetValue(C_008670f0* self, void* /*edx*/, int value)
{
    if (self->field_00b8 != static_cast<uint32_t>(value))
    {
        self->field_0010 = 1;
        self->field_00b8 = static_cast<uint32_t>(value);
    }
    return value;
}

HOOK(0x00732BE0, C_008670f0_SetValue, "auto");
