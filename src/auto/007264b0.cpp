#include <cstdint>
#include "globals.h"
#include "hook.h"

// Constant data of the original, read directly by the function.
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

// 0x007264B0 (7 bytes, ret 0)
// Loads the float constant at 0x0082D444 (0.5f) and returns it as a double
// (the x87 fld widens the float to the double return value).
static double __cdecl GetHalfDurationScale()
{
    return g_HalfDurationScale;
}

HOOK(0x007264B0, GetHalfDurationScale, "auto");
