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

// Returns non-zero when the reaction id is one of the three suppressed values 16, 17 or 18.
static int __cdecl IsReactionSuppressed(int reactionId) {
    if (reactionId == 18 || reactionId == 16 || reactionId == 17) {
        return 1;
    }
    return 0;
}

HOOK(0x00761090, IsReactionSuppressed, "auto");
