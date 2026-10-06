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

// Returns the literal type-name string "TInit" (stored at 0x82f008).
static const char* __cdecl GetTInitName()
{
    return "TInit";
}

HOOK(0x420700, GetTInitName, "auto");
