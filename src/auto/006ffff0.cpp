#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_SetFourRowsAndUpdate, 0x006ADA10, (S_006ada10* self, void* , uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));

// The packet labels these code addresses as type/vtable references.
// The disassembly contains calls to them, not vtable stores.
[[maybe_unused]] static const auto kMatrixRowsType = CSF_VTBL(0x006ADA10);
[[maybe_unused]] static const auto kMatrixInverseType = CSF_VTBL(0x006ADB40);

static float GfxTeam_ClampToSymmetricExtent(float coordinate, float extent)
{
    if (-extent > extent)
    {
        if (coordinate < extent)
            return extent;
        if (coordinate > -extent)
            return -extent;
    }
    else
    {
        if (coordinate < -extent)
            return -extent;
        if (coordinate > extent)
            return extent;
    }
    return coordinate;
}

static float* __stdcall GfxTeam_GetClosestPointOnOrientedBox(
    float* result, float* box, int point)
{
    alignas(8) std::uint8_t transformedBytes[0xC];
    alignas(8) std::uint8_t clampedBytes[0xC];
    alignas(8) std::uint8_t inverseBytes[0x8];
    alignas(8) std::uint8_t matrixBytes[0x8];

    float* transformed = reinterpret_cast<float*>(transformedBytes);
    float* clamped = reinterpret_cast<float*>(clampedBytes);
    S_006ada10& matrix = *reinterpret_cast<S_006ada10*>(matrixBytes);

    matrix.field_0000 = (decltype(matrix.field_0000))RwMatrixCreate();
    CSF_AT(std::uint8_t, matrixBytes, 0x4) = 1;
    GfxMatrix_SetFourRowsAndUpdate(
        &matrix, nullptr,
        reinterpret_cast<std::uint32_t*>(box + 6),
        reinterpret_cast<std::uint32_t*>(box + 9),
        reinterpret_cast<std::uint32_t*>(box + 12),
        reinterpret_cast<std::uint32_t*>(box));

    CSF_AT(float*, inverseBytes, 0x0) =
        reinterpret_cast<float*>(RwMatrixCreate());
    CSF_AT(std::uint8_t, inverseBytes, 0x4) = 1;
    Matrix_InvertWithStoredTarget(
        reinterpret_cast<S_006adb40*>(matrixBytes), nullptr,
        &CSF_AT(float*, inverseBytes, 0x0));

    transformed[0] = 0.0f;
    transformed[1] = 0.0f;
    transformed[2] = 0.0f;
    RwV3dTransformPoints(
        reinterpret_cast<int>(transformed), point, 1,
        reinterpret_cast<int>(CSF_AT(float*, inverseBytes, 0x0)));

    clamped[0] = transformed[0];
    clamped[1] = transformed[1];
    clamped[2] = transformed[2];
    transformed[0] = box[3];
    transformed[1] = box[4];
    transformed[2] = box[5];

    clamped[0] = GfxTeam_ClampToSymmetricExtent(clamped[0], transformed[0]);
    clamped[1] = GfxTeam_ClampToSymmetricExtent(clamped[1], transformed[1]);
    clamped[2] = GfxTeam_ClampToSymmetricExtent(clamped[2], transformed[2]);

    transformed[0] = 0.0f;
    transformed[1] = 0.0f;
    transformed[2] = 0.0f;
    RwV3dTransformPoints(
        reinterpret_cast<int>(transformed),
        reinterpret_cast<int>(clamped), 1,
        (int)matrix.field_0000);

    result[0] = transformed[0];
    result[1] = transformed[1];
    result[2] = transformed[2];

    if (CSF_AT(std::uint8_t, inverseBytes, 0x4))
        RwMatrixDestroy(
            reinterpret_cast<std::uint32_t*>(
                CSF_AT(float*, inverseBytes, 0x0)));
    if (CSF_AT(std::uint8_t, matrixBytes, 0x4))
        RwMatrixDestroy((std::uint32_t*)matrix.field_0000);

    return result;
}

HOOK(0x006FFFF0, GfxTeam_GetClosestPointOnOrientedBox, "auto");
