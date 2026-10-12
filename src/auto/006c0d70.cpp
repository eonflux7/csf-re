#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int (__cdecl*)(int atomic, int data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(int, __cdecl, RwV3dTransformPoints, 0x00623040, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(float*, __fastcall, Matrix_InvertWithStoredTarget, 0x006ADB40, (S_006adb40* self, void* , float** a2));

CSF_FUNC(void, __fastcall, C_00860630_DestroyEmbeddedState, 0x006BF180, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00860700_SetFrame, 0x006BF380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, C_00860700_GetFrameMatrix, 0x006BF970, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00860700_ConstructEmbeddedState, 0x006BFD00, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GfxFrames_AccumulateSphere, 0x006EE8C0, (void* self, void* edx, float* a2));

static std::uint32_t* __cdecl GfxFrames_ComputeClumpLocalBoundingSphere(
    std::uint32_t* result, int clump)
{
    alignas(8) std::uint8_t stateBytes[0x20];
    alignas(8) std::uint8_t sphereBytes[0x10];
    alignas(8) std::uint8_t collectionBytes[0x10];
    alignas(8) std::uint8_t boundsBytes[0x10];
    alignas(8) std::uint8_t centerBytes[0xC];

    // These overlapping locals serve first as a matrix wrapper, then as XYZ.
    alignas(8) std::uint8_t transformedBytes[0xC];
    alignas(8) std::uint8_t inverseMatrixBytes[0x8];

    CSF_AT(std::uint32_t, collectionBytes, 0x0) = 0;
    CSF_AT(std::uint32_t, collectionBytes, 0x4) = 0;
    CSF_AT(std::uint32_t, collectionBytes, 0x8) = 0;
    CSF_AT(int, collectionBytes, 0xC) = 10;

    RpClumpForAllAtomics(
        clump,
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006C0B20)),
        reinterpret_cast<int>(collectionBytes));

    C_00860700_ConstructEmbeddedState(stateBytes, nullptr);
    C_00860700_SetFrame(
        stateBytes, nullptr,
        CSF_AT(int, reinterpret_cast<void*>(clump), 0x4));

    CSF_AT(float*, inverseMatrixBytes, 0x0) =
        reinterpret_cast<float*>(RwMatrixCreate());
    CSF_AT(std::uint8_t, inverseMatrixBytes, 0x4) = 1;

    void* sourceMatrix = C_00860700_GetFrameMatrix(
        stateBytes, nullptr, reinterpret_cast<int>(transformedBytes + 0x4));

    // The packet also lists this direct-call address as a vtable reference.
    (void)CSF_VTBL(0x006ADB40);
    Matrix_InvertWithStoredTarget(
        reinterpret_cast<S_006adb40*>(sourceMatrix), nullptr,
        reinterpret_cast<float**>(inverseMatrixBytes));

    if (CSF_AT(std::uint8_t, transformedBytes, 0x8))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, transformedBytes, 0x4));

    CSF_AT(std::uint32_t, boundsBytes, 0x0) = 0;
    CSF_AT(std::uint32_t, boundsBytes, 0x4) = 0;
    CSF_AT(std::uint32_t, boundsBytes, 0x8) = 0;
    CSF_AT(std::uint32_t, boundsBytes, 0xC) = 0;

    std::uint32_t sphereIndex = 0;
    std::uint32_t sphereOffset = 0;
    while (sphereIndex < CSF_AT(std::uint32_t, collectionBytes, 0x8))
    {
        auto* sphere = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, collectionBytes, 0x0) + sphereOffset);

        CSF_AT(std::uint32_t, centerBytes, 0x0) =
            CSF_AT(std::uint32_t, sphere, 0x0);
        CSF_AT(std::uint32_t, centerBytes, 0x4) =
            CSF_AT(std::uint32_t, sphere, 0x4);
        CSF_AT(std::uint32_t, centerBytes, 0x8) =
            CSF_AT(std::uint32_t, sphere, 0x8);

        CSF_AT(std::uint32_t, transformedBytes, 0x0) = 0;
        CSF_AT(std::uint32_t, transformedBytes, 0x4) = 0;
        CSF_AT(std::uint32_t, transformedBytes, 0x8) = 0;

        RwV3dTransformPoints(
            reinterpret_cast<int>(transformedBytes),
            reinterpret_cast<int>(centerBytes), 1,
            CSF_AT(int, inverseMatrixBytes, 0x0));

        CSF_AT(float, sphereBytes, 0xC) = CSF_AT(float, sphere, 0xC);
        CSF_AT(std::uint32_t, sphereBytes, 0x0) =
            CSF_AT(std::uint32_t, transformedBytes, 0x0);
        CSF_AT(std::uint32_t, sphereBytes, 0x4) =
            CSF_AT(std::uint32_t, transformedBytes, 0x4);
        CSF_AT(std::uint32_t, sphereBytes, 0x8) =
            CSF_AT(std::uint32_t, transformedBytes, 0x8);

        GfxFrames_AccumulateSphere(
            boundsBytes, nullptr, reinterpret_cast<float*>(sphereBytes));

        ++sphereIndex;
        sphereOffset += 16;
    }

    result[0] = CSF_AT(std::uint32_t, boundsBytes, 0x0);
    result[1] = CSF_AT(std::uint32_t, boundsBytes, 0x4);
    result[2] = CSF_AT(std::uint32_t, boundsBytes, 0x8);
    result[3] = CSF_AT(std::uint32_t, boundsBytes, 0xC);

    if (CSF_AT(std::uint8_t, inverseMatrixBytes, 0x4))
        RwMatrixDestroy(CSF_AT(std::uint32_t*, inverseMatrixBytes, 0x0));

    C_00860630_DestroyEmbeddedState(stateBytes, nullptr);

    if (CSF_AT(void*, collectionBytes, 0x0))
        Mem_Free(CSF_AT(void*, collectionBytes, 0x0));

    return result;
}

HOOK(0x006C0D70, GfxFrames_ComputeClumpLocalBoundingSphere, "auto");
