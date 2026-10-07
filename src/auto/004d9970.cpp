#include <cstdint>
#include "globals.h"
#include "hook.h"

// Type-name accessor: simply returns the address of the class's name constant.
// __cdecl, no arguments, ret 0 (callee pops nothing).
static const char* __cdecl TConversacion_GetTypeName()
{
    return CSF_STR(0x0083DD28, "TConversacion");
}

HOOK(0x4D9970, TConversacion_GetTypeName, "auto");
