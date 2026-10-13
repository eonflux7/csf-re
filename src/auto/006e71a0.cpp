#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, C_00861d48_GetSourceMatrix, 0x006E6D90, (void* self, void* edx, int a2, int a3));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall C_00861d48_CreateEntryMatrix(
    C_00861d48* self, void* /*edx*/, int outputAddress, int entryIndex)
{
    alignas(8) std::uint8_t transformedBytes[0xC];
    alignas(8) std::uint8_t translationBytes[0xC];
    alignas(8) std::uint8_t sourceMatrixBytes[0x8];

    const std::uint32_t entryOffset =
        20u * static_cast<std::uint32_t>(entryIndex);

    auto* entries = reinterpret_cast<std::uint8_t*>(
        CSF_AT(std::uint32_t, self, 0x3C));

    C_00861d48_GetSourceMatrix(
        self, nullptr, reinterpret_cast<int>(sourceMatrixBytes),
        CSF_AT(int, entries + entryOffset, 4));

    entries = reinterpret_cast<std::uint8_t*>(
        CSF_AT(std::uint32_t, self, 0x3C));
    auto* entry = entries + entryOffset;

    CSF_AT(float, translationBytes, 0) = CSF_AT(float, entry, 8);
    CSF_AT(float, translationBytes, 4) = CSF_AT(float, entry, 12);
    CSF_AT(float, translationBytes, 8) = CSF_AT(float, entry, 16);

    if (CSF_AT(float, translationBytes, 0) != g_ZeroVectorX ||
        CSF_AT(float, translationBytes, 4) != g_ZeroVectorY ||
        CSF_AT(float, translationBytes, 8) != g_ZeroVectorZ)
    {
        RwV3dTransformPoints(
            reinterpret_cast<int>(transformedBytes),
            reinterpret_cast<int>(translationBytes), 1,
            CSF_AT(int, sourceMatrixBytes, 0));

        CSF_AT(float, translationBytes, 0) =
            CSF_AT(float, transformedBytes, 0);
        CSF_AT(float, translationBytes, 8) =
            CSF_AT(float, transformedBytes, 8);
        CSF_AT(float, translationBytes, 4) =
            CSF_AT(float, transformedBytes, 4);

        GfxMatrix_CopyTranslationAndUpdate(
            sourceMatrixBytes, nullptr,
            reinterpret_cast<const uint32_t*>(translationBytes));
    }

    auto* matrix = RwMatrixCreate();
    auto* source = CSF_AT(std::uint32_t*, sourceMatrixBytes, 0);
    auto* output = reinterpret_cast<void*>(outputAddress);

    CSF_AT(std::uint32_t*, output, 0) = matrix;
    std::memcpy(matrix, source, 0x40);

    const bool ownsSource = CSF_AT(std::uint8_t, sourceMatrixBytes, 4) != 0;
    CSF_AT(std::uint8_t, output, 4) = 1;

    if (ownsSource)
        RwMatrixDestroy(CSF_AT(std::uint32_t*, sourceMatrixBytes, 0));

    return outputAddress;
}

HOOK(0x006E71A0, C_00861d48_CreateEntryMatrix, "auto");
