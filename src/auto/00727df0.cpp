#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00727DF0: stores the byte argument into this->field_007d and returns it.
static char __fastcall C_008655e8_SetField007d(C_008655e8* self, void* /*edx*/, char value)
{
    self->field_007d = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x00727DF0, C_008655e8_SetField007d, "auto");
