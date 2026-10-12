#include <cstdint>
#include <cmath>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x82D444);

static int __fastcall GfxFrames_WriteOffsetPairFromDirection(
    float* out, void* /*edx*/, float* direction)
{
    const float doubledW = CSF_AT(float, direction, 0xC)
                         + CSF_AT(float, direction, 0xC);
    const float srcX = CSF_AT(float, direction, 0x0);
    const float srcY = CSF_AT(float, direction, 0x4);
    const float srcZ = CSF_AT(float, direction, 0x8);

    const float half = std::sqrt(doubledW * doubledW + doubledW * doubledW) * g_HalfDurationScale;

    CSF_AT(float, out, 0x0) = srcX + half;
    CSF_AT(float, out, 0x4) = srcY + half;
    CSF_AT(float, out, 0x8) = srcZ + half;
    CSF_AT(float, out, 0xC) = srcX - half;

    const float lowerY = srcY - half;
    CSF_AT(float, out, 0x10) = lowerY;
    CSF_AT(float, out, 0x14) = srcZ - half;

    int result;
    std::memcpy(&result, &lowerY, sizeof(result));
    return result;
}

HOOK(0x6C0450, GfxFrames_WriteOffsetPairFromDirection, "auto");
