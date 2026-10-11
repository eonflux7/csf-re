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

// Returns non-zero when the id is one of the three accepted values 5, 6 or 19.
static int __cdecl GameConfig_IsAcceptedOptionId(int optionId) {
    if (optionId == 5 || optionId == 6 || optionId == 19) {
        return 1;
    }
    return 0;
}

HOOK(0x00755960, GameConfig_IsAcceptedOptionId, "auto");
