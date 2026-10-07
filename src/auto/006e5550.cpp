#include <cstdint>
#include <cstddef>
#include "types/auto_classes.h"
#include "hook.h"

// 0x006E5550: stores the given byte into field_0019 and returns it.
static char __fastcall SetField19(C_00861ce0* self, void* /*edx*/, char value)
{
    self->field_0019 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x006E5550, SetField19, "auto");
