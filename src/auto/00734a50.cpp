#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall C_008670f0_SetByteFlag(C_008670f0* self, void* /*edx*/, char value)
{
    self->field_00fe = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x734A50, C_008670f0_SetByteFlag, "auto");
