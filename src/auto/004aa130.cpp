#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the literal identifier "TBuscar" (Spanish: "search").
// Original code: `mov eax, 0x836ac4 ; ret`  -> returns pointer to the string literal.
static const char* __cdecl TBuscar_GetString()
{
    return "TBuscar";
}

HOOK(0x004AA130, TBuscar_GetString, "auto");
