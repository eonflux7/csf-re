#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Sets the byte stored at offset 0x0C of the object and returns that same value.
static char __fastcall C_008656d0_SetField000c(C_008656d0* self, void* /*edx*/, char value)
{
    self->field_000c = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x00713FA0, C_008656d0_SetField000c, "auto");
