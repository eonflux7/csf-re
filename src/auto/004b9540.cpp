#include <cstdint>
#include "hook.h"

// Returns the constant type/kind identifier 0x21 (33).
static int __cdecl TFncMacroOrden_GetTypeId()
{
    return 33;
}

HOOK(0x4B9540, TFncMacroOrden_GetTypeId, "auto");
