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

// Global state word read by this accessor.
CSF_GLOBAL(int, g_GlobalState, 0x8B0570);

// Returns the current value of the global state word.
static int __cdecl GetGlobalState()
{
    return g_GlobalState;
}

HOOK(0x50D190, GetGlobalState, "auto");
