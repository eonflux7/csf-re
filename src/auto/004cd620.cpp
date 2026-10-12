#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Decrements the counter at +0x40 and returns the pre-decrement value minus one.
// When the counter was 1 (now 0), clears bit 0 of the flags at +0x18 and resets the counter.
static int __fastcall C_0083b118_DecrementRefCountReturnRemaining(C_0083b118* self, void* /*edx*/) {
    uint32_t counter = CSF_AT(uint32_t, self, 0x40);
    int result = static_cast<int>(counter) - 1;
    if (counter == 1) {
        self->field_0018 &= ~1u;
        CSF_AT(uint32_t, self, 0x40) = 0;
    }
    return result;
}

HOOK(0x004CD620, C_0083b118_DecrementRefCountReturnRemaining, "auto");
