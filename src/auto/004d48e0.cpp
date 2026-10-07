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

// Returns the constant string "TRespondiendo" (0x0083BDA8).
static const char* __cdecl GetRespondiendoString()
{
    return CSF_STR(0x0083BDA8, "TRespondiendo");
}

HOOK(0x004D48E0, GetRespondiendoString, "auto");
