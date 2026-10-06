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

// Returns the constant identifier string "TRendirse" used by the game.
static const char* __cdecl GetTRendirse()
{
    return CSF_STR(0x83713C, "TRendirse");
}

HOOK(0x4AA080, GetTRendirse, "auto");
