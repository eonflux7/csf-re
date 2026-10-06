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

// Returns the name string of the TFncGirar object at 0x0082F9D0.
static const char* __cdecl TFncGirar_GetName()
{
    return CSF_STR(0x0082F9D0, "TFncGirar");
}

HOOK(0x4AA0A0, TFncGirar_GetName, "auto");
