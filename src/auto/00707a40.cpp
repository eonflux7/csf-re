#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));

static std::uint32_t* __fastcall C_008633a0_TransformPointByInverseMatrix(
    C_008633a0* self, void* /*edx*/, std::uint32_t* output, int input)
{
    alignas(8) std::uint8_t matrixBytes[0x10];
    S_006adb40& matrix = *reinterpret_cast<S_006adb40*>(matrixBytes);
    alignas(8) std::uint8_t pointBytes[0xC];

    // Address marker required by the packet; this function stores no vtable.
    (void)CSF_VTBL(0x006ADB40);

    self->vSlot1C(reinterpret_cast<int>(&matrix));

    std::uint32_t* inverseMatrix = RwMatrixCreate();
    std::memcpy(&matrix.field_0008, &inverseMatrix, sizeof(inverseMatrix));
    CSF_AT(std::uint8_t, matrixBytes, 0xC) = 1;

    Matrix_InvertWithStoredTarget(
        &matrix, nullptr, reinterpret_cast<float**>(&matrix.field_0008));

    std::memcpy(&inverseMatrix, &matrix.field_0008, sizeof(inverseMatrix));
    CSF_AT(std::uint32_t, pointBytes, 0x0) = 0;
    CSF_AT(std::uint32_t, pointBytes, 0x4) = 0;
    CSF_AT(std::uint32_t, pointBytes, 0x8) = 0;
    RwV3dTransformPoints(
        reinterpret_cast<int>(pointBytes), input, 1,
        reinterpret_cast<int>(inverseMatrix));

    const std::uint32_t x = CSF_AT(std::uint32_t, pointBytes, 0x0);
    const std::uint32_t y = CSF_AT(std::uint32_t, pointBytes, 0x4);
    output[0] = x;
    const std::uint32_t z = CSF_AT(std::uint32_t, pointBytes, 0x8);
    output[1] = y;
    const bool destroyInverse = CSF_AT(std::uint8_t, matrixBytes, 0xC) != 0;
    output[2] = z;

    if (destroyInverse) {
        std::memcpy(&inverseMatrix, &matrix.field_0008, sizeof(inverseMatrix));
        RwMatrixDestroy(inverseMatrix);
    }
    if (matrix.pad_0004[0]) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrix.field_0000));
    }
    return output;
}

HOOK(0x00707A40, C_008633a0_TransformPointByInverseMatrix, "auto");
