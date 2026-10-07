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

// Returns the constant type-name string "TFncRastrear".
static const char* __cdecl GetFncRastrearTypeName()
{
    return CSF_STR(0x00839864, "TFncRastrear");
}

HOOK(0x4AE3C0, GetFncRastrearTypeName, "auto");
