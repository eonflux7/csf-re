#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GfxMatrix_AreVectorsDifferent, 0x0040FB20,
         (void* self, void* edx, float* a2));

static int __fastcall GfxMatrix_AreTransformsDifferent(
    float** self, void* /*edx*/, float** other)
{
    float* otherMatrix = CSF_AT(float*, other, 0x00);
    float* matrix = CSF_AT(float*, self, 0x00);

    if (CSF_AT(float, matrix, 0x20) != CSF_AT(float, otherMatrix, 0x20)
        || CSF_AT(float, matrix, 0x24) != CSF_AT(float, otherMatrix, 0x24)
        || CSF_AT(float, matrix, 0x28) != CSF_AT(float, otherMatrix, 0x28)
        || CSF_AT(float, matrix, 0x00) != CSF_AT(float, otherMatrix, 0x00)
        || CSF_AT(float, matrix, 0x04) != CSF_AT(float, otherMatrix, 0x04)
        || CSF_AT(float, matrix, 0x08) != CSF_AT(float, otherMatrix, 0x08))
    {
        return 1;
    }

    if (static_cast<std::uint8_t>(
            GfxMatrix_AreVectorsDifferent(matrix + 4, nullptr, otherMatrix + 4)))
    {
        return 1;
    }

    if (static_cast<std::uint8_t>(
            GfxMatrix_AreVectorsDifferent(matrix + 12, nullptr, otherMatrix + 12)))
    {
        return 1;
    }

    return 0;
}

HOOK(0x006ADC80, GfxMatrix_AreTransformsDifferent, "auto");
