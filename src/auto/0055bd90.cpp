#include <cstdint>
#include <cstddef>
#include "types/auto_classes.h"
#include "hook.h"

// 0x0055BD90: stores the byte argument into this->field_0018 and returns it unchanged.
static char __fastcall C_00867078_SetByteAt18(C_00867078* self, void* /*edx*/, char value)
{
    self->field_0018 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x0055BD90, C_00867078_SetByteAt18, "auto");
