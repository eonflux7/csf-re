#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// 0x00713F90: stores the incoming value into field_002c and returns it unchanged.
static int __fastcall C_008656d0_SetField002C(C_008656d0* self, void* /*edx*/, int value)
{
    self->field_002c = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x00713F90, C_008656d0_SetField002C, "auto");
