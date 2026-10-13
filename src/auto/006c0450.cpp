#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

static int __fastcall GfxFrames_SetBoundsFromSphere(
    float* self, void* /*edx*/, float* sphere)
{
    const double diameter =
        static_cast<double>(CSF_AT(float, sphere, 0x0C)) * 2.0;
    const float centerX = CSF_AT(float, sphere, 0x00);
    const float centerY = CSF_AT(float, sphere, 0x04);
    const float centerZ = CSF_AT(float, sphere, 0x08);

    const double halfExtent =
        std::sqrt(diameter * diameter + diameter * diameter) *
        g_HalfDurationScale;

    CSF_AT(float, self, 0x00) = static_cast<float>(centerX + halfExtent);
    CSF_AT(float, self, 0x04) = static_cast<float>(centerY + halfExtent);
    CSF_AT(float, self, 0x08) = static_cast<float>(centerZ + halfExtent);
    CSF_AT(float, self, 0x0C) = static_cast<float>(centerX - halfExtent);

    const float lowerY = static_cast<float>(centerY - halfExtent);
    CSF_AT(float, self, 0x10) = lowerY;
    CSF_AT(float, self, 0x14) = static_cast<float>(centerZ - halfExtent);

    int result;
    std::memcpy(&result, &lowerY, sizeof(result));
    return result;
}

HOOK(0x006C0450, GfxFrames_SetBoundsFromSphere, "auto");
