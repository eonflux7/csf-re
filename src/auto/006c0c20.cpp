#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int (__cdecl*)(int atomic, int data);

CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void*));
CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, S_006abb80_ComputeAxisAlignedBounds, 0x006AC000, (S_006abb80* self, void* , float* maxX, float* minX, float* maxY, float* minY, float* maxZ, float* minZ));
CSF_FUNC(S_00402350*, __fastcall, GfxMatrix_TransformFrame, 0x006AC400, (uint32_t* self, void* , S_00402350* output, int* matrix));
CSF_FUNC(float*, __fastcall, S_006abb80_ExpandBounds, 0x006AC5E0, (S_006abb80* self, void* , S_006abb80* other));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));
CSF_FUNC(char, __fastcall, GfxFrame_InitRotationDegreesFromMatrix, 0x006BF380, (void* self, void* , int matrixBase));
CSF_FUNC(S_006adb40*, __fastcall, C_00860700_GetFrameLtm, 0x006BF970, (C_00860700* self, void* , int resultOut));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));

static int __cdecl GfxFrames_ComputeClumpLocalBounds(
    int outputAddress, int clump, float options)
{
    alignas(8) std::uint8_t transformedBoundsBytes[0x3C];
    alignas(8) std::uint8_t aggregateBoundsBytes[0x3C];
    alignas(8) std::uint8_t frameStateBytes[0x20];
    alignas(8) std::uint8_t frameLtmBytes[0x8];
    alignas(8) std::uint8_t boundsListBytes[0x14];
    alignas(8) std::uint8_t inverseMatrixBytes[0x8];

    CSF_AT(std::uint32_t, boundsListBytes, 0x0) = 0;
    CSF_AT(std::uint32_t, boundsListBytes, 0x4) = 0;
    CSF_AT(std::uint32_t, boundsListBytes, 0x8) = 0;
    CSF_AT(int, boundsListBytes, 0xC) = 10;
    std::memcpy(boundsListBytes + 0x10, &options, 1);

    RpClumpForAllAtomics(
        clump,
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006C0A40)),
        reinterpret_cast<int>(boundsListBytes));

    auto* frameState = reinterpret_cast<C_00860700*>(frameStateBytes);
    GfxObject_ConstructEmbeddedState(frameState, nullptr);
    GfxFrame_InitRotationDegreesFromMatrix(
        frameState, nullptr,
        CSF_AT(int, reinterpret_cast<void*>(clump), 0x4));

    CSF_AT(float*, inverseMatrixBytes, 0x0) =
        reinterpret_cast<float*>(RwMatrixCreate());
    CSF_AT(std::uint8_t, inverseMatrixBytes, 0x4) = 1;

    S_006adb40* frameLtm = C_00860700_GetFrameLtm(
        frameState, nullptr, reinterpret_cast<int>(frameLtmBytes));

    (void)CSF_VTBL(0x006ADB40);
    Matrix_InvertWithStoredTarget(
        frameLtm, nullptr, reinterpret_cast<float**>(inverseMatrixBytes));

    if (CSF_AT(std::uint8_t, frameLtmBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, frameLtmBytes, 0x0));

    (void)CSF_VTBL(0x00402350);
    S00402350_InitTransform(
        reinterpret_cast<S_00402350*>(aggregateBoundsBytes), nullptr);

    std::uint32_t index = 0;
    std::uint32_t byteOffset = 0;
    if (CSF_AT(std::uint32_t, boundsListBytes, 0x8))
    {
        do
        {
            auto* transformed = GfxMatrix_TransformFrame(
                reinterpret_cast<std::uint32_t*>(
                    CSF_AT(std::uint8_t*, boundsListBytes, 0x0) + byteOffset),
                nullptr,
                reinterpret_cast<S_00402350*>(transformedBoundsBytes),
                reinterpret_cast<int*>(inverseMatrixBytes));

            S_006abb80_ExpandBounds(
                reinterpret_cast<S_006abb80*>(aggregateBoundsBytes),
                nullptr,
                reinterpret_cast<S_006abb80*>(transformed));

            ++index;
            byteOffset += 60;
        }
        while (index < CSF_AT(std::uint32_t, boundsListBytes, 0x8));
    }

    float maxX;
    float maxY;
    float minY;
    float maxZ;
    float minZ;

    // The original reuses its argument slots as outputs. Preserve their
    // incoming bits if the bounds routine leaves either slot unchanged.
    std::memcpy(&minY, &clump, sizeof(minY));
    S_006abb80_ComputeAxisAlignedBounds(
        reinterpret_cast<S_006abb80*>(aggregateBoundsBytes), nullptr,
        &maxX, &options, &maxY, &minY, &maxZ, &minZ);

    void* output = reinterpret_cast<void*>(outputAddress);
    CSF_AT(float, output, 0xC) = options;
    std::memcpy(static_cast<std::uint8_t*>(output) + 0x10,
                &minY, sizeof(minY));
    CSF_AT(float, output, 0x4) = maxY;
    CSF_AT(float, output, 0x14) = minZ;
    CSF_AT(float, output, 0x0) = maxX;
    CSF_AT(float, output, 0x8) = maxZ;

    if (CSF_AT(std::uint8_t, inverseMatrixBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, inverseMatrixBytes, 0x0));

    GfxObject_DestroyEmbeddedState(
        reinterpret_cast<C_00860630*>(frameStateBytes), nullptr);

    if (CSF_AT(void*, boundsListBytes, 0x0))
        CoreMemory_Free(CSF_AT(void*, boundsListBytes, 0x0));

    return outputAddress;
}

HOOK(0x006C0C20, GfxFrames_ComputeClumpLocalBounds, "auto");
