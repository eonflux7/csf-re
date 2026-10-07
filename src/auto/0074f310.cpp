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

// Returns the constant name string "NetPlayerList" (0x008686BC).
static const char* __cdecl GetNetPlayerListString()
{
    return CSF_STR(0x8686BC, "NetPlayerList");
}

HOOK(0x74F310, GetNetPlayerListString, "auto");
