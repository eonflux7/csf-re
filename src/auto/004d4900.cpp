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

// Returns the localized/identifier string "TVolviendo" (constant data at 0x0083BFE8).
static const char* __cdecl GetTVolviendo()
{
    return CSF_STR(0x83BFE8, "TVolviendo");
}

HOOK(0x004D4900, GetTVolviendo, "auto");
