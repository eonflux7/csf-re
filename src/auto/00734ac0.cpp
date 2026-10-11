#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_008670f0_SetStoredValue(C_008670f0* self, void* /*edx*/, int value)
{
    self->field_0104 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x734AC0, C_008670f0_SetStoredValue, "auto");
