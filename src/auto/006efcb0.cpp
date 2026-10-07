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

// Returns the size in bytes of a C_00862240 instance (0xD0 == 208).
static int __cdecl C_00862240_GetSize()
{
    return 0xD0;
}

HOOK(0x006EFCB0, C_00862240_GetSize, "auto");
