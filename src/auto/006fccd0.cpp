#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the size of the C_00862f70 object in bytes (0x84 == 132),
// matching sizeof(C_00862f70). Likely a virtual size/allocator accessor.
static int __fastcall C_00862f70_GetObjectSize(C_00862f70* /*self*/, void* /*edx*/)
{
    return 132;
}

HOOK(0x006FCCD0, C_00862f70_GetObjectSize, "auto");
