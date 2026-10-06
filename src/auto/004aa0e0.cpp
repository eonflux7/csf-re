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

// Returns the literal name "TDesplazamiento" (Spanish for "displacement/offset").
// Original: mov eax, 0x837734 ; ret  -> pointer to a static string inside the game image.
static const char* __cdecl GetDesplazamientoName()
{
    return "TDesplazamiento";
}

HOOK(0x4AA0E0, GetDesplazamientoName, "auto");
