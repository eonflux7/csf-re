#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

// Returns the pointer to the constant class/type name "TNavegar".
static const char* __cdecl TNavegar_GetName()
{
    return CSF_STR(0x0083D4F8, "TNavegar");
}

HOOK(0x004D6E90, TNavegar_GetName, "auto");
