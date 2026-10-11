#include <cstdint>
#include "hook.h"

// Returns non-zero (1) when the object state code is 28 or 29, otherwise 0.
static int __cdecl IsObjectStateFlagSet(int stateCode) {
    return (stateCode == 28 || stateCode == 29) ? 1 : 0;
}

HOOK(0x755980, IsObjectStateFlagSet, "auto");
