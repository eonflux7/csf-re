#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00861120_SetValue(C_00861120* self, void* /*edx*/, int value)
{
    self->field_00f8 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x734A40, C_00861120_SetValue, "auto");
