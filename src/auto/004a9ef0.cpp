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

// Returns the class name string "TEvitarOclusores" ("avoid occluders").
static const char* __cdecl GetTEvitarOclusoresName()
{
    return CSF_STR(0x008356FC, "TEvitarOclusores");
}

HOOK(0x004A9EF0, GetTEvitarOclusoresName, "auto");
