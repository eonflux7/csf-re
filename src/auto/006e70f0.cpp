#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, C_00861d48_ResolveTransformMatrix, 0x006E6D90, (void* self, void* edx, int a2, int a3));

static std::uint32_t* __fastcall C_00861d48_TransformIndexedPoint(
    C_00861d48* self, void* /*edx*/, std::uint32_t* output, int index)
{
    std::uint32_t* matrix = RwMatrixCreate();
    const std::uint32_t recordOffset = 20 * static_cast<std::uint32_t>(index);

    alignas(8) std::uint8_t matrixResultBytes[0xC];
    auto* records = reinterpret_cast<std::uint8_t*>(
        CSF_AT(std::uint32_t, self, 0x3C));

    const int matrixResult = C_00861d48_ResolveTransformMatrix(
        self, nullptr, reinterpret_cast<int>(matrixResultBytes),
        CSF_AT(int, records + recordOffset, 0x4));

    std::memcpy(
        matrix,
        CSF_AT(std::uint32_t*, reinterpret_cast<void*>(matrixResult), 0),
        0x40);

    if (CSF_AT(std::uint8_t, matrixResultBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixResultBytes, 0));

    records = reinterpret_cast<std::uint8_t*>(
        CSF_AT(std::uint32_t, self, 0x3C));
    auto* point = records + recordOffset + 8;

    alignas(8) std::uint8_t transformedPointBytes[0x18];
    CSF_AT(std::uint32_t, transformedPointBytes, 0) = 0;
    CSF_AT(std::uint32_t, transformedPointBytes, 0x4) = 0;
    CSF_AT(std::uint32_t, transformedPointBytes, 0x8) = 0;

    RwV3dTransformPoints(
        reinterpret_cast<int>(transformedPointBytes),
        reinterpret_cast<int>(point), 1, reinterpret_cast<int>(matrix));

    const std::uint32_t y = CSF_AT(std::uint32_t, transformedPointBytes, 0x4);
    output[0] = CSF_AT(std::uint32_t, transformedPointBytes, 0);
    const std::uint32_t z = CSF_AT(std::uint32_t, transformedPointBytes, 0x8);
    output[1] = y;
    output[2] = z;

    RwMatrixDestroy(matrix);
    return output;
}

HOOK(0x006E70F0, C_00861d48_TransformIndexedPoint, "auto");
