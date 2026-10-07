#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the size (in bytes) of a C_00862598 instance.
// Identified by: mov eax, 0x54 ; ret
static int __fastcall C_00862598_GetSize(C_00862598* /*self*/, void* /*edx*/)
{
    return 84; // 0x54 == sizeof(C_00862598)
}

HOOK(0x006F24F0, C_00862598_GetSize, "auto");
