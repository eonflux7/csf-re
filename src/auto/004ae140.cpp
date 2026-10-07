#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the class/type name string for this object ("TMovilAPuntoRastreo").
static const char* __cdecl TMovilAPuntoRastreo_GetTypeName()
{
    return CSF_STR(0x00839344, "TMovilAPuntoRastreo");
}

HOOK(0x004AE140, TMovilAPuntoRastreo_GetTypeName, "auto");
