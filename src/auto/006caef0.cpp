#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00860bb8_SetStoredValue(C_00860bb8* self, void* /*edx*/, int value)
{
    self->field_00b0 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x6CAEF0, C_00860bb8_SetStoredValue, "auto");
