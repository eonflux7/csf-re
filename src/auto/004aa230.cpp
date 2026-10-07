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

// Returns the constant string "TAtacarOff" (0x008384EC).
static const char* __cdecl GetTAtacarOffName()
{
    return CSF_STR(0x008384EC, "TAtacarOff");
}

HOOK(0x004AA230, GetTAtacarOffName, "auto");
