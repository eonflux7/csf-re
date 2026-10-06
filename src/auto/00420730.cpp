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

// Returns the literal name "TMoviendoseImpacto" (moving-impact animation).
static const char* __cdecl GetMoviendoseImpactoName()
{
    return CSF_STR(0x0082F250, "TMoviendoseImpacto");
}

HOOK(0x00420730, GetMoviendoseImpactoName, "auto");
