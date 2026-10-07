#include "globals.h"
#include "hook.h"

// Returns the name of the "TAutonomo" class type.
// Original disassembly:
//   004d7860  mov eax, 0x83d950      ; pointer to the static string "TAutonomo"
//   004d7865  ret
static const char* __cdecl C_0083da30_GetTypeName()
{
    return CSF_STR(0x0083D950, "TAutonomo");
}

HOOK(0x004D7860, C_0083da30_GetTypeName, "auto");
