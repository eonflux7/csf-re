#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns true when bit 2 (value 4) of the flag word at +0x30 is set.
static int __fastcall C_00862110_IsFlag4Set(C_00862110* self, void* /*edx*/)
{
    return (self->field_0030 & 4) == 4;
}

HOOK(0x006EEC60, C_00862110_IsFlag4Set, "auto");
