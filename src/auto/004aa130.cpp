#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the constant string "TBuscar" (address 0x00836AC4).
static const char* __cdecl TBuscar_GetString()
{
    return CSF_STR(0x836AC4, "TBuscar");
}

HOOK(0x4AA130, TBuscar_GetString, "auto");
