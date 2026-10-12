#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixMultiply, 0x00624630, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(std::uint32_t*, __fastcall, GfxMatrix_TransformBasisAndPosition, 0x006ABF10, (std::uint32_t* self, void* , std::uint32_t* matrix));
CSF_FUNC(float*, __stdcall, GfxTeam_ComputePositionFromGeometry, 0x006FFFF0, (float* a1, float* a5, int a3));
CSF_FUNC(void*, __stdcall, GfxTeam_ReadPlayerGeometryData, 0x00700250, (void* a1, int arg4));

static int __fastcall C_008630e0_GetPlayerPosition(
    C_008630e0* self, void* /*edx*/, int output, int playerIndex, int positionIndex)
{
    alignas(8) std::uint8_t geometryBytes[0x3C];
    alignas(8) std::uint8_t matrixBytes[0xC];

    const std::uint32_t entryOffset =
        12 * static_cast<std::uint32_t>(playerIndex);
    auto* entries = CSF_AT(std::uint8_t*,
        reinterpret_cast<void*>(self->field_0004), 0x10);
    auto* entry = entries + entryOffset;
    auto* player = CSF_AT(std::uint32_t*, entry, 0);
    auto* geometry = CSF_AT(std::uint8_t*, player, 0x5C);

    CSF_AT(C_008630e0*, matrixBytes, 0x8) = self;

    if (CSF_AT(std::uint16_t, geometry, 0) == 2)
    {
        auto* parentMatrix = CSF_AT(std::uint32_t*, entry, 0x8);
        auto* localMatrix =
            (CSF_AT(std::uint32_t, geometry, 0x40) & 2) == 0
                ? player : nullptr;

        auto* product = RwMatrixCreate();
        RwMatrixMultiply(product, localMatrix, parentMatrix);
        CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
        std::memcpy(CSF_AT(std::uint32_t*, matrixBytes, 0), product, 0x40);
        RwMatrixDestroy(product);

        auto* resultMatrix = CSF_AT(std::uint32_t*, matrixBytes, 0);
        auto* destination = reinterpret_cast<void*>(output);
        CSF_AT(std::uint32_t, destination, 0) =
            CSF_AT(std::uint32_t, resultMatrix, 0x30);
        CSF_AT(std::uint32_t, destination, 0x4) =
            CSF_AT(std::uint32_t, resultMatrix, 0x34);
        CSF_AT(std::uint32_t, destination, 0x8) =
            CSF_AT(std::uint32_t, resultMatrix, 0x38);
        RwMatrixDestroy(resultMatrix);
        return output;
    }

    GfxTeam_ReadPlayerGeometryData(
        geometryBytes, reinterpret_cast<int>(player));

    entries = CSF_AT(std::uint8_t*,
        reinterpret_cast<void*>(self->field_0004), 0x10);
    geometry = CSF_AT(std::uint8_t*, player, 0x5C);
    const std::uint32_t flags = CSF_AT(std::uint32_t, geometry, 0x40);
    auto* parentMatrix =
        CSF_AT(std::uint32_t*, entries + entryOffset, 0x8);
    auto* localMatrix = (flags & 2) == 0 ? player : nullptr;

    auto* product = RwMatrixCreate();
    RwMatrixMultiply(product, localMatrix, parentMatrix);
    CSF_AT(std::uint32_t*, matrixBytes, 0) = RwMatrixCreate();
    std::memcpy(CSF_AT(std::uint32_t*, matrixBytes, 0), product, 0x40);
    CSF_AT(std::uint8_t, matrixBytes, 0x4) = 1;
    RwMatrixDestroy(product);

    GfxMatrix_TransformBasisAndPosition(
        reinterpret_cast<std::uint32_t*>(geometryBytes), nullptr,
        reinterpret_cast<std::uint32_t*>(matrixBytes));

    if (CSF_AT(std::uint8_t, matrixBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, matrixBytes, 0));

    GfxTeam_ComputePositionFromGeometry(
        reinterpret_cast<float*>(output),
        reinterpret_cast<float*>(geometryBytes), positionIndex);
    return output;
}

HOOK(0x00700500, C_008630e0_GetPlayerPosition, "auto");
