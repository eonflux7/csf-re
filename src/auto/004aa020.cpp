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

// Returns the literal name of the "TCover" type. The original simply loads the
// address of the string constant at 0x836C64, so this is a constant accessor.
static const char* __cdecl TCover_GetName()
{
    return "TCover";
}

HOOK(0x4AA020, TCover_GetName, "auto");
