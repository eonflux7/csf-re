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

// Global dword read by this accessor.
CSF_GLOBAL(int, g_Value_8BC170, 0x8BC170);

// Returns the current value of the global at 0x8BC170.
static int __cdecl GetValue_8BC170()
{
    return g_Value_8BC170;
}

HOOK(0x74A680, GetValue_8BC170, "auto");
