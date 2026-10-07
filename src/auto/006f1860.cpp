#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"

// Returns the allocation/serialization size (0x90 = 144 bytes) of a C_00862490
// instance. The original just materialises the constant into eax and returns.
static int __fastcall C_00862490_GetObjectSize(C_00862490* /*self*/, void* /*edx*/)
{
    return 144;
}

HOOK(0x006F1860, C_00862490_GetObjectSize, "auto");
