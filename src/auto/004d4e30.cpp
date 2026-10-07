#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the class-name string of TDarObjeto ("TDarObjeto"), a constant
// string living at 0x0083C170 inside the game image.
static const char* __cdecl TDarObjeto_GetClassName()
{
    return CSF_STR(0x0083C170, "TDarObjeto");
}

HOOK(0x004D4E30, TDarObjeto_GetClassName, "auto");
