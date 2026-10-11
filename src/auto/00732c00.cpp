#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008670f0_SetSecondaryValue(C_008670f0* self, void* /*edx*/, int value)
{
    if (self->field_00bc != static_cast<uint32_t>(value))
    {
        self->field_0010 = 1;
        self->field_00bc = static_cast<uint32_t>(value);
    }
    return value;
}

HOOK(0x00732C00, C_008670f0_SetSecondaryValue, "auto");
