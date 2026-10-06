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

// Returns the animation/state name literal "TMoviendoseImpacto".
static const char* __cdecl GetMoviendoseImpactoName()
{
    return "TMoviendoseImpacto";
}

HOOK(0x420730, GetMoviendoseImpactoName, "auto");
