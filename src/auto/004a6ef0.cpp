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

// Returns the constant identifier string "TBajar" (stored at 0x834D40).
static const char* __cdecl GetBajarTag()
{
    return "TBajar";
}

HOOK(0x4A6EF0, GetBajarTag, "auto");
