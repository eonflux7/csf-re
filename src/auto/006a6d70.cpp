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

// Global read by the original: dword_8BB418.
CSF_GLOBAL(int, g_GlobalState, 0x8BB418);

// 0x006A6D70 — trivially returns the global at 0x008BB418.
static int __cdecl GetGlobalState()
{
    return g_GlobalState;
}

HOOK(0x6A6D70, GetGlobalState, "auto");
