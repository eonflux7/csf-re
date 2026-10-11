#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Decrements the counter at +0x70 and returns the pre-decrement value minus one.
// When the counter was 1 (now 0), clears bit 0 of the flags at +0x18 and resets the counter.
static int __fastcall C_0083ae28_DecrementRefCountReturnRemaining(C_0083ae28* self, void* /*edx*/) {
    int result = static_cast<int>(self->field_0070) - 1;
    if (self->field_0070 == 1) {
        self->field_0018 &= ~1u;
        self->field_0070 = 0;
    }
    return result;
}

HOOK(0x004CEC60, C_0083ae28_DecrementRefCountReturnRemaining, "auto");
