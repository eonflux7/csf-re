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

// Returns the constant name of the "TPostAction" animation/action group.
static const char* GetTPostActionString()
{
    return CSF_STR(0x82EF48, "TPostAction");
}

HOOK(0x4206F0, GetTPostActionString, "auto");
