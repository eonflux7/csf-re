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

// Returns the pointer to the static string literal "TIrHacia" (0x008359C4).
static const char* __cdecl GetTIrHaciaString()
{
    return CSF_STR(0x8359C4, "TIrHacia");
}

HOOK(0x004AA140, GetTIrHaciaString, "auto");
