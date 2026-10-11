#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008670f0_SetValue(C_008670f0* self, void* /*edx*/, int value)
{
    self->field_0108 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x00734AD0, C_008670f0_SetValue, "auto");
