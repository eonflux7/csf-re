#include "hook.h"

// Returns 1 when any of the four floats differs from the other vector's
// corresponding component (NaN counts as different), 0 when all four are equal.
static int __fastcall Float4_IsNotEqual(float* self, void* /*edx*/, float* other)
{
    if (self[0] != other[0])
        return 1;
    if (self[1] != other[1])
        return 1;
    if (self[2] != other[2])
        return 1;
    if (self[3] != other[3])
        return 1;
    return 0;
}

HOOK(0x737310, Float4_IsNotEqual, "auto");
