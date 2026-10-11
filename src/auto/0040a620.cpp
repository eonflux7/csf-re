#include "hook.h"

// Returns a1 when *a1 < *a2 (ordered compare); otherwise returns a2.
// Unordered (NaN) operands take the a2 branch, matching the original's
// fcomp/test ah,5/jnp sequence.
static float* __cdecl PickSmallerFloatPointer(float* a1, float* a2)
{
    if (!(*a1 < *a2))
        return a2;
    return a1;
}

HOOK(0x0040A620, PickSmallerFloatPointer, "auto");
