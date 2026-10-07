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

// Global int read by the target function (address referenced by the original).
CSF_GLOBAL(int, g_Value_8B0568, 0x008B0568);

// Returns the value of the global variable stored at 0x008B0568.
static int __cdecl GetGlobalValue_8B0568()
{
    return g_Value_8B0568;
}

HOOK(0x4FDBB0, GetGlobalValue_8B0568, "auto");
