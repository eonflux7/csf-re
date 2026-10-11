#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x82D744);
CSF_GLOBAL(float, g_fInv255, 0x830A00);

// Converts one 8-bit channel to [0,1]. The original converts via a signed
// integer load and adds the bias only for negative values; channels are
// 0..255 so the bias path never runs, but it is kept to match the original.
static float ChannelByteToNormalizedFloat(uint32_t channelByte) {
    float value = (float)(int32_t)channelByte;
    if (value < 0.0f) {
        value += g_UnsignedIntegerBias;
    }
    return value * g_fInv255;
}

// Unpacks a 32-bit ARGB-style value into four normalized floats written to
// out[0] = bits 16-23, out[1] = bits 8-15, out[2] = bits 0-7, out[3] = bits 24-31.
// Returns the top byte (bits 24-31) as an integer.
static uint32_t WriteColorBytesAsNormalizedFloats(hook::Regs* regs, const uint32_t* args) {
    (void)args;
    const uint32_t packedColor = regs->eax;
    float* out = reinterpret_cast<float*>(regs->ecx);

    out[0] = ChannelByteToNormalizedFloat((packedColor >> 16) & 0xFF);
    out[1] = ChannelByteToNormalizedFloat((packedColor >> 8) & 0xFF);
    out[2] = ChannelByteToNormalizedFloat(packedColor & 0xFF);
    const uint32_t alphaByte = packedColor >> 24;
    out[3] = ChannelByteToNormalizedFloat(alphaByte);

    return alphaByte;
}

HOOK_REGS(0x709720, WriteColorBytesAsNormalizedFloats, 0, "gfx_water");
