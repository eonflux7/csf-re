#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Setter: stores the given value into this->field_0008 and returns it.
static int __fastcall C_0083e500_SetField0008(C_0083e500* self, void* /*edx*/, int value)
{
    self->field_0008 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x00713B10, C_0083e500_SetField0008, "auto");
