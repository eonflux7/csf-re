#include <cstdint>
#include "hook.h"

// Returns 1 for the flag ids 0-5, 11, 13, 14 and 20; 0 otherwise.
static int AppConfig_IsFlagSet(uint32_t flagId) {
    if (flagId < 6) {
        return 1;
    }
    if (flagId == 11 || flagId == 13 || flagId == 14 || flagId == 20) {
        return 1;
    }
    return 0;
}

HOOK(0x00761210, AppConfig_IsFlagSet, "auto");
