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

// Raw name string living in the game's read-only data at 0x8322E8.
CSF_GLOBAL(char, g_TInterrupcion, 0x8322E8);

// Returns the literal type name "TInterrupcion" stored in game data.
static const char* __cdecl TInterrupcion_GetName()
{
    return &g_TInterrupcion;
}

HOOK(0x47B130, TInterrupcion_GetName, "auto");
