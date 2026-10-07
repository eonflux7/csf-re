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

// Returns the literal class name "NetControl" (address 0x00868544).
static const char* __cdecl NetControl_GetName()
{
    return CSF_STR(0x00868544, "NetControl");
}

HOOK(0x0074E660, NetControl_GetName, "auto");
