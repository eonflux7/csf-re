#include <cstdint>
#include "globals.h"
#include "hook.h"

// Converts an unsigned 32-bit value to a float in [0,1] scaled by 1/255.
// Mirrors the original: the value is loaded with signed fild, and if the
// sign bit was set, the unsigned bias (2^32) is added before scaling.
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_fInv255, 0x00830A00);

static float ScaleUnsignedByteToUnit(uint32_t value) {
    int32_t signedValue = static_cast<int32_t>(value);
    double scaled = static_cast<double>(signedValue);
    if (signedValue < 0) {
        scaled += static_cast<double>(g_UnsignedIntegerBias);
    }
    return static_cast<float>(scaled * static_cast<double>(g_fInv255));
}

// __thiscall: ecx = this (float[4] color), four unsigned channel values on the stack.
// Returns the 4th channel value (a4) as in the original.
static unsigned int __fastcall CMenuMissionInfo_SetNormalizedColor(
    float* self, void* /*edx*/, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5) {
    self[0] = ScaleUnsignedByteToUnit(a2);
    self[1] = ScaleUnsignedByteToUnit(a3);
    self[2] = ScaleUnsignedByteToUnit(a4);
    self[3] = ScaleUnsignedByteToUnit(a5);
    return a4;
}

HOOK(0x0053D260, CMenuMissionInfo_SetNormalizedColor, "auto");
