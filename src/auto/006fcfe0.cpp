#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Sets field_003d (byte at +0x3D) to the given value and returns it.
static char __fastcall C_00862f70_SetValue3d(C_00862f70* self, void* /*edx*/, char value)
{
    self->field_003d = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x006FCFE0, C_00862f70_SetValue3d, "auto");
