#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns 1 when the state value at +0x18 is 1, 2 or 3; otherwise 0.
static int __fastcall C_0083e5e8_IsStateOneTwoOrThree(C_0083e5e8* self, void* /*edx*/) {
    uint32_t stateValue = CSF_AT(uint32_t, self, 0x18);
    if (stateValue == 2 || stateValue == 1 || stateValue == 3) {
        return 1;
    }
    return 0;
}

HOOK(0x4E2D10, C_0083e5e8_IsStateOneTwoOrThree, "auto");
