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

// Returns the constant string "TDisparando" (a __cdecl function returning a pointer
// to a global string literal). The original simply loads the address 0x008323B0
// and returns; no stack arguments, so it pops 0 bytes (ret 0).
static const char* __cdecl C_0047b110_GetDisparandoText()
{
    return CSF_STR(0x008323B0, "TDisparando");
}

HOOK(0x0047B110, C_0047b110_GetDisparandoText, "auto");
