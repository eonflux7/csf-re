#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the class-name string of C_0083c0d8 ("TDarObjeto"), a constant
// string living at 0x0083C170 inside the game image.
static const char* __cdecl C_0083c0d8_GetClassName()
{
    return CSF_STR(0x0083C170, "TDarObjeto");
}

HOOK(0x004D4E30, C_0083c0d8_GetClassName, "auto");
