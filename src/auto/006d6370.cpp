#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00861120_SetStoredValue00F0(C_00861120* self, void* /*edx*/, int value)
{
    self->field_00f0 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x6D6370, C_00861120_SetStoredValue00F0, "auto");
