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

// Returns the literal string "TMuerto" (pointer to static data at 0x82f3d8).
static const char* __cdecl GetTMuertoString()
{
    return "TMuerto";
}

HOOK(0x420750, GetTMuertoString, "auto");
