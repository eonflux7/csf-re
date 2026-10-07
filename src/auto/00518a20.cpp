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

// Global integer read by the accessor at 0x00518A20.
CSF_GLOBAL(int, g_Value_8B057C, 0x8B057C);

// Returns the value of the global stored at 0x008B057C.
static int __cdecl GetValue_518A20()
{
    return g_Value_8B057C;
}

HOOK(0x518A20, GetValue_518A20, "auto");
