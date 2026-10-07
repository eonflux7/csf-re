#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x0072A270: stores the given byte into this->field_0064 and returns it.
static char __fastcall C_00865aa8_SetField_0064(C_00865aa8* self, void* /*edx*/, char value)
{
    self->field_0064 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x0072A270, C_00865aa8_SetField_0064, "auto");
