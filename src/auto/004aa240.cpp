#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the constant string "TParapeto" used as a class/type name.
static const char* __cdecl TParapeto_GetName()
{
    return CSF_STR(0x83857C, "TParapeto");
}

HOOK(0x4AA240, TParapeto_GetName, "auto");
