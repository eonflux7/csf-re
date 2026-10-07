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

// Returns the constant string "TPrcBuscarParapeto" (address 0x0083A508).
static const char* __cdecl GetBuscarParapetoName()
{
    return CSF_STR(0x0083A508, "TPrcBuscarParapeto");
}

HOOK(0x004B2710, GetBuscarParapetoName, "auto");
