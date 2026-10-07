#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the constant type-name string for this class ("TFncInteractivo").
static const char* __cdecl C_0083a788_GetTypeName()
{
    return CSF_STR(0x0083A80C, "TFncInteractivo");
}

HOOK(0x004B2A10, C_0083a788_GetTypeName, "auto");
