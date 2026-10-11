#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00833908_SetStoredValue(C_00833908* self, void* /*edx*/, int value)
{
    self->field_00bc = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x48EDB0, C_00833908_SetStoredValue, "auto");
