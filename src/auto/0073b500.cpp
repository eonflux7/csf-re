#include <cstdint>
#include "globals.h"
#include "hook.h"

// The constant referenced by the original at 0x830A00 (1/255 as a 32-bit float).
CSF_GLOBAL(float, g_ByteToUnitScale, 0x830A00);

// 0x0073B500: loads the float stored at this+0x14 and scales it by the
// constant at 0x830A00, returning the (exact) product as the x87 result.
static double __fastcall GetNormalizedUnitValue(float* self, void* /*edx*/)
{
    return static_cast<double>(self[5]) * static_cast<double>(g_ByteToUnitScale);
}

HOOK(0x73B500, GetNormalizedUnitValue, "auto");
