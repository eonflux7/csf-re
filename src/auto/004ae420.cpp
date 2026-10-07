#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the constant identifier string "TFncBusqueda" for this object.
static const char* __fastcall C_00839878_GetTFncBusqueda(C_00839878* self, void* /*edx*/)
{
    (void)self;
    return CSF_STR(0x008398FC, "TFncBusqueda");
}

HOOK(0x4AE420, C_00839878_GetTFncBusqueda, "auto");
