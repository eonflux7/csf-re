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

// Returns the object size (0x24 = 36 bytes) - likely GetSize / GetClassSize accessor.
static int __cdecl C_00862900_GetSize()
{
    return 36;
}

HOOK(0x006C7AC0, C_00862900_GetSize, "auto");
