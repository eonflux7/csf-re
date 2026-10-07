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

// Returns the object size (112 bytes / 0x70) for class C_00860cbc.
static int __cdecl C_00860cbc_GetSize()
{
    return 112;
}

HOOK(0x6CF6B0, C_00860cbc_GetSize, "auto");
