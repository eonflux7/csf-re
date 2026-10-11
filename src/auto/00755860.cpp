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

// __cdecl: caller pops arguments, so this function uses `ret` with no immediate.
// Returns 1 for control kinds 0, 2 and 3 (the alternate control kinds), 0 otherwise.
static int IsAlternateControlKind(int kind)
{
    return kind == 0 || kind == 2 || kind == 3;
}

HOOK(0x00755860, IsAlternateControlKind, "auto");
