#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns true when either byte flag at offsets 0x59 / 0x5A of the object is non-zero.
// The original sets al via "xor al, al" / "mov al, 1" and returns with ret 8 (__stdcall, 2 args).
static bool __stdcall IsEitherFlag59Or5AEnabled(int a1, int a2) {
    (void)a1;
    bool result = false;
    uint8_t flag59 = CSF_AT(uint8_t, a2, 0x59);
    if (flag59 != 0) {
        return true;
    }
    uint8_t flag5A = CSF_AT(uint8_t, a2, 0x5A);
    if (flag5A != 0) {
        result = true;
    }
    return result;
}

HOOK(0x004A4380, IsEitherFlag59Or5AEnabled, "auto");
