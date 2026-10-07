#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x00728470: stores the given byte into this->field_004c and returns it.
static char __fastcall C_008657e0_SetField004c(C_008657e0* self, void* /*edx*/, char value)
{
    self->field_004c = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x00728470, C_008657e0_SetField004c, "auto");
