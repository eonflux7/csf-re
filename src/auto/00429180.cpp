#include <cstdint>
#include "globals.h"
#include "hook.h"

// Constant data referenced by the original (single-precision 0.1f).
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);

// 0x00429180 (7 bytes, ret 0)
// Loads a constant float from 0x0082DC84 and returns it; the FPU value is
// promoted to double at the call boundary (Hex-Rays renders the constant as 0.1).
static double __cdecl C_008300c0_GetConstant() {
    return g_flt_0082DC84;
}

HOOK(0x00429180, C_008300c0_GetConstant, "auto");
