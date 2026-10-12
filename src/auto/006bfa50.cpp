#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformVectors, 0x006230A0, (int a1, int a2, int a3, int a4));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));

[[maybe_unused]] static const auto kMatrixDescriptorBinding = CSF_VTBL(0x006ADB40);

static std::uint32_t* __fastcall C_00860700_TransformVectorToLocal(
    C_00860700* self, void* /*edx*/, std::uint32_t* output, int input)
{
    alignas(8) std::uint8_t matrixBytes[0x10];
    S_006adb40& matrix = *reinterpret_cast<S_006adb40*>(matrixBytes);

    matrix.field_0000 =
        reinterpret_cast<std::uint32_t>(RwFrameGetLTM(self->field_000c));
    matrix.pad_0004[0] = 0;

    std::uint32_t* inverse = RwMatrixCreate();
    std::memcpy(&matrix.field_0008, &inverse, sizeof(inverse));
    CSF_AT(std::uint8_t, matrixBytes, 0xC) = 1;

    Matrix_InvertWithStoredTarget(
        &matrix, nullptr, reinterpret_cast<float**>(&matrix.field_0008));

    alignas(8) std::uint8_t vectorBytes[0xC];
    CSF_AT(std::uint32_t, vectorBytes, 0x0) = 0;
    CSF_AT(std::uint32_t, vectorBytes, 0x4) = 0;
    CSF_AT(std::uint32_t, vectorBytes, 0x8) = 0;

    std::memcpy(&inverse, &matrix.field_0008, sizeof(inverse));
    RwV3dTransformVectors(
        reinterpret_cast<int>(vectorBytes), input, 1,
        reinterpret_cast<int>(inverse));

    const std::uint32_t x = CSF_AT(std::uint32_t, vectorBytes, 0x0);
    const std::uint32_t y = CSF_AT(std::uint32_t, vectorBytes, 0x4);
    output[0] = x;
    const std::uint32_t z = CSF_AT(std::uint32_t, vectorBytes, 0x8);
    output[1] = y;
    output[2] = z;

    if (CSF_AT(std::uint8_t, matrixBytes, 0xC)) {
        std::memcpy(&inverse, &matrix.field_0008, sizeof(inverse));
        RwMatrixDestroy(inverse);
    }
    if (matrix.pad_0004[0])
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrix.field_0000));

    return output;
}

HOOK(0x006BFA50, C_00860700_TransformVectorToLocal, "auto");
