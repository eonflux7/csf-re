#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// 0x846128 holds the float constant 2*pi (6.2831855).
CSF_GLOBAL(float, g_MotionTwoPi, 0x846128);

// Subtracts 2*pi until the value is no longer greater than 2*pi.
// Values at or below 2*pi (including negatives) are returned unchanged.
static double ReduceAngleBelowTwoPi(float angle)
{
    double result = angle;
    while (result > g_MotionTwoPi) {
        result = result - g_MotionTwoPi;
    }
    return result;
}

HOOK_KEEP_ECX_CDECL(0x6D29B0, ReduceAngleBelowTwoPi, 1, "auto");
