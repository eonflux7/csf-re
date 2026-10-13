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

// Returns TRUE only when the preload mode argument equals 2 (signed compare via sete after cmp).
static int __cdecl GameBddLoader_ShouldPreloadResources(int preloadMode) {
    return preloadMode == 2 ? 1 : 0;
}

HOOK(0x0069D5C0, GameBddLoader_ShouldPreloadResources, "auto");
