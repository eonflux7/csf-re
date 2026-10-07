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

// Flag byte at 0x8BB3EC (set to 1 by this function).
CSF_GLOBAL(uint8_t, g_SomeFlag_8BB3EC, 0x8BB3EC);

// 0x0069D570: stores 1 into the global flag byte at 0x8BB3EC.
static void __cdecl SetFlag_8BB3EC()
{
    g_SomeFlag_8BB3EC = 1;
}

HOOK(0x69D570, SetFlag_8BB3EC, "auto");
