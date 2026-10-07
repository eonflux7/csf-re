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

// Global integer stored at 0x8B0578, simply returned by the original function.
CSF_GLOBAL(int, g_GlobalValue_8B0578, 0x8B0578);

// Returns the value of the global at 0x8B0578 unchanged.
static int __cdecl Get_Global_8B0578()
{
    return g_GlobalValue_8B0578;
}

HOOK(0x5189D0, Get_Global_8B0578, "auto");
