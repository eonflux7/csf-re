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

// String literal "TParado" living in the game's data section.
CSF_GLOBAL(char, g_TParadoString, 0x82E958);

// Returns a pointer to the "TParado" (Spanish for "standing still") state name string.
static const char* __cdecl GetStoppedStateName()
{
    return &g_TParadoString;
}

HOOK(0x420670, GetStoppedStateName, "auto");
