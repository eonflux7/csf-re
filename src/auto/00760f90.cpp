#include <cstdint>
#include "hook.h"

// Returns 1 when the sub-state id belongs to the set of enabled sub-states, else 0.
static int __cdecl AppConfig_IsSubStateEnabled(int subState) {
    if (subState == 21 || subState == 16 || subState == 17 || subState == 6 ||
        subState == 22 || subState == 23 || subState == 24 || subState == 25 ||
        subState == 26 || subState == 27 || subState == 28 || subState == 31 ||
        subState == 33) {
        return 1;
    }
    return 0;
}

HOOK_KEEP_ECX_CDECL(0x760F90, AppConfig_IsSubStateEnabled, 1, "app_config");
