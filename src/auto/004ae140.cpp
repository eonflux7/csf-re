#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the class/type name string for this object ("TMovilAPuntoRastreo").
static const char* __cdecl C_008392c0_GetTypeName()
{
    return CSF_STR(0x00839344, "TMovilAPuntoRastreo");
}

HOOK(0x004AE140, C_008392c0_GetTypeName, "auto");
