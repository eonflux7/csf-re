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

// Returns non-zero when the action code is one of the enabled codes:
// 0 through 8 inclusive, plus 11, 12, 13, 18, 20 and 30.
static int __cdecl IsActionCodeEnabled(unsigned int actionCode)
{
    if (actionCode <= 8)
        return 1;
    return (actionCode == 11 ||
            actionCode == 12 ||
            actionCode == 13 ||
            actionCode == 20 ||
            actionCode == 30 ||
            actionCode == 18) ? 1 : 0;
}

HOOK(0x761260, IsActionCodeEnabled, "auto");
