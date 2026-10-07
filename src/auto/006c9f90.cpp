#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns true when bit 0 of the flags field at offset 0x14 is set.
static int __fastcall C_00862598_IsFlagBit0Set(C_00862598* self, void* /*edx*/)
{
    return (self->field_0014 & 1) == 1;
}

HOOK(0x006C9F90, C_00862598_IsFlagBit0Set, "auto");
