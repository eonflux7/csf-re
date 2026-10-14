#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static float* __fastcall GfxMatrix_GetBoundingSphere(
    float* self, void* /*edx*/, float* output)
{
    const float x =
        CSF_AT(float, self, 0x0C) * CSF_AT(float, self, 0x18) +
        CSF_AT(float, self, 0x10) * CSF_AT(float, self, 0x24) +
        CSF_AT(float, self, 0x14) * CSF_AT(float, self, 0x30);
    const float y =
        CSF_AT(float, self, 0x0C) * CSF_AT(float, self, 0x1C) +
        CSF_AT(float, self, 0x10) * CSF_AT(float, self, 0x28) +
        CSF_AT(float, self, 0x14) * CSF_AT(float, self, 0x34);
    const float z =
        CSF_AT(float, self, 0x0C) * CSF_AT(float, self, 0x20) +
        CSF_AT(float, self, 0x10) * CSF_AT(float, self, 0x2C) +
        CSF_AT(float, self, 0x14) * CSF_AT(float, self, 0x38);

    CSF_AT(std::uint32_t, output, 0x00) = CSF_AT(std::uint32_t, self, 0x00);
    CSF_AT(std::uint32_t, output, 0x04) = CSF_AT(std::uint32_t, self, 0x04);
    CSF_AT(std::uint32_t, output, 0x08) = CSF_AT(std::uint32_t, self, 0x08);
    CSF_AT(float, output, 0x0C) = static_cast<float>(std::sqrt((double)z * z + (double)y * y + (double)x * x));
    return output;
}

HOOK(0x006AC530, GfxMatrix_GetBoundingSphere, "auto");
