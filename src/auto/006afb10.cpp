#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(unsigned int, __fastcall, WeaponFirearms_DecodeColor, 0x00476430, (float* self, void* , unsigned int color));
CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

// Scales the four color channels (stored at +0x24..+0x30) by g_ColorChannelScale
// after decoding the packed color value. Returns the bit pattern of the scaled
// blue channel, as the original does via LODWORD.
static int __fastcall MenuControls_SetDisplayValue(float* self, void* /*edx*/, int packedColor)
{
    float* channels = self + 9;

    WeaponFirearms_DecodeColor(channels, nullptr, static_cast<unsigned int>(packedColor));

    float red = channels[0] * g_ColorChannelScale;
    float green = channels[1] * g_ColorChannelScale;
    float blue = channels[2] * g_ColorChannelScale;
    float alpha = channels[3] * g_ColorChannelScale;

    channels[0] = red;
    channels[1] = green;
    channels[2] = blue;
    channels[3] = alpha;

    int blueBits;
    std::memcpy(&blueBits, &blue, sizeof(blueBits));
    return blueBits;
}

HOOK(0x006AFB10, MenuControls_SetDisplayValue, "auto");
