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

// Returns non-zero when the object type id is one of the three special types 22, 23 or 24.
static int __cdecl IsSpecialObjectType(int objectType) {
    if (objectType == 22 || objectType == 23 || objectType == 24) {
        return 1;
    }
    return 0;
}

HOOK(0x00755940, IsSpecialObjectType, "auto");
