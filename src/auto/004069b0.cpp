#include "hook.h"

// Compares three floats of two vectors exactly; a NaN in any component makes the result false.
static int __fastcall Vector3ExactEquals(float* self, void* /*edx*/, float* other) {
    if (self[0] != other[0]) {
        return 0;
    }
    if (self[1] != other[1]) {
        return 0;
    }
    if (self[2] != other[2]) {
        return 0;
    }
    return 1;
}

HOOK(0x004069B0, Vector3ExactEquals, "auto");
