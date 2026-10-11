#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns 1 when the state word at +0x18 is 0 or 3, otherwise 0.
static int __fastcall C_0083e5e8_IsStateZeroOrThree(C_0083e5e8* self, void* /*edx*/) {
    uint32_t state = CSF_AT(uint32_t, self, 0x18);
    if (state == 0 || state == 3) {
        return 1;
    }
    return 0;
}

HOOK(0x4E1D50, C_0083e5e8_IsStateZeroOrThree, "auto");
