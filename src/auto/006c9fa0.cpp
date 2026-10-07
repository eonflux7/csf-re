#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns TRUE when bit 1 (value 2) of field_0014 is set.
static int __fastcall C_00862598_HasBit1OfField0014(C_00862598* self, void* /*edx*/)
{
    return (self->field_0014 & 2) == 2;
}

HOOK(0x6C9FA0, C_00862598_HasBit1OfField0014, "auto");
