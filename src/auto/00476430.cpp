#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_fInv255, 0x00830A00);

static unsigned int __fastcall WeaponFirearms_DecodeColor(
    float* self, void* /*edx*/, unsigned int color)
{
    CSF_AT(float, self, 0x00) = static_cast<float>((color >> 16) & 0xff) * g_fInv255;
    CSF_AT(float, self, 0x04) = static_cast<float>((color >> 8) & 0xff) * g_fInv255;
    CSF_AT(float, self, 0x08) = static_cast<float>(color & 0xff) * g_fInv255;
    const unsigned int alpha = color >> 24;
    CSF_AT(float, self, 0x0C) = static_cast<float>(alpha) * g_fInv255;
    return alpha;
}

HOOK(0x00476430, WeaponFirearms_DecodeColor, "auto");
