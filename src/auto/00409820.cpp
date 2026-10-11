#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_RotateAroundAxisY, 0x006AE1D0, (int* self, void* , float angle));

static int __fastcall GfxTransform_GetRotatedPosition(void* self, void* /*edx*/)
{
    auto* position = static_cast<std::uint8_t*>(self) + 0x40;

    if (CSF_AT(std::uint8_t, self, 0x24))
    {
        CSF_AT(std::uint32_t, self, 0x40) =
            CSF_AT(std::uint32_t, self, 0x34);
        const auto sourceY = CSF_AT(std::uint32_t, self, 0x38);
        const auto sourceZ = CSF_AT(std::uint32_t, self, 0x3C);
        CSF_AT(std::uint32_t, self, 0x44) = sourceY;
        CSF_AT(std::uint32_t, self, 0x48) = sourceZ;

        auto* matrix = RwMatrixCreate();
        int matrixState[2] = {reinterpret_cast<int>(matrix), 1};
        GfxMatrix_RotateAroundAxisY(
            matrixState, nullptr, CSF_AT(float, self, 0x10));

        CSF_AT(float, self, 0x40) -= CSF_AT(float, self, 0x28);
        CSF_AT(float, self, 0x44) -= CSF_AT(float, self, 0x2C);
        CSF_AT(float, self, 0x48) -= CSF_AT(float, self, 0x30);

        std::uint32_t transformedPosition[3] = {};
        RwV3dTransformPoints(
            reinterpret_cast<int>(transformedPosition),
            reinterpret_cast<int>(position),
            1,
            matrixState[0]);

        CSF_AT(std::uint32_t, self, 0x40) = transformedPosition[0];
        const auto transformedY = transformedPosition[1];
        const auto transformedZ = transformedPosition[2];
        CSF_AT(std::uint32_t, self, 0x44) = transformedY;
        const bool ownsMatrix = CSF_AT(std::uint8_t, matrixState, 0x4) != 0;
        CSF_AT(std::uint32_t, self, 0x48) = transformedZ;

        CSF_AT(float, self, 0x40) += CSF_AT(float, self, 0x28);
        CSF_AT(float, self, 0x44) =
            CSF_AT(float, self, 0x2C) + CSF_AT(float, self, 0x44);
        CSF_AT(float, self, 0x48) =
            CSF_AT(float, self, 0x30) + CSF_AT(float, self, 0x48);

        if (ownsMatrix)
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixState[0]));
    }

    return reinterpret_cast<int>(position);
}

HOOK(0x00409820, GfxTransform_GetRotatedPosition, "auto");
