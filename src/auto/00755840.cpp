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

// Returns non-zero when the id is one of the three accepted values 4, 5 or 6.
static int __cdecl IsSubtitleOrSoundConfigId(int configId) {
    if (configId == 4 || configId == 6 || configId == 5) {
        return 1;
    }
    return 0;
}

HOOK(0x00755840, IsSubtitleOrSoundConfigId, "auto");
