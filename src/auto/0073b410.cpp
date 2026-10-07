// Commandos: Strike Force (Pyro Studios) — reimplementation of 0x0073B410
// Stores a single byte passed by the caller into this+0x5C and returns it.

#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// __thiscall method (ecx = this, one stack arg, callee pops 4 bytes).
static char __fastcall C_00867444_SetField005c(C_00867444* self, void* /*edx*/, char value)
{
    self->field_005c = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x73B410, C_00867444_SetField005c, "auto");
