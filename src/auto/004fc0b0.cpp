#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_fInv255, 0x00830A00);

// Converts one 0..255 colour channel to a normalized float (channel / 255).
// The bias branch mirrors the original's signed fild fixup.
static float AnimsChannelToNormalizedFloat(int channel) {
    float value = static_cast<float>(channel);
    if (channel < 0) {
        value += g_UnsignedIntegerBias;
    }
    return value * g_fInv255;
}

// Unpacks a packed 0xAARRGGBB-style dword into four floats at self[0..3]:
// self[0] = byte2, self[1] = byte1, self[2] = byte0, self[3] = 1.0.
// Returns the low byte of the input.
static int __fastcall AnimsUnpackColorToFloats(float* self, void* /*edx*/, int packedColor) {
    self[0] = AnimsChannelToNormalizedFloat((packedColor >> 16) & 0xFF);
    self[1] = AnimsChannelToNormalizedFloat((packedColor >> 8) & 0xFF);
    self[3] = 1.0f;
    self[2] = AnimsChannelToNormalizedFloat(packedColor & 0xFF);
    return packedColor & 0xFF;
}

HOOK(0x004FC0B0, AnimsUnpackColorToFloats, "auto");
