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

// Returns the constant literal "TPostAction".
static const char* __cdecl GetTPostActionString()
{
    return "TPostAction";
}

HOOK(0x4206F0, GetTPostActionString, "auto");
