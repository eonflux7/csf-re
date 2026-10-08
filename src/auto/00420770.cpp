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

// Returns the literal string "TBlending".
static const char* __cdecl GetTBlendingName()
{
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x82f558));
}

HOOK(0x420770, GetTBlendingName, "auto");
