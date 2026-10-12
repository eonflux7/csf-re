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

// Returns non-zero when the action id is one of the three item-using actions: 6, 16 or 17.
static int __cdecl CharActionFsm_ItemUsesActionParameter(int actionId) {
    if (actionId == 6 || actionId == 16 || actionId == 17) {
        return 1;
    }
    return 0;
}

HOOK(0x00761040, CharActionFsm_ItemUsesActionParameter, "auto");
