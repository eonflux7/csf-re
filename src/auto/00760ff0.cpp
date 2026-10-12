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

// Returns 1 when the sub-object field id belongs to the set of flagged
// sub-object fields, 0 otherwise.
static int __cdecl AppConfig_IsSubObjectFieldFlagSet(int fieldId) {
    switch (fieldId) {
        case 6:
        case 7:
        case 8:
        case 10:
        case 16:
        case 17:
        case 18:
        case 19:
        case 21:
        case 24:
        case 25:
            return 1;
        default:
            return 0;
    }
}

HOOK_KEEP_ECX_CDECL(0x760FF0, AppConfig_IsSubObjectFieldFlagSet, 0, "app_config");
