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

// Returns the type-name string constant "TUsarInteractivo".
static const char* __cdecl GetUsarInteractivoName()
{
    return CSF_STR(0x00836534, "TUsarInteractivo");
}

HOOK(0x004AA060, GetUsarInteractivoName, "auto");
