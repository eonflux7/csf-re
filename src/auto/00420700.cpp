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

// Returns the constant string name "TInit" (address 0x0082F008).
static const char* __cdecl GetTInitName()
{
    return CSF_STR(0x0082F008, "TInit");
}

HOOK(0x00420700, GetTInitName, "auto");
