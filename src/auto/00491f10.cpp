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

// Returns the constant 12 (likely a size/count/version identifier).
static int __cdecl GetTwelve()
{
    return 12;
}

HOOK(0x491F10, GetTwelve, "auto");
