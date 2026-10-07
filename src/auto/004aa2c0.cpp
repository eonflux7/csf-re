#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

// Returns the literal string "TSalir" (Spanish: "to exit / leave").
static const char* __cdecl C_4AA2C0_GetSalirString()
{
    return CSF_STR(0x838E4C, "TSalir");
}

HOOK(0x4AA2C0, C_4AA2C0_GetSalirString, "auto");
