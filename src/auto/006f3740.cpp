#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the size in bytes of the C_00862748 object (0x88 = 136).
static int __fastcall C_00862748_GetSize(C_00862748* self, void* /*edx*/)
{
    (void)self;
    return 136;
}

HOOK(0x6F3740, C_00862748_GetSize, "auto");
