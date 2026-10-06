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

// Returns the string literal "TUsarInteractivo" (address 0x836534).
static const char* __cdecl GetUsarInteractivoName()
{
    return "TUsarInteractivo";
}

HOOK(0x004AA060, GetUsarInteractivoName, "auto");
