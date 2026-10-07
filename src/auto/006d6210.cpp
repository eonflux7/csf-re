#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the size in bytes (0x108 = 264) of a C_00861120 instance.
// Leaf function: just loads the immediate and returns.
static int __fastcall C_00861120_GetSize(C_00861120* /*self*/, void* /*edx*/)
{
    return 0x108;
}

HOOK(0x6D6210, C_00861120_GetSize, "auto");
